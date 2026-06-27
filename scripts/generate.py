import os
import csv
from typing import Final
import subprocess

from jinja2 import Environment, FileSystemLoader

TEMPLATE1_FILENAME: Final = "spi_queries.j2"
TEMPLATE2_FILENAME: Final = "query_functions.j2"

OUTPUT1_LOCATION: Final = "Core/Src/spi/include/spi_query.h"
OUTPUT2_LOCATION: Final = "Core/Src/spi/include/spi_queries.h"

SCRIPT_DIR = os.path.dirname(__file__)

PAYLOADS: Final = ["THERMAL", "RADIATION", "CAMERA"]


def get_queries() -> dict:
    queries = {
        "RADIATION": [],
        "THERMAL": [],
        "CAMERA": [],
    }

    with open(os.path.join(SCRIPT_DIR, "queries.csv")) as f:
        reader = csv.DictReader(f)

        for row in reader:
            queries[row["payload"]].append(
                {
                    "query_code": int(row["query"]),
                    "command": row["command"],
                    "tx_size": int(row["tx_size"]),
                    "rx_size": int(row["rx_size"]),
                }
            )

    return queries


def main() -> None:
    env = Environment(
        loader=FileSystemLoader(SCRIPT_DIR),
        trim_blocks=True,
        lstrip_blocks=True,
    )

    template1 = env.get_template(TEMPLATE1_FILENAME)
    output1_path = os.path.join(SCRIPT_DIR, "..", OUTPUT1_LOCATION)
    with open(output1_path, "w") as f:
        f.write(template1.render(queries=get_queries()))

    template2 = env.get_template(TEMPLATE2_FILENAME)
    output2_path = os.path.join(SCRIPT_DIR, "..", OUTPUT2_LOCATION)
    with open(output2_path, "w") as f:
        f.write(template2.render(queries=get_queries()))

    subprocess.run(["clang-format", "-i", OUTPUT1_LOCATION, OUTPUT2_LOCATION])


if __name__ == "__main__":
    main()
