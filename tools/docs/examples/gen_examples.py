# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from contextlib import chdir
import os
from pathlib import Path
import re
import sys
from typing import List, Dict, Optional, Union, Tuple

import mkdocs_gen_files
import markdown


REPO_PATH = Path(__file__).parent / ".." / ".." / ".."
TOOLS_PATH = REPO_PATH / "tools"
MKDOCS_PATH = TOOLS_PATH / "docs"
GEN_PATH = MKDOCS_PATH / "examples"
WEB_CONTENT_PATH = REPO_PATH / "test" / "e2e" / "test_cases" / "web_content"
TEST_DATA_PATH = REPO_PATH / "test" / "e2e" / "test_data"
STATIC_EXAMPLES_PATH = WEB_CONTENT_PATH / "static"
OPERATION_EXAMPLES_PATH = WEB_CONTENT_PATH / "analytical_operations"
PRESET_EXAMPLES_PATH = WEB_CONTENT_PATH / "presets"
SHOWCASES_PATH = REPO_PATH / "docs" / "showcases"
JS_ASSETS_PATH = "assets/javascripts"


sys.path.insert(0, str(TOOLS_PATH / "modules"))
sys.path.insert(0, str(TOOLS_PATH / "ci"))
sys.path.insert(0, str(MKDOCS_PATH))

from node import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Node,
)
from vizzu import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Vizzu,
)
from markdown_format import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    Markdown,
)
from config import (  # pylint: disable=import-error, wrong-import-position, wrong-import-order
    MkdocsConfig,
)


