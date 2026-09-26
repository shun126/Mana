"""Shared helpers for the Mana documentation tools.

`documents/wiki/<language>/` and `documents/pages/<language>/` hold the
manuscripts. The command line tools next to this module turn them into the
GitHub Wiki and the GitHub Pages site; nothing here writes back into the
manuscripts.
"""

from __future__ import annotations

import posixpath
import re
import shutil
import sys
from dataclasses import dataclass, field
from pathlib import Path, PurePosixPath

# --------------------------------------------------------------------------
# Repository layout
# --------------------------------------------------------------------------

TOOLS_DIR = Path(__file__).resolve().parent
DOCUMENT_DIR = TOOLS_DIR.parent
REPOSITORY_DIR = DOCUMENT_DIR.parent

WIKI_DIR = DOCUMENT_DIR / "wiki"
PAGES_DIR = DOCUMENT_DIR / "pages"
ASSETS_DIR = DOCUMENT_DIR / "assets"

WIKI_CONFIG = WIKI_DIR / "wiki.yml"
PAGES_CONFIG = PAGES_DIR / "site.yml"

# Assets are published under this directory of every output tree, so a
# manuscript reference to `documents/assets/ja/diagrams/x.svg` becomes
# `assets/ja/diagrams/x.svg`.
ASSETS_OUTPUT_NAME = "assets"


def configure_output():
    """Keep tool output readable on consoles with a legacy code page."""
    for stream in (sys.stdout, sys.stderr):
        reconfigure = getattr(stream, "reconfigure", None)
        if reconfigure:
            reconfigure(encoding="utf-8", errors="replace")


def repository_relative(path: Path) -> str:
    """Return `path` as a POSIX path relative to the repository root."""
    return PurePosixPath(Path(path).resolve().relative_to(REPOSITORY_DIR).as_posix()).as_posix()


# --------------------------------------------------------------------------
# YAML
# --------------------------------------------------------------------------


def load_yaml(path: Path):
    """Load a configuration file.

    PyYAML is used when it is installed. The fallback understands the subset
    of YAML these configuration files use, so the tools also run on a bare
    Python installation; it raises on anything it does not understand rather
    than guessing.
    """
    return load_yaml_text(Path(path).read_text(encoding="utf-8"), Path(path))


def load_yaml_text(text: str, path: Path):
    """Load configuration text; `path` only names the source in messages."""
    try:
        import yaml  # type: ignore
    except ImportError:
        return _load_simple_yaml(text, Path(path))
    try:
        return yaml.safe_load(text)
    except yaml.YAMLError as error:
        raise ConfigError(f"{path}: {error}") from error


class ConfigError(Exception):
    """A configuration file cannot be used."""


def _strip_comment(line: str) -> str:
    result = []
    quote = None
    previous = ""
    for character in line:
        if quote:
            result.append(character)
            if character == quote:
                quote = None
        elif character in "\"'":
            quote = character
            result.append(character)
        elif character == "#" and (not previous or previous.isspace()):
            break
        else:
            result.append(character)
        previous = character
    return "".join(result).rstrip()


def _parse_scalar(text: str):
    text = text.strip()
    if not text:
        return ""
    if text[0] in "\"'" and text[-1] == text[0] and len(text) >= 2:
        return text[1:-1]
    lowered = text.lower()
    if lowered in ("true", "yes"):
        return True
    if lowered in ("false", "no"):
        return False
    if lowered in ("null", "~"):
        return None
    if re.fullmatch(r"-?\d+", text):
        return int(text)
    return text


_BLOCK_STYLES = ("|", "|-", "|+", ">", ">-", ">+")


