import csv
import os
import subprocess
from typing import Final

from datatypes import payload_c_struct_lines
from jinja2 import Environment, FileSystemLoader
from query import Query

# this script's code is heavily inspired from apss2's https://github.com/APSS-KESSLER/apss-2-scripts

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEMPLATES_DIR = os.path.join(SCRIPT_DIR, "templates")
CSV_PATH = os.path.join(SCRIPT_DIR, "queries.csv")

INCLUDE_DIR = os.path.join(SCRIPT_DIR, "..", "Core", "Src", "spi", "include", "queries")
SRC_DIR = os.path.join(SCRIPT_DIR, "..", "Core", "Src", "spi", "src", "queries")

PAYLOAD_IDS: Final = {
    "THERMAL": 1,
    "RADIATION": 2,
    "CAMERA": 3,
}


def load_queries() -> dict[str, list[Query]]:
    queries: dict[str, list[Query]] = {p: [] for p in PAYLOAD_IDS}

    with open(CSV_PATH, newline="") as f:
        for row in csv.DictReader(f):
            payload = row["payload"].strip().upper()
            queries[payload].append(Query.from_csv_row(row))

    for payload in PAYLOAD_IDS:
        queries[payload].sort(key=lambda q: q.query_code)

    return queries


def write_file(path: str, content: str) -> None:
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(content)
    print(f"wrote {os.path.relpath(path, SCRIPT_DIR)}")


def create_deserialize_functions(
    payload: str, queries: list[Query], env: Environment
) -> list[str]:
    written = []
    template = env.get_template("spi_deserialize.c.j2")
    path = os.path.join(SRC_DIR, f"spi_{payload.lower()}_deserialize.c")

    if os.path.exists(path):
        return written

    body = []
    body.append(f'#include "queries/spi_{payload.lower()}_generated.h"')
    body.append("")
    for q in queries:
        if not q.deserialize_fn:
            continue
        body.append(template.render(q=q))

    if not body:
        return written

    write_file(path, "\n".join(body))
    written.append(path)
    return written


def main() -> None:
    queries = load_queries()

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
    p = os.path.join(INCLUDE_DIR, "..", "spi_query.h")
    write_file(p, query_h)
    written_paths.append(p)

    for payload in PAYLOAD_IDS:
        rows = queries[payload]

        header = env.get_template("spi_generated.h.j2").render(
            payload=payload, queries=rows
        )
        h_path = os.path.join(INCLUDE_DIR, f"spi_{payload.lower()}_generated.h")
        write_file(h_path, header)
        written_paths.append(h_path)

        source = env.get_template("spi_generated.c.j2").render(
            payload=payload, queries=rows
        )
        c_path = os.path.join(SRC_DIR, f"spi_{payload.lower()}_generated.c")
        write_file(c_path, source)
        written_paths.append(c_path)

        written_paths += create_deserialize_functions(payload, rows, env)

    subprocess.run(["clang-format", "-i", *written_paths], check=True)
    print("formatted the code with clang format")


if __name__ == "__main__":
    main()
