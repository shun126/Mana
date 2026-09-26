#!/usr/bin/env python3
"""Check the Mana documentation manuscripts before they are published.

    python documents/tools/check-docs.py

Errors fail the check. Warnings report a language that is behind the default
language, which is expected while a translation is still being written.
Use `--strict` to fail on warnings as well.
"""

from __future__ import annotations

import argparse
import posixpath
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import manadoc


class Report:
    def __init__(self):
        self.errors = []
        self.warnings = []

    def error(self, message):
        self.errors.append(message)

    def warning(self, message):
        self.warnings.append(message)

    def print(self, strict: bool) -> int:
        for message in self.warnings:
            print(f"warning: {message}")
        for message in self.errors:
            print(f"error: {message}")
        failed = bool(self.errors) or (strict and self.warnings)
        print(
            "%d error(s), %d warning(s) — %s"
            % (len(self.errors), len(self.warnings), "FAILED" if failed else "OK")
        )
        return 1 if failed else 0


# --------------------------------------------------------------------------
# Wiki manuscripts
# --------------------------------------------------------------------------


def check_wiki_language(config: manadoc.WikiConfig, language: manadoc.Language, report: Report):
    root = config.source_root(language)
    label = manadoc.repository_relative(root)

    listed = {}
    for page in config.pages:
        for relative in page.files:
            if relative in listed:
                report.error(
                    f"wiki.yml lists {relative} in both '{listed[relative]}' and '{page.key}'"
                )
            else:
                listed[relative] = page.key
            # A missing translation is reported by check_wiki_translations.
            if not (root / relative).is_file() and not language.optional:
                report.error(f"{label}/{relative} is listed in wiki.yml but does not exist")

    for path in sorted(root.rglob("*.md")):
        relative = path.relative_to(root).as_posix()
        if relative not in listed:
            report.error(f"{label}/{relative} exists but no wiki.yml page includes it")

    generator = manadoc.WikiGenerator(
        config, language, on_issue=lambda message: report.error(f"{label}: {message}")
    )

    # Manuscript links and images must resolve in the repository.
    for relative in listed:
        path = root / relative
        if not path.is_file():
            continue
        text = path.read_text(encoding="utf-8")
        for is_image, _, target in manadoc.iter_links(text):
            if manadoc.is_external(target):
                continue
            file_part, _ = manadoc.split_fragment(target)
            if not file_part:
                continue
            repository_path = generator.repository_path(relative, file_part)
            if repository_path is None:
                report.error(f"{label}/{relative}: '{target}' leaves the repository")
                continue
            resolved = manadoc.REPOSITORY_DIR / repository_path
            manuscript = generator.language_relative(repository_path)
            if (
                not resolved.exists()
                and language.optional
                and manuscript in generator.index
                and (config.source_root(config.default_language) / manuscript).is_file()
            ):
                # Not translated yet; the Wiki links the default language instead.
                continue
            if not resolved.exists():
                kind = "image" if is_image else "link target"
                report.error(f"{label}/{relative}: {kind} '{target}' does not exist")
            elif is_image and generator.asset_relative(repository_path) is None:
                report.warning(
                    f"{label}/{relative}: image '{target}' is outside documents/assets, "
                    "so it will not be published with the Wiki"
                )

    # Every page must produce content.
    try:
        for name, text in generator.build_all().items():
            if not text.strip():
                report.error(f"generated page '{name}' is empty")
    except manadoc.ConfigError as error:
        report.error(str(error))


