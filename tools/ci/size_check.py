# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring

from pathlib import Path
import sys


THRESHOLD = 1.05

workspace_path = Path(__file__).parent
weblib_path = Path.joinpath(workspace_path, "../..", "build/cmake-wasm/weblib")

with open(Path.joinpath(weblib_path, "size.txt"), "r", encoding="utf8") as size_file:
    for line in size_file:
        file, actual_size = line.strip().split()
        with open(
            Path.joinpath(weblib_path, "size_downloaded.txt"), "r", encoding="utf8"
        ) as downloaded_file:
            for d_line in downloaded_file:
                if d_line.startswith(file):
                    downloaded_size = d_line.split()[1]
                    if float(actual_size) > float(downloaded_size) * THRESHOLD:
                        print(f"Error: File {file} size exceeds the threshold.")
                        sys.exit(1)
                    else:
                        print(
                            f"Success: File {file} size comparison passed. "
                            f"Actual: {actual_size}, Downloaded: {downloaded_size}"
                        )
                    break
            else:
                print(
                    f"Warning: Downloaded size not found for file {file}. "
                    f"Skipping comparison. Actual: {actual_size}"
                )