class GenExamples:
    # pylint: disable=too-many-instance-attributes

    datafiles: Dict[str, bool] = {}

    datafile_re = re.compile(r"test_data\/(\w*).mjs")
    dataname_re = re.compile(r"import\s*\{\s*(.*)\s*}")
    title_re = re.compile(r'title\:\s"(.*)"')

    def __init__(self, name: str, src: Path, dst: str) -> None:
        self._name = name

        self._src = src
        self._dst = dst
        self._depth = len(dst.split("/")) + 1

        self._generated: List[str] = []
        self._indices: List[str] = []

        self._merge_subfolders = False
        self._video_thumbnails = False
        self._blocked: List[str] = []

    @property
    def merge_subfolders(self) -> bool:
        return self._merge_subfolders

    @merge_subfolders.setter
    def merge_subfolders(self, value: bool) -> None:
        self._merge_subfolders = value

    @property
    def video_thumbnails(self) -> bool:
        return self._video_thumbnails

    @video_thumbnails.setter
    def video_thumbnails(self, value: bool) -> None:
        self._video_thumbnails = value

    @property
    def blocked(self) -> List[str]:
        return self._blocked

    @blocked.setter
    def blocked(self, items: List[str]) -> None:
        self._blocked = items

    @staticmethod
    def _get_content(item: Path) -> str:
        with open(item, "r", encoding="utf8") as fh_item:
            return fh_item.read()

    def _get_title(self, item: Path, sub: Optional[str] = None) -> str:
        drop = ["Existingmeasure", "Newmeasure"]
        conjunction = ["and", "from", "to", "with"]
        title_parts = str(item.stem).split("_")
        contains_conjunction = False
        for index, title_part in enumerate(title_parts):
            if title_part in conjunction:
                contains_conjunction = True
                continue
            title_parts[index] = title_part.capitalize()
        title_parts[0] = title_parts[0].capitalize()
        if title_parts[0] in drop:
            title_parts = title_parts[1:]
        title = " ".join(title_parts)
        if not contains_conjunction:
            if title_parts[-1].isdigit():
                title = " ".join(
                    title_parts[1:-1] + [title_parts[0]] + [title_parts[-1]]
                )
            else:
                title = " ".join(title_parts[1:] + [title_parts[0]])
        if sub:
            title = " ".join([title, sub.capitalize()])
        title = title.replace("plot", " Plot")
        return title

    def _get_datafile(self, item: Path, content: str) -> str:
        datafiles = re.findall(GenExamples.datafile_re, content)
        if not datafiles or len(datafiles) > 1:
            raise ValueError(f"failed to find datafile {item}")
        datafile = "".join(datafiles)
        return datafile

    def _get_dataname(self, item: Path, content: str) -> str:
        datanames = re.findall(GenExamples.dataname_re, content)
        if not datanames or len(datanames) > 1:
            raise ValueError(f"failed to find dataname {item}")
        dataname = "".join(datanames)
        dataname = dataname.strip()
        return dataname

    def _get_sub(self, path: Path) -> Optional[str]:
        sub = os.path.relpath(path.parent, self._src)
        if self._merge_subfolders and sub != ".":
            return sub
        return None

    def _create_index(self, dst: str, depth: int, title: str) -> str:
        index = "/".join([self._dst, dst]) if dst != "." else self._dst
        assets = "/".join([".."] * (depth - 1))
        if index not in self._indices:
            with mkdocs_gen_files.open(f"{index}/index.md", "a") as fh_index:
                meta = """---\nhide:\n  - toc\n---"""
                fh_index.write(f"{meta}\n\n")
                fh_index.write(f"# {title}\n")
                fh_index.write(
                    f'<script src="{assets + "/" + JS_ASSETS_PATH}/thumbs.js"></script>\n'
                )
            self._indices.append(index)

        return index

    def _add_index_item(  # pylint: disable=too-many-arguments,too-many-positional-arguments
        self,
        index: str,
        title: str,
        href: str,
        thumbnail: Optional[str] = None,
        figcaption: bool = False,
    ) -> None:
        if not thumbnail:
            thumbnail = href
        if self._video_thumbnails:
            self._add_video(index, title, href, thumbnail, figcaption)
        else:
            self._add_image(index, title, href, thumbnail)

    def _add_index_sub_menus(self, index: str, groups: dict) -> None:
        with mkdocs_gen_files.open(f"{index}/index.md", "a") as fh_index:
            fh_index.write("<pre>")
        sorted_items = {}
        items = []
        for group_title in groups.keys():
            number = groups[group_title]["number"]
            if number:
                sorted_items[number] = group_title
            else:
                items.append(group_title)
        numbers = list(sorted_items.keys())
        numbers.sort()
        numbers.reverse()
        for number in numbers:
            items.insert(0, sorted_items[number])
        for group_title in items:
            self._add_index_item(
                index,
                group_title,
                groups[group_title]["dst"],
                groups[group_title]["dst"] + "/" + groups[group_title]["item"].stem,
                True,
            )
        with mkdocs_gen_files.open(f"{index}/index.md", "a") as fh_index:
            fh_index.write("</pre>\n")

    def _add_image(self, index: str, title: str, href: str, thumbnail: str) -> None:
        with mkdocs_gen_files.open(f"{index}/index.md", "a") as fh_index:
            fh_index.write(
                "["
                + f"![{title}]"
                + f"(./{thumbnail}.png)"
                + f"{{ class='image-gallery', title='{title}' }}"
                + "]"
                + f"(./{href}.md)\n"
            )

    def _add_video(  # pylint: disable=too-many-arguments,too-many-positional-arguments
        self, index: str, title: str, href: str, thumbnail: str, figcaption: bool
    ) -> None:
        with mkdocs_gen_files.open(f"{index}/index.md", "a") as fh_index:
            html = []
            html.append(f"<a href='./{href}/' title='{title}'>")
            if figcaption:
                html.append("<figure markdown class='image-figure'>")
                html.append(
                    "<video nocontrols autoplay muted loop class='image-gallery-w-caption'>"
                )
            else:
                html.append(
                    "<video nocontrols autoplay muted loop class='image-gallery'>"
                )
            html.append(f"<source src='./{thumbnail}.webm'")
            html.append(" type='video/webm'>")
            html.append(f"<source src='./{thumbnail}.mp4'")
            html.append(" type='video/mp4'>")
            html.append("</video>")
            if figcaption:
                html.append(f"<figcaption class='image-caption'>{title}</figcaption>")
                html.append("</figure>")
            html.append("</a>\n")
            fh_index.write("".join(html))

    @staticmethod
    def _find_dict_with_value(
        data: Union[dict, list], val: str
    ) -> Tuple[Optional[str], Optional[int]]:
        if isinstance(data, dict):
            return GenExamples._find_dict_with_value_in_dict(data, val)
        if isinstance(data, list):
            return GenExamples._find_dict_with_value_in_list(data, val)
        return None, None

    @staticmethod
    def _find_dict_with_value_in_dict(
        data: dict, val: str
    ) -> Tuple[Optional[str], Optional[int]]:
        for key, value in data.items():
            if isinstance(value, dict):
                if len(value) == 1 and val in value.values():
                    return key, None
                nested_result = GenExamples._find_dict_with_value(value, val)
                if nested_result != (None, None):
                    return nested_result
            if isinstance(value, list):
                nested_result = GenExamples._find_dict_with_value(value, val)
                if nested_result != (None, None):
                    return nested_result
            if value == val:
                return key, None
        return None, None

    @staticmethod
    def _find_dict_with_value_in_list(
        data: list, val: str
    ) -> Tuple[Optional[str], Optional[int]]:
        for i, item in enumerate(data):
            if isinstance(item, dict) and len(item) == 1 and val in item.values():
                return list(item.keys())[0], i
            if isinstance(item, (list, dict)):
                nested_result = GenExamples._find_dict_with_value(item, val)
                if nested_result != (None, None):
                    return nested_result
            if item == val:
                return None, i
        return None, None

    @staticmethod
    def _generate_example_data(datafile: str) -> None:
        if datafile not in GenExamples.datafiles:
            GenExamples.datafiles[datafile] = True

            datacontent = GenExamples._get_content(TEST_DATA_PATH / f"{datafile}.mjs")
            with mkdocs_gen_files.open(f"assets/data/{datafile}.js", "w") as fh_data:
                fh_data.write(datacontent)

            content = Node.node(
                True,
                GEN_PATH / "mjs2csv.mjs",
                f"{TEST_DATA_PATH}/{datafile}.mjs",
            )
            with mkdocs_gen_files.open(f"assets/data/{datafile}.csv", "w") as f_example:
                f_example.write(content)

    def _generate_example_js(  # pylint: disable=too-many-arguments,too-many-positional-arguments
        self,
        item: Path,
        item_name: str,
        dst: str,
        depth: int,
        datafile: str,
        dataname: str,
    ) -> None:
        params = [str(item), "/".join([".."] * depth), datafile, dataname]
        content = Node.node(True, GEN_PATH / "mjs2js.mjs", *params)
        with mkdocs_gen_files.open(
            f"{self._dst}/{dst}/{item_name}/main.js", "w"
        ) as f_example:
            f_example.write(content)

    def _generate_example_md(  # pylint: disable=too-many-arguments,too-many-positional-arguments
        self,
        item: Path,
        item_name: str,
        dst: str,
        depth: int,
        datafile: str,
        dataname: str,
        title: str,
    ) -> None:
        params = [
            str(item),
            str(TEST_DATA_PATH),
            "/".join([".."] * depth),
            datafile,
            dataname,
            title,
        ]
        content = Node.node(True, GEN_PATH / "mjs2md.mjs", *params)
        content = Vizzu.set_version(content)
        content = Markdown.format(content)
        with mkdocs_gen_files.open(
            f"{self._dst}/{dst}/{item_name}.md", "w"
        ) as f_example:
            f_example.write(content)

    def _generate_example(  # pylint: disable=too-many-arguments,too-many-positional-arguments
        self,
        item: Path,
        item_name: str,
        dst: str,
        depth: int,
        datafile: str,
        dataname: str,
        title: str,
    ) -> None:
        self._generate_example_md(
            item, item_name, dst, depth, datafile, dataname, title
        )
        self._generate_example_js(item, item_name, dst, depth, datafile, dataname)
        GenExamples._generate_example_data(datafile)

    def generate(self) -> None:
        # pylint: disable=too-many-locals

        config = MkdocsConfig.load(MKDOCS_PATH / "mkdocs.yml")
        dst = "."
        depth = self._depth
        index = self._create_index(dst, depth, self._name)
        groups = {}
        items = list(self._src.rglob("*.mjs"))
        items.sort(key=lambda f: f.stem)
        for item in items:
            if item in self._blocked:
                continue
            item_name = item.stem
            dst = "."
            depth = self._depth
            sub_index = index
            sub = self._get_sub(item)
            if sub:
                item_name += f"_{sub}"
            if self._merge_subfolders:
                if item_name in self._generated:
                    raise ValueError(f"example already exists {item_name}")
                self._generated.append(item_name)
            else:
                dst = os.path.relpath(item.parent, self._src)
                depth += dst.count("/")
                value = self._dst + "/"
                if dst != ".":
                    depth += 1
                    value += dst + "/"
                group_title, number = GenExamples._find_dict_with_value(
                    config["nav"], value
                )
                if group_title and group_title not in groups:
                    groups[group_title] = {"item": item, "dst": dst, "number": number}
                sub_index = self._create_index(dst, depth, group_title)  # type: ignore
            content = GenExamples._get_content(item)
            datafile = self._get_datafile(item, content)
            dataname = self._get_dataname(item, content)
            title = self._get_title(item, sub)
            self._add_index_item(sub_index, title, item_name)
            self._generate_example(
                item, item_name, dst, depth, datafile, dataname, title
            )
        self._add_index_sub_menus(index, groups)


