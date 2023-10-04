# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

import mdformat


class Markdown:
    # pylint: disable=too-few-public-methods

    @staticmethod
    def format(content: str) -> str:
        return mdformat.text(  # type: ignore
            content,
            options={"wrap": 80, "end-of-line": "keep", "line-length": 70},
            extensions={
                "gfm",
                "tables",
                "footnote",
                "frontmatter",
                "configblack",
                "admonition",
            },
            codeformatters={
                "python",
                "bash",
                "sh",
                "json",
                "toml",
                "yaml",
                "javascript",
                "js",
                "css",
                "html",
                "xml",
            },
        )
