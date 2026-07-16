#!/usr/bin/env python3
import pathlib
import re
import urllib.request

URL = "https://raw.githubusercontent.com/xenia-project/libxenon/cd535d5948c9696377f719d2ae59a927bc6b8ab5/devkitxenon/examples/xenon/graphics/cube/source/main.c"
OUT = pathlib.Path(__file__).resolve().parents[1] / "platform/xenon/GeneratedCubeShaders.h"


def extract(source: str, name: str) -> str:
    match = re.search(rf"static unsigned char {name}\[\]\s*=\s*\{{(.*?)\n\}};", source, re.S)
    if not match:
        raise RuntimeError(f"Cannot find shader array: {name}")
    return match.group(1).strip()


def main() -> None:
    with urllib.request.urlopen(URL, timeout=30) as response:
        source = response.read().decode("utf-8")

    ps = extract(source, "shader_3d_ps")
    vs = extract(source, "shader_3d_vs")
    OUT.write_text(
        "#pragma once\n"
        "// Generated from the pinned open-source LibXenon cube example.\n"
        "static const unsigned char kEvaCubePixelShader[] = {\n" + ps + "\n};\n\n"
        "static const unsigned char kEvaCubeVertexShader[] = {\n" + vs + "\n};\n",
        encoding="utf-8",
    )
    print(f"Generated {OUT}")


if __name__ == "__main__":
    main()
