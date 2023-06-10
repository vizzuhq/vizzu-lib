"""Touch files."""

import argparse
from pathlib import Path


def main() -> None:
    """
    The main method.
    Touch files.
    """

    parser = argparse.ArgumentParser(
        prog="touch.py",
        description="Touch files.",
    )
    parser.add_argument("-f", "--flag", type=str, required=True)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()

    flag = Path(args.flag)

    if not args.check or flag.exists():
        with open(flag, "w", encoding="utf8") as file_handler:
            file_handler.write("")


main()
