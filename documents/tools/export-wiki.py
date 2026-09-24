#!/usr/bin/env python3
"""Generate the GitHub Wiki pages from the manuscripts in `documents/wiki/`.

    python documents/tools/export-wiki.py --output build/wiki
    python documents/tools/export-wiki.py --language ja --output build/wiki
    python documents/tools/export-wiki.py --language all --output build/wiki

The output directory is a complete Wiki tree. It is generated material and is
never committed to this repository; `publish-wiki.yml` mirrors it into
`Mana.wiki.git`.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import manadoc


def parse_arguments(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument(
        "--language",
        default="default",
        help="language code, 'default' for the default language, or 'all'",
    )
    parser.add_argument(
        "--output",
        default="build/wiki",
        type=Path,
        help="directory to generate into (default: build/wiki)",
    )
    parser.add_argument("--config", default=manadoc.WIKI_CONFIG, type=Path)
    parser.add_argument(
        "--ref",
        default=None,
        help="repository ref used for links that leave the manuscripts",
    )
    parser.add_argument(
        "--keep",
        action="store_true",
        help="add to the output directory instead of emptying it first",
    )
    return parser.parse_args(argv)


def selected_languages(config: manadoc.WikiConfig, request: str):
    """Resolve `--language` into the languages that can actually be generated."""
    if request == "all":
        present = config.present_languages()
        missing = [
            language.code
            for language in config.languages.values()
            if language not in present
        ]
        for code in missing:
            language = config.languages[code]
            if language.optional:
                print(f"note: {code} has no manuscripts yet; skipping", file=sys.stderr)
            else:
                raise manadoc.ConfigError(
                    f"language '{code}' is required but has no manuscripts in "
                    f"{manadoc.repository_relative(config.source_root(language))}"
                )
        if not present:
            raise manadoc.ConfigError("no language has any manuscripts")
        return present

    language = config.default_language if request == "default" else config.language(request)
    root = config.source_root(language)
    if not root.is_dir():
        raise manadoc.ConfigError(
            f"{manadoc.repository_relative(config.source_root(language))} does not exist"
        )
    return [language]


def main(argv=None) -> int:
    manadoc.configure_output()
    arguments = parse_arguments(argv)
    try:
        config = manadoc.load_wiki_config(arguments.config)
        if arguments.ref:
            config.repository_ref = arguments.ref
        languages = selected_languages(config, arguments.language)
    except manadoc.ConfigError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1

    output = Path(arguments.output)
    if arguments.keep:
        output.mkdir(parents=True, exist_ok=True)
    else:
        manadoc.reset_directory(output)

    problems = []
    written = []
    for language in languages:
        generator = manadoc.WikiGenerator(
            config, language, on_issue=lambda message: problems.append(message)
        )
        try:
            pages = generator.build_all()
        except manadoc.ConfigError as error:
            print(f"error: {error}", file=sys.stderr)
            return 1
        for name, text in pages.items():
            if not text.strip():
                print(f"error: generated page '{name}' is empty", file=sys.stderr)
                return 1
            manadoc.write_text(output / (name + ".md"), text)
            written.append(name)
        # A partly translated optional language publishes its finished pages.
        for page in config.pages:
            if page.name(language) not in pages:
                done = sum(
                    (config.source_root(language) / relative).is_file()
                    for relative in page.files
                )
                print(
                    f"note: {page.name(language)} is not published yet "
                    f"({done} of {len(page.files)} manuscripts translated)",
                    file=sys.stderr,
                )

    manadoc.write_text(output / "_Sidebar.md", manadoc.build_sidebar(config, languages))
    written.append("_Sidebar")
    if config.footer:
        manadoc.write_text(output / "_Footer.md", config.footer + "\n")
        written.append("_Footer")

    assets = manadoc.copy_assets(languages, output / manadoc.ASSETS_OUTPUT_NAME)

    for problem in problems:
        print(f"warning: {problem}", file=sys.stderr)

    print(
        "Generated %d pages and %d assets for %s in %s"
        % (
            len(written),
            len(assets),
            ", ".join(language.code for language in languages),
            output,
        )
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