def _load_simple_yaml(text: str, path: Path):
    """Parse nested mappings, sequences, and literal or folded block text."""
    raw_lines = text.splitlines()
    entries = []
    for number, raw in enumerate(raw_lines, 1):
        line = _strip_comment(raw)
        if not line.strip():
            continue
        indent = len(line) - len(line.lstrip(" "))
        entries.append((indent, line.strip(), number))

    def parse_block_scalar(number: int, indent: int, style: str):
        """Read the indented lines after a `key: |` or `key: >` line."""
        index = number  # `number` is 1-based, so this is the line after the key.
        collected = []
        while index < len(raw_lines):
            raw = raw_lines[index]
            if raw.strip() and len(raw) - len(raw.lstrip(" ")) <= indent:
                break
            collected.append(raw)
            index += 1
        while collected and not collected[-1].strip():
            collected.pop()
        if not collected:
            return "", index
        margin = min(
            len(line) - len(line.lstrip(" ")) for line in collected if line.strip()
        )
        body = [line[margin:] if line.strip() else "" for line in collected]
        if style.startswith("|"):
            value = "\n".join(body)
        else:
            # Folded: lines of a paragraph join with a space, blank lines break it.
            paragraphs = []
            current = []
            for line in body:
                if line:
                    current.append(line)
                elif current:
                    paragraphs.append(" ".join(current))
                    current = []
            if current:
                paragraphs.append(" ".join(current))
            value = "\n".join(paragraphs)
        if style.endswith("-"):
            return value, index
        return value + "\n", index

    def parse_block(position: int, indent: int):
        if position < len(entries) and entries[position][1].startswith("- "):
            return parse_sequence(position, indent)
        return parse_mapping(position, indent)

    def parse_sequence(position: int, indent: int):
        items = []
        while position < len(entries) and entries[position][0] == indent:
            _, content, number = entries[position]
            if not content.startswith("- "):
                break
            value = content[2:].strip()
            if not value:
                raise ConfigError(f"{path}:{number}: empty list entry is not supported")
            if re.match(r"^[A-Za-z_][\w.\-]*:(\s|$)", value):
                # A mapping written on the dash line; continue it at the
                # column the key starts in.
                entries[position] = (indent + 2, value, number)
                item, position = parse_mapping(position, indent + 2)
                items.append(item)
                continue
            items.append(_parse_scalar(value))
            position += 1
        return items, position

    def parse_mapping(position: int, indent: int):
        mapping = {}
        while position < len(entries) and entries[position][0] == indent:
            _, content, number = entries[position]
            if ":" not in content:
                raise ConfigError(f"{path}:{number}: expected 'key: value'")
            key, _, value = content.partition(":")
            key = key.strip()
            value = value.strip()
            position += 1
            if value in _BLOCK_STYLES:
                mapping[key], consumed = parse_block_scalar(number, indent, value)
                while position < len(entries) and entries[position][2] <= consumed:
                    position += 1
                continue
            if value:
                mapping[key] = _parse_scalar(value)
                continue
            if position < len(entries) and entries[position][0] > indent:
                mapping[key], position = parse_block(position, entries[position][0])
            else:
                mapping[key] = None
        return mapping, position

    if not entries:
        return {}
    value, position = parse_block(0, entries[0][0])
    if position != len(entries):
        raise ConfigError(f"{path}:{entries[position][2]}: unexpected indentation")
    return value


# --------------------------------------------------------------------------
# Markdown scanning
# --------------------------------------------------------------------------

_FENCE_RE = re.compile(r"^ {0,3}(`{3,}|~{3,})")
_HEADING_RE = re.compile(r"^(#{1,6})(\s+)(.*?)\s*$")
_LINK_RE = re.compile(
    r"(!?)\["
    r"((?:[^\[\]]|\[[^\[\]]*\])*)"
    r"\]\("
    r"([^()\s]*(?:\([^()\s]*\))?[^()\s]*)"
    r"(\s+\"[^\"]*\")?"
    r"\)"
)


def code_line_mask(lines):
    """Return one flag per line: True while inside a fenced code block."""
    mask = []
    fence = None
    for line in lines:
        match = _FENCE_RE.match(line)
        if fence is None:
            if match:
                fence = match.group(1)[0] * 3
                mask.append(True)
                continue
            mask.append(False)
        else:
            mask.append(True)
            if match and match.group(1)[0] * 3 == fence:
                fence = None
    return mask


def split_inline_code(line: str):
    """Split a line into `(is_code, text)` runs so code spans stay untouched."""
    runs = []
    position = 0
    for match in re.finditer(r"(`+)(?:.*?)\1", line):
        if match.start() > position:
            runs.append((False, line[position:match.start()]))
        runs.append((True, match.group(0)))
        position = match.end()
    if position < len(line):
        runs.append((False, line[position:]))
    return runs


