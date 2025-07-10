# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from pathlib import Path
from subprocess import PIPE, Popen
from typing import Union
import os


class Node:
    @staticmethod
    def node(strict: bool, script: Union[str, Path], *params: str) -> str:
        return Node.run(strict, "node", script, *params)

    @staticmethod
    def npx(strict: bool, script: Union[str, Path], *params: str) -> str:
        return Node.run(strict, "npx", script, *params)

    @staticmethod
    def run(strict: bool, exe: str, script: Union[str, Path], *params: str) -> str:
        env = os.environ.copy()
        env["NODE_OPTIONS"] = "--no-deprecation"

        with Popen(
            [exe, script, *params], stdin=PIPE, stdout=PIPE, stderr=PIPE, env=env
        ) as node:
            outs, errs = node.communicate()

        if errs and not strict:
            print(errs.decode())

        if node.returncode or (strict and errs):
            raise RuntimeError(f"failed to run {Path(script).stem}")

        return outs.decode()
