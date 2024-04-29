# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from pathlib import Path
import sys


REPO_PATH = Path(__file__).parent / ".." / ".."
TOOLS_PATH = REPO_PATH / "tools"
MKDOCS_PATH = TOOLS_PATH / "docs"


sys.path.insert(0, str(TOOLS_PATH / "modules"))

from chdir import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    chdir,
)
from vizzu import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Vizzu,
)


PLACEHOLDER = """placeholders:
  LIB_MINOR_VERSION: "{}"
  LIB_PATCH_VERSION: "{}"
settings:
  auto_placeholder_tables: false
"""


class Placeholder:
    # pylint: disable=too-few-public-methods

    @staticmethod
    def generate() -> None:
        with open(
            MKDOCS_PATH / "placeholder-plugin.yaml", "w", encoding="utf8"
        ) as fh_config:
            version = Vizzu.get_vizzu_full_version()
            minor = f"{version[0]}.{version[1]}"
            patch = f"{version[0]}.{version[1]}.{version[2]}"
            fh_config.write(PLACEHOLDER.format(minor, patch))


def main() -> None:
    with chdir(REPO_PATH):
        Placeholder.generate()


main()