def document_title(text: str):
    """Return the text of the first level-1 heading outside code blocks."""
    lines = text.splitlines()
    for line, in_code in zip(lines, code_line_mask(lines)):
        if in_code:
            continue
        match = _HEADING_RE.match(line)
        if match and len(match.group(1)) == 1:
            return match.group(3).strip()
    return None


def demote_headings(text: str, levels: int = 1, on_issue=None) -> str:
    """Push every heading outside code blocks down by `levels`."""
    lines = text.splitlines()
    result = []
    for line, in_code in zip(lines, code_line_mask(lines)):
        match = None if in_code else _HEADING_RE.match(line)
        if match:
            depth = len(match.group(1)) + levels
            if depth > 6:
                depth = 6
                if on_issue:
                    on_issue(f"heading '{match.group(3)}' cannot be demoted below level 6")
            line = "#" * depth + match.group(2) + match.group(3)
        result.append(line)
    return "\n".join(result)


def iter_links(text: str):
    """Yield `(is_image, text, target)` for links outside code."""
    lines = text.splitlines()
    for line, in_code in zip(lines, code_line_mask(lines)):
        if in_code:
            continue
        for is_code, run in split_inline_code(line):
            if is_code:
                continue
            for match in _LINK_RE.finditer(run):
                yield bool(match.group(1)), match.group(2), match.group(3)


def replace_links(text: str, rewrite) -> str:
    """Rewrite link targets outside code blocks with `rewrite(is_image, target)`."""
    lines = text.splitlines()
    result = []
    for line, in_code in zip(lines, code_line_mask(lines)):
        if in_code:
            result.append(line)
            continue
        pieces = []
        for is_code, run in split_inline_code(line):
            if is_code:
                pieces.append(run)
                continue

            def substitute(match):
                target = rewrite(bool(match.group(1)), match.group(3))
                if target is None:
                    return match.group(0)
                return "%s[%s](%s%s)" % (
                    match.group(1),
                    match.group(2),
                    target,
                    match.group(4) or "",
                )

            pieces.append(_LINK_RE.sub(substitute, run))
        result.append("".join(pieces))
    return "\n".join(result)


def is_external(target: str) -> bool:
    """True for targets the tools must leave alone."""
    if not target:
        return True
    return bool(re.match(r"^(?:[A-Za-z][A-Za-z0-9+.-]*:|//|#|mailto:)", target))


def split_fragment(target: str):
    path, separator, fragment = target.partition("#")
    return path, (separator + fragment if separator else "")


# --------------------------------------------------------------------------
# Wiki configuration
# --------------------------------------------------------------------------


@dataclass
class Language:
    code: str
    label: str
    source: str
    suffix: str = ""
    default: bool = False
    optional: bool = False
    # Shown on the Wiki's front page in the default language, so readers of
    # this language find their edition. `{page}` becomes the page to open.
    notice: str = ""


@dataclass
class WikiPage:
    key: str
    titles: dict
    files: list
    mode: str = "combined"

    def title(self, language: Language, fallback: str) -> str:
        return self.titles.get(language.code) or self.titles.get(fallback) or self.key

    def name(self, language: Language) -> str:
        """The GitHub Wiki page name, such as `Tutorial` or `Tutorial-en`."""
        return self.key + language.suffix


