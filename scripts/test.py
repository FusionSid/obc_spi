import os
from typing import Final

from jinja2 import Environment, FileSystemLoader

TEMPLATE_FILENAME: Final = "test.j2"
OUTPUT_LOCATION: Final = "Core/Src/spi/include/spi_query.h"


def main() -> None:
    script_dir = os.path.dirname(__file__)

    env = Environment(
        loader=FileSystemLoader(script_dir),
        trim_blocks=True,
        lstrip_blocks=True,
    )
    template = env.get_template(TEMPLATE_FILENAME)

    queries_list = [
        "NOT_USED",
        "ACKNOWLEDGE",
        "ECHO",
        "RTD_DATA",
        "THERMISTOR_DATA",
        "TIME",
        "STORE_ERRORS",
        "SEND_ERRORS",
    ]

    output_path = os.path.join(script_dir, "..", OUTPUT_LOCATION)
    with open(output_path, "w") as f:
        f.write(template.render(queries=queries_list))


if __name__ == "__main__":
    main()
