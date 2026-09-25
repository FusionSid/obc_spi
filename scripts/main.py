import os
import re
import csv
import argparse
import subprocess
from typing import Final

from jinja2 import Environment, FileSystemLoader

from query import Query
from datatypes import payload_c_struct_lines

# this script's code is heavily inspired from apss2's https://github.com/APSS-KESSLER/apss-2-scripts

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEMPLATES_DIR = os.path.join(SCRIPT_DIR, "templates")
DEFAULT_SPI_DIRECTORY = os.path.join(SCRIPT_DIR, "..", "Core", "Src", "spi")

PAYLOAD_IDS: Final = {
    "THERMAL": 1,
    "RADIATION": 2,
    "CAMERA": 3,
}


def load_queries(input_file: str) -> dict[str, list[Query]]:
    queries: dict[str, list[Query]] = {p: [] for p in PAYLOAD_IDS}
    seen_codes: dict[str, set[int]] = {p: set() for p in PAYLOAD_IDS}

    with open(input_file, newline="") as f:
        for line_num, row in enumerate(csv.DictReader(f), start=2):
            payload = row["payload"].strip().upper()
            if payload not in PAYLOAD_IDS:
                raise ValueError(
                    f"queries.csv line {line_num}: unknown payload {payload!r}, "
                    f"expected one of {list(PAYLOAD_IDS)}"
                )
            query = Query.from_csv_row(row)
            if query.query_code in seen_codes[payload]:
                raise ValueError(
                    f"queries.csv line {line_num}: duplicate query_code "
                    f"0x{query.query_code:02X} for payload {payload}"
                )
            seen_codes[payload].add(query.query_code)
            queries[payload].append(query)

    for payload in PAYLOAD_IDS:
        queries[payload].sort(key=lambda q: q.query_code)

    return queries


def write_file(path: str, content: str) -> None:
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(content)
    print(f"wrote {os.path.relpath(path, SCRIPT_DIR)}")


def _extract_user_code_block(text: str, tag: str) -> str | None:
    pattern = re.compile(
        r"/\* USER CODE BEGIN {tag} \*/(.*?)/\* USER CODE END {tag} \*/".format(
            tag=re.escape(tag)
        ),
        re.DOTALL,
    )
    m = pattern.search(text)
    if m is None:
        return None
    return m.group(1).strip("\n")


def _extract_function_body(text: str, func_name: str) -> str | None:
    m = re.search(r"\b" + re.escape(func_name) + r"\s*\(", text)
    if m is None:
        return None

    i = m.end() - 1
    depth = 0
    while i < len(text):
        if text[i] == "(":
            depth += 1
        elif text[i] == ")":
            depth -= 1
            if depth == 0:
                break
        i += 1
    else:
        return None

    open_brace = text.find("{", i)
    if open_brace == -1:
        return None

    depth = 0
    k = open_brace
    while k < len(text):
        if text[k] == "{":
            depth += 1
        elif text[k] == "}":
            depth -= 1
            if depth == 0:
                break
        k += 1
    else:
        return None

    return text[open_brace + 1 : k].strip("\n")


def _merge_deserialize_body(old_text: str | None, q: Query) -> tuple[str, bool]:
    default_body = q.default_deserialize_body()
    tag = q.deserialize_fn

    if old_text is not None:
        preserved = _extract_user_code_block(old_text, tag)  # type: ignore
        if preserved is None:
            preserved = _extract_function_body(old_text, tag)  # type: ignore
        if preserved is not None:
            implemented = " ".join(preserved.split()) != " ".join(default_body.split())
            return preserved, implemented

    return default_body, False


def _merge_includes(old_text: str | None) -> str:
    if old_text is None:
        return ""

    preserved = _extract_user_code_block(old_text, "Includes")
    if preserved is not None:
        return preserved

    extra = [
        line.strip()
        for line in old_text.splitlines()
        if line.strip().startswith("#include") and "_generated.h" not in line
    ]
    return "\n".join(extra)


def create_deserialize_functions(
    payload: str, queries: list[Query], env: Environment, src_dir: str
) -> list[str]:
    written = []
    template = env.get_template("spi_deserialize.c.j2")
    path = os.path.join(src_dir, f"spi_{payload.lower()}_deserialize.c")

    old_text = None
    if os.path.exists(path):
        with open(path) as f:
            old_text = f.read()

    deserializable = [q for q in queries if q.deserialize_fn]
    if not deserializable:
        return written

    user_includes = _merge_includes(old_text)

    body = [f'#include "queries/spi_{payload.lower()}_generated.h"']
    body.append("/* USER CODE BEGIN Includes */")
    if user_includes:
        body.append(user_includes)
    body.append("/* USER CODE END Includes */")
    body.append("")

    for q in deserializable:
        user_code, implemented = _merge_deserialize_body(old_text, q)
        body.append(template.render(q=q, user_code=user_code, implemented=implemented))

    write_file(path, "\n".join(body))
    written.append(path)
    return written


def main() -> None:
    parser = argparse.ArgumentParser(
        description="generate spi queries functions apss-3"
    )

    parser.add_argument("-i", "--input", required=True, help="Path to the input CSV file, containing the queries")
    parser.add_argument(
        "-o", "--output", required=False, help="Path to the output spi/ directory"
    )

    args = parser.parse_args()

    if not os.path.exists(args.input):
        raise ValueError(f"file {args.input} does not even exist")

    spi_dir = DEFAULT_SPI_DIRECTORY if args.output is None else args.output

    src_dir = os.path.join(spi_dir, "src", "queries")
    include_dir = os.path.join(spi_dir, "include", "queries")

    queries = load_queries(args.input)

    env = Environment(
        loader=FileSystemLoader(TEMPLATES_DIR),
        trim_blocks=True,
        lstrip_blocks=True,
    )
    env.globals["struct_lines"] = (
        payload_c_struct_lines  # ty: ignore[invalid-assignment]
    )

    written_paths = []

    query_h = env.get_template("spi_query.h.j2").render(
        queries=queries, payload_order=list(PAYLOAD_IDS.keys()), payload_ids=PAYLOAD_IDS
    )
    p = os.path.join(include_dir, "..", "spi_query.h")
    write_file(p, query_h)
    written_paths.append(p)

    for payload in PAYLOAD_IDS:
        rows = queries[payload]

        header = env.get_template("spi_generated.h.j2").render(
            payload=payload, queries=rows
        )
        h_path = os.path.join(include_dir, f"spi_{payload.lower()}_generated.h")
        write_file(h_path, header)
        written_paths.append(h_path)

        source = env.get_template("spi_generated.c.j2").render(
            payload=payload, queries=rows
        )
        c_path = os.path.join(src_dir, f"spi_{payload.lower()}_generated.c")
        write_file(c_path, source)
        written_paths.append(c_path)

        written_paths += create_deserialize_functions(payload, rows, env, src_dir)

    subprocess.run(["clang-format", "-i", *written_paths], check=True)
    print("formatted the code with clang format")


if __name__ == "__main__":
    main()