@dataclass
class WikiConfig:
    path: Path
    languages: dict = field(default_factory=dict)
    pages: list = field(default_factory=list)
    toc_titles: dict = field(default_factory=dict)
    footer: str = ""
    repository_url: str = ""
    repository_ref: str = "master"
    document_root: str = "documents"

    @property
    def default_language(self) -> Language:
        for language in self.languages.values():
            if language.default:
                return language
        return next(iter(self.languages.values()))

    @property
    def front_language(self) -> Language:
        """The language whose pages carry no suffix, so its Home is where the Wiki opens.

        This can differ from the default language, which is the original the
        others are translated from.
        """
        for language in self.languages.values():
            if not language.suffix:
                return language
        return self.default_language

    def language(self, code: str) -> Language:
        if code not in self.languages:
            known = ", ".join(sorted(self.languages))
            raise ConfigError(f"unknown language '{code}'; wiki.yml defines {known}")
        return self.languages[code]

    def source_root(self, language: Language) -> Path:
        return WIKI_DIR / language.source

    def present_languages(self):
        """Languages whose manuscript directory holds at least one page."""
        found = []
        for language in self.languages.values():
            root = self.source_root(language)
            if root.is_dir() and any(root.rglob("*.md")):
                found.append(language)
        return found

    def is_complete(self, page: "WikiPage", language: Language) -> bool:
        """True when every manuscript of `page` exists in `language`."""
        root = self.source_root(language)
        return all((root / relative).is_file() for relative in page.files)

    def published_pages(self, language: Language):
        """The pages `language` publishes, in wiki.yml order.

        An optional language is translated a page at a time: a page appears
        once all its manuscripts exist, and links to the others fall back to
        the default language. Any other language must provide every page.
        """
        if not language.optional:
            return list(self.pages)
        return [page for page in self.pages if self.is_complete(page, language)]

    def page_name_for(self, key: str, code: str) -> str:
        """The Wiki page to link for `key` from content written in `code`.

        The page in that language when it is published there, otherwise the
        default language's page, so a link never points at a missing page.
        """
        page = next((page for page in self.pages if page.key == key), None)
        if page is None:
            known = ", ".join(page.key for page in self.pages)
            raise ConfigError(f"'{key}' is not a Wiki page; wiki.yml defines {known}")
        language = self.languages.get(code)
        if language is not None and self.is_complete(page, language):
            return page.name(language)
        return page.name(self.default_language)

    def toc_title(self, language: Language) -> str:
        return self.toc_titles.get(language.code) or self.toc_titles.get(
            self.default_language.code, "Contents"
        )

    def blob_url(self, repository_path: str) -> str:
        return "%s/blob/%s/%s" % (
            self.repository_url.rstrip("/"),
            self.repository_ref,
            repository_path,
        )


def load_wiki_config(path: Path = WIKI_CONFIG) -> WikiConfig:
    path = Path(path)
    data = load_yaml(path) or {}
    config = WikiConfig(path=path)

    languages = data.get("languages") or {}
    if not languages:
        raise ConfigError(f"{path}: no languages are defined")
    for code, entry in languages.items():
        entry = entry or {}
        config.languages[code] = Language(
            code=code,
            label=entry.get("label", code),
            source=entry.get("source", code),
            suffix=entry.get("suffix") or "",
            default=bool(entry.get("default")),
            optional=bool(entry.get("optional")),
            notice=entry.get("notice") or "",
        )

    pages = data.get("pages") or {}
    if not pages:
        raise ConfigError(f"{path}: no pages are defined")
    for key, entry in pages.items():
        entry = entry or {}
        files = entry.get("files") or []
        if not files:
            raise ConfigError(f"{path}: page '{key}' lists no files")
        mode = entry.get("mode", "combined")
        if mode not in ("combined", "single"):
            raise ConfigError(f"{path}: page '{key}' has unknown mode '{mode}'")
        titles = entry.get("title") or {}
        if isinstance(titles, str):
            titles = {config.default_language.code: titles}
        config.pages.append(WikiPage(key=key, titles=titles, files=list(files), mode=mode))

    config.toc_titles = data.get("toc") or {}
    config.footer = (data.get("footer") or "").strip()
    repository = data.get("repository") or {}
    config.repository_url = repository.get("url", "")
    config.repository_ref = repository.get("ref", "master")
    config.document_root = repository.get("document_root", "documents")
    return config


# --------------------------------------------------------------------------
# Wiki generation
# --------------------------------------------------------------------------


def is_overview(relative_path: str) -> bool:
    """True for a section overview, the file that introduces a directory."""
    return PurePosixPath(relative_path).stem.lower() in ("readme", "index")


def anchor_for(relative_path: str) -> str:
    """Return the explicit anchor of a manuscript.

    The anchor comes from the file name so it is identical in every language
    and stable no matter how GitHub renders the heading text. A section
    overview takes the name of its directory.
    """
    path = PurePosixPath(relative_path)
    if is_overview(relative_path):
        return path.parent.name or "home"
    return path.stem


