# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

import sys
import subprocess


def main() -> None:
    with subprocess.Popen(
        sys.argv[1:], stdout=subprocess.PIPE, stderr=subprocess.PIPE
    ) as process:
        out, err = process.communicate()
        if out or err or process.returncode:
            if out:
                print(out.decode())
            if err:
                print(err.decode())
            raise RuntimeError(f"failed to run {sys.argv[1]}")


main()
