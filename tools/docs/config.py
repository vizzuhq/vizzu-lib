# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from pathlib import Path
from typing import Optional

import yaml


class MkdocsConfig:
    # pylint: disable=too-few-public-methods

    @staticmethod
    def _format_url(url: Optional[str]) -> Optional[str]:
        if url and url.endswith("/"):
            return url[:-1]
        return url

    @staticmethod
    def _format(config: dict) -> dict:
        if "site_url" in config:
            config["site_url"] = MkdocsConfig._format_url(config["site_url"])
        return config

    @staticmethod
    def load(config: Path) -> dict:
        with open(config, "rt", encoding="utf8") as f_yml:
            return MkdocsConfig._format(yaml.load(f_yml, Loader=yaml.FullLoader))
