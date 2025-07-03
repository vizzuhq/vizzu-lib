# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

import sys
import subprocess
import os


def main() -> None:
    env = os.environ.copy()

    env["PYTHONWARNINGS"] = "ignore:pkg_resources is deprecated"

    with subprocess.Popen(
        sys.argv[1:], stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env
    ) as process:
        out, err = process.communicate()

    out_str = out.decode() if out else ""
    err_str = err.decode() if err else ""

    if out_str or err_str or process.returncode != 0:
        if out_str:
            print(out_str)
        if err_str:
            print(err_str)
        raise RuntimeError(f"failed to run {sys.argv[1]}")


if __name__ == "__main__":
    main()
