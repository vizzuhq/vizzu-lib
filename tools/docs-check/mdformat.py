"""Run mdformat and check the output"""

import sys
import subprocess


def main() -> None:
    """
    The main method.
    Run mdformat and check the output.
    """

    with subprocess.Popen(
        sys.argv[1:], stdout=subprocess.PIPE, stderr=subprocess.PIPE
    ) as process:
        out, err = process.communicate()
        if out or err or process.returncode:
            if out:
                print(out.decode())
            if err:
                print(err.decode())
            raise RuntimeError("failed to run mdformat")


main()
