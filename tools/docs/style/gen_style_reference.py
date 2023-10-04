# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from pathlib import Path
import sys

import mkdocs_gen_files


REPO_PATH = Path(__file__).parent / ".." / ".." / ".."
TOOLS_PATH = REPO_PATH / "tools"
MKDOCS_PATH = TOOLS_PATH / "docs"
GEN_PATH = MKDOCS_PATH / "style"

sys.path.insert(0, str(TOOLS_PATH / "modules"))

from chdir import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    chdir,
)
from node import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Node,
)
from vizzu import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Vizzu,
)


class StyleReference:
    # pylint: disable=too-few-public-methods

    @staticmethod
    def generate(dst: str) -> None:
        content = Node.node(
            True,
            GEN_PATH / "gen_style_reference.mjs",
            Vizzu.get_vizzu_styleref_backend_url(),
        )
        with mkdocs_gen_files.open(dst, "a") as f_index:
            f_index.write(f"\n{content}\n")


def main() -> None:
    with chdir(REPO_PATH):
        StyleReference.generate(dst="tutorial/style.md")


main()
