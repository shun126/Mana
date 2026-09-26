"""Validate a Mana release tag and assemble source distributions."""

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path
from zipfile import ZIP_DEFLATED, ZipFile


ROOT = Path(__file__).resolve().parents[2]
VERSION_FILE = ROOT / "runner/common/Version.json"
GENERATED_FILES = {
    "compiler/Parser.cpp": "generated/Parser.cpp",
    "compiler/Parser.hpp": "generated/Parser.hpp",
    "compiler/Lexer.cpp": "generated/Lexer.cpp",
    "runner/common/Version.h": "runner/common/Version.h",
}
# Repository-only content left out of the compiler archive.
COMPILER_EXCLUDED_DIRECTORIES = ("documents", "examples", "tests")
COMPILER_EXCLUDED_FILES = ("CMakeLists.txt", "CodingConvention.md", "CodingConvention-ja.md")


def validate(tag):
    if not re.fullmatch(r"v(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)", tag):
        raise ValueError(f"Invalid release tag: {tag!r}; expected vMAJOR.MINOR.PATCH")

    version = json.loads(VERSION_FILE.read_text(encoding="utf-8"))
    parts = []
    for key in ("MajorVersion", "MinorVersion", "PatchVersion"):
        value = version[key]
        if type(value) is not int or value < 0:
            raise ValueError(f"{key} must be a non-negative integer")
        parts.append(str(value))
    number = ".".join(parts)
    if tag != f"v{number}":
        raise ValueError(f"Tag {tag} does not match runner/common/Version.json ({number})")

    # The checkout and every package file must come from the pushed tag.
    git = ["git"]
    head = subprocess.check_output(git + ["rev-parse", "HEAD"], cwd=ROOT, text=True).strip()
    tagged = subprocess.check_output(
        git + ["rev-parse", "--verify", f"refs/tags/{tag}^{{commit}}"],
        cwd=ROOT, text=True,
    ).strip()
    if head != tagged:
        raise ValueError(f"Checkout {head} does not match tag {tag} ({tagged})")
    if subprocess.run(git + ["diff", "--quiet", "HEAD", "--"], cwd=ROOT).returncode:
        raise ValueError("Tracked source files differ from the tagged revision")
    print(f"Validated {tag} at {head}")
    return number, git


def tracked_files(git):
    output = subprocess.check_output(git + ["ls-files", "-z"], cwd=ROOT)
    return sorted(Path(name.decode("utf-8")) for name in output.split(b"\0") if name)


def write_package(path, files, generated, prefix):
    path.parent.mkdir(parents=True, exist_ok=True)
    with ZipFile(path, "w", compression=ZIP_DEFLATED, compresslevel=9) as archive:
        for relative in files:
            source = ROOT / relative
            if not source.is_file():
                raise FileNotFoundError(source)
            archive.write(source, f"{prefix}/{relative.as_posix()}")
        for relative, source in generated.items():
            if not source.is_file():
                raise FileNotFoundError(source)
            archive.write(source, f"{prefix}/{relative}")
    print(f"Created {path}")


def package(number, git, build_dir, output_dir):
    generated = {name: build_dir / path for name, path in GENERATED_FILES.items()}
    generated["runner/common/Version.h"] = ROOT / "runner/common/Version.h"
    for path in generated.values():
        if not path.is_file():
            raise FileNotFoundError(f"Build first; generated source is missing: {path}")

    files = tracked_files(git)
    required = {Path(name) for name in ("runner/common/Version.json",
                                        "LICENSE.md", "README.md", "runner/Mana.h")}
    missing = required.difference(files)
    if missing:
        raise ValueError(f"Required tagged files are missing: {sorted(map(str, missing))}")
    compiler_files = [p for p in files if not p.as_posix().startswith(".github/")
                      and p.name not in (".gitignore", ".gitattributes")
                      and p.parts[0] not in COMPILER_EXCLUDED_DIRECTORIES
                      and p.as_posix() not in COMPILER_EXCLUDED_FILES]
    compiler_files = [p for p in compiler_files if p.as_posix() not in GENERATED_FILES]
    compiler_files.sort()

    runtime_files = [p for p in files if p.as_posix().startswith("runner/")]
    runtime_files += [Path("LICENSE.md"), Path("README.md")]
    runtime_files = [p for p in runtime_files if p.as_posix() not in GENERATED_FILES]
    runtime_files.sort()

    write_package(output_dir / f"mana-runtime-{number}-source.zip",
                  runtime_files, {"runner/common/Version.h": generated["runner/common/Version.h"]},
                  f"mana-runtime-{number}")
    write_package(output_dir / f"mana-compiler-{number}-source.zip",
                  compiler_files, generated, f"mana-compiler-{number}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "package"))
    parser.add_argument("tag")
    parser.add_argument("--build-dir", type=Path, default=ROOT / "build")
    parser.add_argument("--output-dir", type=Path, default=ROOT / "dist")
    args = parser.parse_args()
    number, git = validate(args.tag)
    if args.command == "package":
        package(number, git, args.build_dir.resolve(), args.output_dir.resolve())


if __name__ == "__main__":
    try:
        main()
    except (ValueError, KeyError, FileNotFoundError, subprocess.CalledProcessError) as error:
        print(f"Release error: {error}", file=sys.stderr)
        sys.exit(1)
