# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from pathlib import Path
import sys

import mkdocs_gen_files


REPO_PATH = Path(__file__).parent / ".." / ".." / ".."
TOOLS_PATH = REPO_PATH / "tools"
MKDOCS_PATH = TOOLS_PATH / "docs"

sys.path.insert(0, str(TOOLS_PATH / "modules"))

from chdir import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    chdir,
)
from vizzu import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Vizzu,
)


class VizzuUrl:
    # pylint: disable=too-few-public-methods

    TEMPLATE = 'const vizzu = "{vizzu}";\n\nexport default vizzu;\n'

    @staticmethod
    def generate(dst: str) -> None:
        with mkdocs_gen_files.open(dst, "w") as f_vizzu:
            f_vizzu.write(VizzuUrl.TEMPLATE.format(vizzu=Vizzu.get_vizzu_backend_url()))


def main() -> None:
    with chdir(REPO_PATH):
        VizzuUrl.generate("assets/javascripts/vizzu.js")


main()