class GenShowcases(GenExamples):
    def generate(self) -> None:
        dst = "."
        depth = self._depth
        index = self._create_index(dst, depth, self._name)

        items = list(self._src.rglob("main.html"))
        for item in items:
            content = GenExamples._get_content(item)
            content = Vizzu.set_version_showcase(content)
            with mkdocs_gen_files.open(
                self._dst + "/" + os.path.relpath(item, SHOWCASES_PATH), "w"
            ) as fh_js:
                fh_js.write(content)

        items = list(self._src.rglob("*.md"))
        items.sort(key=lambda f: f.stem)
        for item in items:
            content = GenExamples._get_content(item)
            html = markdown.markdown(content)
            h1_titles = re.findall(r"<h1.*?>(.*?)</h1>", html)
            self._add_index_item(index, h1_titles[0], item.stem)


def main() -> None:
    with chdir(REPO_PATH):
        presets = GenExamples(
            "Preset charts",
            PRESET_EXAMPLES_PATH,
            "examples/presets",
        )
        presets.merge_subfolders = True
        presets.generate()

        static = GenExamples(
            "Static charts",
            STATIC_EXAMPLES_PATH,
            "examples/static",
        )
        static.merge_subfolders = True
        static.generate()

        operations = GenExamples(
            "Analytical Operations",
            OPERATION_EXAMPLES_PATH,
            "examples/analytical_operations",
        )
        operations.video_thumbnails = True
        operations.generate()

        real = GenShowcases(
            "Showcases",
            SHOWCASES_PATH,
            "showcases",
        )
        real.video_thumbnails = True
        real.generate()


main()
