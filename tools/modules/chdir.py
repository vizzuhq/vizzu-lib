# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

import os
from pathlib import Path
import sys
from typing import Union


if sys.version_info >= (3, 11):
    from contextlib import chdir  # pylint: disable=unused-import
else:
    # TODO: remove once support for Python 3.10 is dropped
    from contextlib import contextmanager

    @contextmanager
    def chdir(path: Union[str, Path]):
        old_wd = os.getcwd()
        os.chdir(path)
        try:
            yield
        finally:
            os.chdir(old_wd)
