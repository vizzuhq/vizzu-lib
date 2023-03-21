"""Touch DEV_BUILD_FLAG."""

import argparse
from pathlib import Path


def main() -> None:
    """
    The main method.
    Touch DEV_BUILD_FLAG.
    """

    parser = argparse.ArgumentParser(
        prog="touch.py",
        description="Touch DEV_BUILD_FLAG",
    )
    parser.add_argument("-f", "--flag", type=str, required=True)
    args = parser.parse_args()

    flag = Path(args.flag)

    with open(flag, "w", encoding="utf8") as file_handler:
        file_handler.write("")


main()
