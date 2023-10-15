# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from pathlib import Path
import re


REPO_PATH = Path(__file__).parent / ".." / ".."
TOOLS_PATH = REPO_PATH / "tools"
MKDOCS_PATH = TOOLS_PATH / "docs"

VIZZU_VERSION = ""

VIZZU_DOC_URL = "https://github.com/vizzuhq/vizzu-lib-doc"
VIZZU_SITE_URL = "https://lib.vizzuhq.com"
VIZZU_CDN_URL = "https://cdn.jsdelivr.net/npm/vizzu"


class Vizzu:
    _vizzu_version = ""

    @staticmethod
    def get_vizzu_full_version() -> list:
        with open(
            REPO_PATH / "src" / "chart" / "main" / "version.cpp",
            "r",
            encoding="utf8",
        ) as f_version:
            content = f_version.read()
            version = re.search(r"version\(\s*(\d+),\s*(\d+),\s*(\d+)\s*\)", content)
            return [
                version.group(1),  # type: ignore
                version.group(2),  # type: ignore
                version.group(3),  # type: ignore
            ]

    @staticmethod
    def get_vizzu_version() -> str:
        if VIZZU_VERSION:
            return VIZZU_VERSION
        if not Vizzu._vizzu_version:
            version_parts = Vizzu.get_vizzu_full_version()
            Vizzu._vizzu_version = f"{version_parts[0]}.{version_parts[1]}"
        return Vizzu._vizzu_version

    @staticmethod
    def set_version(content: str, restore: bool = False) -> str:
        version = Vizzu.get_vizzu_version()
        if not restore:
            content = content.replace(
                f"{VIZZU_CDN_URL}@latest/",
                f"{VIZZU_CDN_URL}@{version}/",
            )
            content = content.replace(
                f"{VIZZU_SITE_URL}/latest/", f"{VIZZU_SITE_URL}/{version}/"
            )
        else:
            content = content.replace(
                f"{VIZZU_CDN_URL}@{version}/",
                f"{VIZZU_CDN_URL}@latest/",
            )
            content = content.replace(
                f"{VIZZU_SITE_URL}/{version}/",
                f"{VIZZU_SITE_URL}/latest/",
            )
        return content

    @staticmethod
    def set_version_showcase(content: str) -> str:
        version = Vizzu.get_vizzu_version()
        github_url = f"{VIZZU_DOC_URL}/tree/gh-pages"
        content = content.replace(
            f"{github_url}/latest/",
            f"{github_url}/{version}/",
        )
        return content
