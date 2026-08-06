import csv
import os
import subprocess
from typing import Final

from jinja2 import Environment, FileSystemLoader

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
TEMPLATES_DIR = os.path.join(SCRIPT_DIR, "templates")
CSV_PATH = os.path.join(SCRIPT_DIR, "queries.csv")

PAYLOAD_IDS: Final = {
    "THERMAL": 1,
    "RADIATION": 2,
    "CAMERA": 3,
}
PAYLOAD_ORDER: Final = list(PAYLOAD_IDS.keys())

OUTPUTS: Final = {
    "spi_query.h.j2": "Core/Src/spi/include/spi_query.h",
    "spi_queries_raw.h.j2": "Core/Src/spi/include/spi_queries_raw.h",
    "spi_queries_raw.c.j2": "Core/Src/spi/src/spi_queries_raw.c",
}


def parse_int(value: str) -> int:
    return int(value.strip(), 0)


def parse_bool(value: str) -> bool:
    return value.strip().lower() == "true"


def load_queries() -> dict:
    queries = {payload: [] for payload in PAYLOAD_ORDER}

    with open(CSV_PATH) as f:
        reader = csv.DictReader(f)

        for row in reader:
            payload = row["payload"].strip().upper()

            queries[payload].append(
                {
                    "query_code": parse_int(row["query_code"]),
                    "command": row["command"].strip().upper(),
                    "tx_size": parse_int(row["tx_size"]),
                    "rx_size": parse_int(row["rx_size"]),
                    "expects_response": parse_bool(row["expects_response"]),
                    "notes": row.get("notes", "").strip(),
                }
            )

    for payload in PAYLOAD_ORDER:
        queries[payload].sort(key=lambda q: q["query_code"])

    return queries


def main() -> None:
    queries = load_queries()

    env = Environment(
        loader=FileSystemLoader(TEMPLATES_DIR),
        trim_blocks=True,
        lstrip_blocks=True,
    )

    written_paths = []

    for template_name, output_relative_path in OUTPUTS.items():
        template = env.get_template(template_name)

        output_path = os.path.join(SCRIPT_DIR, "..", output_relative_path)
        with open(output_path, "w") as f:
            f.write(
                template.render(
                    queries=queries,
                    payload_order=PAYLOAD_ORDER,
                    payload_ids=PAYLOAD_IDS,
                )
            )

        written_paths.append(output_path)
        print(f"wrote {output_relative_path}")

    subprocess.run(["clang-format", "-i", *written_paths], check=True)


if __name__ == "__main__":
    main()