@dataclass
class Section:
    """One manuscript inside a generated page."""

    relative_path: str
    anchor: str
    title: str
    body: str


class WikiGenerator:
    """Join the manuscripts of one language into GitHub Wiki pages."""

    def __init__(self, config: WikiConfig, language: Language, on_issue=None):
        self.config = config
        self.language = language
        self.root = config.source_root(language)
        self.on_issue = on_issue or (lambda message: None)
        self.default_code = config.default_language.code
        # Manuscript path -> (page, anchor), used to turn file links into
        # links to the joined pages.
        self.index = {}
        for page in config.pages:
            for relative in page.files:
                self.index[relative] = (page, anchor_for(relative))
        self.published = {page.key for page in config.published_pages(language)}
        self.used_assets = set()

    # -- helpers ---------------------------------------------------------

    def manuscript(self, relative: str) -> Path:
        return self.root / relative

    def counterpart(self, page: WikiPage):
        """The same page in the other languages, when those exist."""
        links = []
        for language in self.config.languages.values():
            if language.code == self.language.code:
                continue
            if not self.config.source_root(language).is_dir():
                continue
            if not self.config.is_complete(page, language):
                continue
            links.append((language, page.name(language)))
        return links

    def repository_path(self, relative_source: str, target: str):
        """Resolve a link target to a repository path, or None if it leaves it."""
        language_root = "%s/wiki/%s" % (self.config.document_root, self.root.name)
        base = posixpath.dirname(posixpath.join(language_root, relative_source))
        resolved = posixpath.normpath(posixpath.join(base, target))
        if resolved.startswith(".."):
            return None
        return resolved

    def language_relative(self, repository_path: str):
        """Return the manuscript path inside this language, or None."""
        prefix = "%s/wiki/%s/" % (self.config.document_root, self.root.name)
        if repository_path.startswith(prefix):
            return repository_path[len(prefix):]
        return None

    def asset_relative(self, repository_path: str):
        prefix = "%s/assets/" % self.config.document_root
        if repository_path.startswith(prefix):
            return repository_path[len(prefix):]
        return None

    # -- link rewriting --------------------------------------------------

    def rewrite_target(self, relative_source: str, is_image: bool, target: str):
        if is_external(target):
            return None
        path, fragment = split_fragment(target)
        if not path:
            return None

        repository_path = self.repository_path(relative_source, path)
        if repository_path is None:
            self.on_issue(f"{relative_source}: link '{target}' leaves the repository")
            return None

        asset = self.asset_relative(repository_path)
        if asset is not None:
            self.used_assets.add(asset)
            return "%s/%s%s" % (ASSETS_OUTPUT_NAME, asset, fragment)

        manuscript = self.language_relative(repository_path)
        if manuscript is not None:
            entry = self.index.get(manuscript)
            if entry is None:
                self.on_issue(
                    f"{relative_source}: '{target}' is not listed in wiki.yml, "
                    "so it cannot be linked"
                )
                return None
            page, anchor = entry
            destination = fragment or "#" + anchor
            current = self.index.get(relative_source)
            if current and current[0].key == page.key:
                return destination
            # Anchors are the same in every language, so an untranslated page
            # can be linked in the default language at the same section.
            language = (
                self.language if page.key in self.published else self.config.default_language
            )
            if not fragment and manuscript == page.files[0] and is_overview(manuscript):
                # The section overview opens its page, so name the page itself.
                return page.name(language)
            return page.name(language) + destination

        # Anything else in the repository, such as the runnable examples.
        return self.config.blob_url(repository_path) + fragment

    def rewrite(self, relative_source: str, text: str) -> str:
        return replace_links(
            text,
            lambda is_image, target: self.rewrite_target(relative_source, is_image, target),
        )

    # -- page assembly ---------------------------------------------------

    def read_section(self, relative: str) -> Section:
        path = self.manuscript(relative)
        if not path.is_file():
            raise ConfigError(f"{repository_relative(self.root)}/{relative} is missing")
        text = path.read_text(encoding="utf-8")
        title = document_title(text) or PurePosixPath(relative).stem
        body = self.rewrite(relative, text)
        return Section(relative, anchor_for(relative), title, body)

    def language_links(self, page: WikiPage):
        """A bar naming every language this page exists in, the current one in bold."""
        others = {language.code: name for language, name in self.counterpart(page)}
        if not others:
            return ""
        items = []
        for language in self.config.languages.values():
            if language.code == self.language.code:
                items.append("**%s**" % language.label)
            elif language.code in others:
                items.append("[%s](%s)" % (language.label, others[language.code]))
        return "🌐 " + " · ".join(items)

    def notices(self, page: WikiPage):
        """On the page the Wiki opens on, point readers of other languages to their edition."""
        if self.language.code != self.config.front_language.code or page is not self.config.pages[0]:
            return []
        return [
            "> " + language.notice.replace("{page}", name)
            for language, name in self.counterpart(page)
            if language.notice
        ]

    def build_page(self, page: WikiPage) -> str:
        sections = [self.read_section(relative) for relative in page.files]
        header = "\n\n".join(
            part for part in [self.language_links(page)] + self.notices(page) if part
        )

        if page.mode == "single":
            body = sections[0].body
            if header:
                body = _insert_after_title(body, header)
            return body.rstrip() + "\n"

        lines = ["# " + page.title(self.language, self.default_code), ""]
        if header:
            lines += [header, ""]
        lines += ["## " + self.config.toc_title(self.language), ""]
        for section in sections:
            lines.append("- [%s](#%s)" % (section.title, section.anchor))
        lines.append("")
        for section in sections:
            demoted = demote_headings(
                section.body,
                on_issue=lambda message, path=section.relative_path: self.on_issue(
                    f"{path}: {message}"
                ),
            )
            lines.append('<a id="%s"></a>' % section.anchor)
            lines.append("")
            lines.append(demoted.strip())
            lines.append("")
        return "\n".join(lines).rstrip() + "\n"

    def build_all(self):
        """Build the pages this language publishes."""
        return {
            page.name(self.language): self.build_page(page)
            for page in self.config.published_pages(self.language)
        }