def check_wiki_translations(config: manadoc.WikiConfig, report: Report):
    """Compare each optional language against the default language."""
    default = config.default_language
    default_files = {
        path.relative_to(config.source_root(default)).as_posix()
        for path in config.source_root(default).rglob("*.md")
    }
    for language in config.languages.values():
        if language.code == default.code:
            continue
        root = config.source_root(language)
        if not root.is_dir() or not any(root.rglob("*.md")):
            message = f"language '{language.code}' has no manuscripts yet"
            (report.warning if language.optional else report.error)(message)
            continue
        present = {path.relative_to(root).as_posix() for path in root.rglob("*.md")}
        if language.optional:
            for page in config.pages:
                done = sum((root / relative).is_file() for relative in page.files)
                if done < len(page.files):
                    report.warning(
                        f"{language.code}: page {page.name(language)} is not published yet "
                        f"({done} of {len(page.files)} manuscripts translated)"
                    )
        for relative in sorted(default_files - present):
            message = (
                f"{language.code}: {relative} is not translated "
                f"(present in {default.code})"
            )
            (report.warning if language.optional else report.error)(message)
        for relative in sorted(present - default_files):
            report.error(
                f"{language.code}: {relative} has no counterpart in {default.code}; "
                "both languages must use the same file names"
            )


# --------------------------------------------------------------------------
# Pages manuscripts
# --------------------------------------------------------------------------


def check_pages(config: manadoc.WikiConfig, report: Report):
    config_path = manadoc.PAGES_CONFIG
    if not config_path.is_file():
        report.error(f"{manadoc.repository_relative(manadoc.PAGES_DIR)}/site.yml is missing")
        return
    try:
        site = manadoc.load_yaml(config_path) or {}
    except manadoc.ConfigError as error:
        report.error(str(error))
        return

    languages = site.get("languages") or {}
    default = site.get("default_language")
    if default not in languages:
        report.error(f"site.yml: default_language '{default}' is not configured")
        return

    index = manadoc.PAGES_DIR / default / "index.md"
    if not index.is_file():
        report.error(f"{manadoc.repository_relative(index)} is missing")

    wiki_base = (site.get("wiki") or {}).get("url")
    if not wiki_base:
        report.error("site.yml: the Wiki base URL is not configured")

    required_wiki_pages = {"Getting-Started", "Tutorial", "Language-Reference", "Integration"}
    for code in languages:
        directory = manadoc.PAGES_DIR / code
        if not directory.is_dir() or not any(directory.glob("*.md")):
            report.warning(f"pages: language '{code}' has no manuscripts yet")
            continue

        linked = set()
        for path in sorted(directory.rglob("*.md")):
            relative = manadoc.repository_relative(path)
            text = path.read_text(encoding="utf-8")
            for is_image, _, target in manadoc.iter_links(text):
                if target.startswith("wiki:"):
                    name = target[len("wiki:"):].split("#")[0]
                    linked.add(name)
                    if name and name not in {page.key for page in config.pages}:
                        report.error(f"{relative}: '{target}' is not a page in wiki.yml")
                    continue
                if manadoc.is_external(target):
                    continue
                file_part, _ = manadoc.split_fragment(target)
                if not file_part:
                    continue
                base = posixpath.dirname(manadoc.repository_relative(path))
                resolved = posixpath.normpath(posixpath.join(base, file_part))
                if resolved.startswith(".."):
                    report.error(f"{relative}: '{target}' leaves the repository")
                elif not (manadoc.REPOSITORY_DIR / resolved).exists():
                    kind = "image" if is_image else "link target"
                    report.error(f"{relative}: {kind} '{target}' does not exist")

        missing = required_wiki_pages - linked
        if missing:
            report.error(
                "pages/%s: the site must link to the Wiki pages %s"
                % (code, ", ".join(sorted(missing)))
            )


# --------------------------------------------------------------------------


def check_assets(report: Report):
    common = manadoc.ASSETS_DIR / "common"
    if not common.is_dir():
        report.error(f"{manadoc.repository_relative(manadoc.ASSETS_DIR)}/common is missing")


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--config", default=manadoc.WIKI_CONFIG, type=Path)
    parser.add_argument("--strict", action="store_true", help="treat warnings as errors")
    arguments = parser.parse_args(argv)

    manadoc.configure_output()
    report = Report()
    try:
        config = manadoc.load_wiki_config(arguments.config)
    except manadoc.ConfigError as error:
        print(f"error: {error}")
        return 1

    for language in config.present_languages():
        check_wiki_language(config, language, report)
    check_wiki_translations(config, report)
    check_assets(report)
    check_pages(config, report)
    return report.print(arguments.strict)


if __name__ == "__main__":
    raise SystemExit(main())
