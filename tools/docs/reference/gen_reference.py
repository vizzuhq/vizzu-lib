# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from contextlib import chdir
import os
from pathlib import Path
import shutil
import sys

import mkdocs_gen_files


REPO_PATH = Path(__file__).parent / ".." / ".." / ".."
TOOLS_PATH = REPO_PATH / "tools"
MKDOCS_PATH = TOOLS_PATH / "docs"
GEN_PATH = MKDOCS_PATH / "reference"

sys.path.insert(0, str(TOOLS_PATH / "modules"))
sys.path.insert(0, str(TOOLS_PATH / "ci"))

from node import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Node,
)
from vizzu import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Vizzu,
)
from markdown_format import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Markdown,
)


class Reference:
    # pylint: disable=too-few-public-methods

    @staticmethod
    def generate(folder: str) -> None:
        Reference._gen_reference(folder)

    @staticmethod
    def _gen_reference(folder: str) -> None:
        tmp_dir = GEN_PATH / "tmp"
        if os.path.exists(tmp_dir):
            shutil.rmtree(tmp_dir)

        Node.node(False, GEN_PATH / "gen_reference.cjs")

        for path in Path(tmp_dir).rglob("*.md"):
            with open(path, "rt", encoding="utf8") as f_src:
                content = f_src.read()
                content = content.replace("##### ", "").replace("#### ", "")
                content = Vizzu.set_version(content)
                content = Markdown.format(content)
                with mkdocs_gen_files.open(
                    f"{folder}/{path.relative_to(tmp_dir)}", "w"
                ) as f_dst:
                    f_dst.write(content)


def main() -> None:
    with chdir(REPO_PATH):
        Reference.generate("reference")


main()