def _insert_after_title(text: str, extra: str) -> str:
    lines = text.splitlines()
    for position, (line, in_code) in enumerate(zip(lines, code_line_mask(lines))):
        match = None if in_code else _HEADING_RE.match(line)
        if match and len(match.group(1)) == 1:
            lines[position + 1:position + 1] = ["", extra]
            return "\n".join(lines)
    return extra + "\n\n" + text


def build_sidebar(config: WikiConfig, languages) -> str:
    """Build `_Sidebar.md` listing the pages each language publishes."""
    entries = [
        (language, config.published_pages(language))
        for language in languages
    ]
    entries = [(language, pages) for language, pages in entries if pages]
    lines = ["# Mana", ""]
    show_headings = len(entries) > 1
    for language, pages in entries:
        if show_headings:
            lines += ["## " + language.label, ""]
        for page in pages:
            title = page.title(language, config.default_language.code)
            lines.append("- [%s](%s)" % (title, page.name(language)))
        lines.append("")
    return "\n".join(lines).rstrip() + "\n"


# --------------------------------------------------------------------------
# Assets
# --------------------------------------------------------------------------


def asset_sources(languages):
    """Asset directories to publish: shared material plus each language."""
    directories = []
    common = ASSETS_DIR / "common"
    if common.is_dir():
        directories.append(common)
    for language in languages:
        directory = ASSETS_DIR / language.source
        if directory.is_dir():
            directories.append(directory)
    return directories


def copy_assets(languages, destination: Path):
    """Copy the shared and per-language assets into `destination`."""
    copied = []
    for source in asset_sources(languages):
        for path in sorted(source.rglob("*")):
            if not path.is_file() or path.name.startswith("."):
                continue
            relative = path.relative_to(ASSETS_DIR)
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(path, target)
            copied.append(relative.as_posix())
    return copied


def reset_directory(path: Path):
    """Empty an output directory without following it outside the build tree."""
    path = Path(path)
    if path.exists():
        shutil.rmtree(path)
    path.mkdir(parents=True, exist_ok=True)
    return path


def write_text(path: Path, text: str):
    path.parent.mkdir(parents=True, exist_ok=True)
    # Generated files always use LF so the Wiki repository stays stable.
    path.write_text(text, encoding="utf-8", newline="\n")
