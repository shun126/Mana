#!/usr/bin/env python3
"""Build the Mana official website from the manuscripts in `document/pages/`.

    python document/tools/build-pages.py --output build/pages
    python document/tools/build-pages.py --language ja --output build/pages

The site introduces Mana and sends readers to the Wiki for the manual. The
output directory is generated material and is never committed; the GitHub
Pages workflow uploads it as a deployment artifact.
"""

from __future__ import annotations

import argparse
import html
import posixpath
import re
import shutil
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import manadoc

# --------------------------------------------------------------------------
# Markdown
# --------------------------------------------------------------------------

_INLINE_CODE_RE = re.compile(r"`([^`]+)`")
_STRONG_RE = re.compile(r"\*\*([^*]+)\*\*")
_EMPHASIS_RE = re.compile(r"(?<![*\w])\*([^*]+)\*(?!\*)")
_LINK_RE = re.compile(r"(!?)\[((?:[^\[\]]|\[[^\[\]]*\])*)\]\(([^()\s]+)\)")
_HEADING_RE = re.compile(r"^(#{1,6})\s+(.*?)\s*$")
_FENCE_RE = re.compile(r"^ {0,3}(`{3,}|~{3,})\s*(\S*)\s*$")
_LIST_RE = re.compile(r"^\s*[-*]\s+(.*)$")
_ORDERED_RE = re.compile(r"^\s*\d+\.\s+(.*)$")


def render_inline(text: str, resolve) -> str:
    """Render one line of Markdown, keeping code spans literal."""
    pieces = []
    position = 0
    for match in _INLINE_CODE_RE.finditer(text):
        pieces.append((False, text[position:match.start()]))
        pieces.append((True, match.group(1)))
        position = match.end()
    pieces.append((False, text[position:]))

    rendered = []
    for is_code, piece in pieces:
        if is_code:
            rendered.append("<code>%s</code>" % html.escape(piece))
            continue

        placeholders = []

        def keep(markup):
            placeholders.append(markup)
            return "\x00%d\x00" % (len(placeholders) - 1)

        def link(match):
            target = resolve(bool(match.group(1)), match.group(3))
            label = match.group(2)
            if match.group(1):
                return keep(
                    '<img src="%s" alt="%s">'
                    % (html.escape(target, quote=True), html.escape(label, quote=True))
                )
            return keep('<a href="%s">' % html.escape(target, quote=True)) + label + keep("</a>")

        piece = _LINK_RE.sub(link, piece)
        piece = html.escape(piece)
        piece = _STRONG_RE.sub(r"<strong>\1</strong>", piece)
        piece = _EMPHASIS_RE.sub(r"<em>\1</em>", piece)
        piece = re.sub(r"\x00(\d+)\x00", lambda m: placeholders[int(m.group(1))], piece)
        rendered.append(piece)
    return "".join(rendered)


def render_markdown(text: str, resolve, section_level: int = 2) -> str:
    """Render a manuscript body. Headings at `section_level` open a section."""
    lines = text.splitlines()
    out = []
    open_section = False
    position = 0

    def close_section():
        nonlocal open_section
        if open_section:
            out.append("</section>")
            open_section = False

    while position < len(lines):
        line = lines[position]

        fence = _FENCE_RE.match(line)
        if fence:
            marker, language = fence.group(1), fence.group(2)
            position += 1
            body = []
            while position < len(lines) and not (
                _FENCE_RE.match(lines[position])
                and _FENCE_RE.match(lines[position]).group(1)[0] == marker[0]
            ):
                body.append(lines[position])
                position += 1
            position += 1
            attribute = ' class="language-%s"' % html.escape(language, quote=True) if language else ""
            out.append(
                "<pre><code%s>%s</code></pre>"
                % (attribute, html.escape("\n".join(body)))
            )
            continue

        heading = _HEADING_RE.match(line)
        if heading:
            level = len(heading.group(1))
            if level == section_level:
                close_section()
                out.append("<section>")
                open_section = True
            out.append(
                "<h%d>%s</h%d>" % (level, render_inline(heading.group(2), resolve), level)
            )
            position += 1
            continue

        if _LIST_RE.match(line) or _ORDERED_RE.match(line):
            ordered = bool(_ORDERED_RE.match(line))
            pattern = _ORDERED_RE if ordered else _LIST_RE
            items = []
            while position < len(lines) and pattern.match(lines[position]):
                items.append(pattern.match(lines[position]).group(1))
                position += 1
            tag = "ol" if ordered else "ul"
            out.append("<%s>" % tag)
            out += ["<li>%s</li>" % render_inline(item, resolve) for item in items]
            out.append("</%s>" % tag)
            continue

        if line.startswith("|") and position + 1 < len(lines) and set(
            lines[position + 1].replace("|", "").replace(" ", "")
        ) <= set("-:") and lines[position + 1].strip():
            rows = []
            while position < len(lines) and lines[position].startswith("|"):
                rows.append([cell.strip() for cell in lines[position].strip("|").split("|")])
                position += 1
            header, _, body = rows[0], rows[1], rows[2:]
            out.append("<table><thead><tr>")
            out += ["<th>%s</th>" % render_inline(cell, resolve) for cell in header]
            out.append("</tr></thead><tbody>")
            for row in body:
                out.append("<tr>")
                out += ["<td>%s</td>" % render_inline(cell, resolve) for cell in row]
                out.append("</tr>")
            out.append("</tbody></table>")
            continue

        if not line.strip():
            position += 1
            continue

        paragraph = []
        while position < len(lines) and lines[position].strip():
            if _HEADING_RE.match(lines[position]) or _FENCE_RE.match(lines[position]):
                break
            if _LIST_RE.match(lines[position]) or _ORDERED_RE.match(lines[position]):
                break
            paragraph.append(lines[position].strip())
            position += 1
        if paragraph:
            rendered = render_inline(" ".join(paragraph), resolve)
            if rendered.startswith("<img "):
                out.append(rendered)
            else:
                out.append("<p>%s</p>" % rendered)

    close_section()
    return "\n".join(out)


def split_front_matter(text: str, path: Path):
    if not text.startswith("---"):
        return {}, text
    end = text.find("\n---", 3)
    if end == -1:
        raise manadoc.ConfigError(f"{path}: the front matter is not closed")
    header = text[text.index("\n") + 1:end]
    body = text[end + len("\n---"):].lstrip("\n")
    data = manadoc.load_yaml_text(header, path) or {}
    return data, body


# --------------------------------------------------------------------------
# Site
# --------------------------------------------------------------------------


class SiteBuilder:
    def __init__(self, site, output: Path, ref: str):
        self.site = site
        self.output = output
        self.ref = ref
        self.wiki = (site.get("wiki") or {}).get("url", "").rstrip("/")
        self.repository = (site.get("repository") or {}).get("url", "").rstrip("/")
        theme = site.get("theme") or {}
        self.layout_path = manadoc.PAGES_DIR / theme.get("layout", "theme/base.html")
        self.stylesheet_path = manadoc.PAGES_DIR / theme.get("stylesheet", "theme/site.css")
        self.layout = self.layout_path.read_text(encoding="utf-8")
        # Wiki links follow the reader's language once that Wiki page exists.
        self.wiki_config = manadoc.load_wiki_config()
        self.problems = []

    # -- link resolution -------------------------------------------------

    def wiki_url(self, target: str, code: str) -> str:
        """Turn `Tutorial` or `Tutorial#anchor` into a Wiki URL for `code`."""
        name, fragment = manadoc.split_fragment(target)
        if not name:
            return self.wiki + fragment
        try:
            name = self.wiki_config.page_name_for(name, code)
        except manadoc.ConfigError as error:
            self.problems.append(str(error))
        return "%s/%s%s" % (self.wiki, name, fragment)

    def resolver(self, source: Path, depth: int, code: str):
        """Resolve one manuscript's link targets against the output tree."""
        up = "../" * depth

        def resolve(is_image, target):
            if target.startswith("wiki:"):
                return self.wiki_url(target[len("wiki:"):], code)
            if manadoc.is_external(target):
                return target
            path, fragment = manadoc.split_fragment(target)
            base = posixpath.dirname(manadoc.repository_relative(source))
            resolved = posixpath.normpath(posixpath.join(base, path))
            if resolved.startswith(".."):
                self.problems.append(f"{source}: '{target}' leaves the repository")
                return target
            asset = self.asset_relative(resolved)
            if asset is not None:
                return up + manadoc.ASSETS_OUTPUT_NAME + "/" + asset + fragment
            return "%s/blob/%s/%s%s" % (self.repository, self.ref, resolved, fragment)

        return resolve

    @staticmethod
    def asset_relative(repository_path: str):
        prefix = "document/assets/"
        if repository_path.startswith(prefix):
            return repository_path[len(prefix):]
        return None

    # -- rendering -------------------------------------------------------

    def language_links(self, current: str, languages):
        if len(languages) < 2:
            return ""
        links = [
            '<a href="../%s/">%s</a>' % (code, html.escape(self.site["languages"][code].get("label", code)))
            for code in languages
            if code != current
        ]
        return '<span class="language-links">|</span> ' + " ".join(links)

    def render_actions(self, actions, resolve):
        rendered = []
        for action in actions or []:
            style = action.get("style", "secondary")
            rendered.append(
                '<a class="button button-%s" href="%s">%s</a>'
                % (
                    html.escape(style, quote=True),
                    html.escape(resolve(False, action["href"]), quote=True),
                    html.escape(action.get("label", "")),
                )
            )
        return "\n    ".join(rendered)

    def fill(self, values, code: str):
        def substitute(match):
            key = match.group(1)
            if key.startswith("wiki:"):
                return html.escape(self.wiki_url(key[len("wiki:"):], code), quote=True)
            if key not in values:
                self.problems.append(f"{self.layout_path}: unknown placeholder '{key}'")
                return match.group(0)
            return values[key]

        return re.sub(r"\{\{\s*(wiki:[\w#-]*|\w+)\s*\}\}", substitute, self.layout)

    def build_language(self, code: str, languages):
        directory = manadoc.PAGES_DIR / code
        settings = self.site["languages"][code]
        built = []
        for source in sorted(directory.rglob("*.md")):
            relative = source.relative_to(directory)
            depth = len(relative.parts)  # the language directory plus any nesting
            resolve = self.resolver(source, depth, code)
            front, body = split_front_matter(source.read_text(encoding="utf-8"), source)
            title = front.get("title", self.site.get("title", "Mana"))
            site_title = self.site.get("title", "Mana")
            page_title = title if relative.name == "index.md" else f"{title} — {site_title}"
            values = {
                "html_lang": settings.get("html_lang", code),
                "site_title": html.escape(site_title),
                "page_title": html.escape(page_title),
                "title": html.escape(title),
                "tagline": html.escape(front.get("tagline", self.site.get("tagline", ""))),
                "lead": render_inline(front.get("lead", "").strip(), resolve),
                "description": html.escape(front.get("description", ""), quote=True),
                "actions": self.render_actions(front.get("actions"), resolve),
                "content": render_markdown(body, resolve),
                "language_links": self.language_links(code, languages),
                "assets": "../" * depth + manadoc.ASSETS_OUTPUT_NAME,
                "stylesheet": "../" * depth + "theme/site.css",
                "wiki": self.wiki,
                "repository": self.repository,
                "ref": self.ref,
            }
            target = self.output / code / relative.with_suffix(".html")
            manadoc.write_text(target, self.fill(values, code))
            built.append(target)
        return built

    def build_root(self, languages):
        """The entry page: one language redirects, several offer a choice."""
        default = self.site["default_language"]
        if len(languages) < 2:
            manadoc.write_text(
                self.output / "index.html",
                '<!DOCTYPE html>\n<html lang="%s">\n<head>\n<meta charset="utf-8">\n'
                '<meta http-equiv="refresh" content="0; url=./%s/">\n'
                '<link rel="canonical" href="./%s/">\n<title>%s</title>\n</head>\n'
                '<body><p><a href="./%s/">%s</a></p></body>\n</html>\n'
                % (
                    self.site["languages"][default].get("html_lang", default),
                    default,
                    default,
                    html.escape(self.site.get("title", "Mana")),
                    default,
                    html.escape(self.site.get("title", "Mana")),
                ),
            )
            return

        buttons = "\n    ".join(
            '<a class="button button-%s" href="./%s/">%s</a>'
            % (
                "primary" if code == default else "secondary",
                code,
                html.escape(self.site["languages"][code].get("label", code)),
            )
            for code in languages
        )
        manadoc.write_text(
            self.output / "index.html",
            '<!DOCTYPE html>\n<html lang="en">\n<head>\n<meta charset="utf-8">\n'
            '<meta name="viewport" content="width=device-width, initial-scale=1">\n'
            "<title>%s</title>\n"
            '<link rel="stylesheet" href="theme/site.css">\n</head>\n<body>\n'
            '<div class="chooser">\n  <img src="%s/common/logo_midium.png" alt="%s" width="160" height="160">\n'
            '  <p class="tagline">%s</p>\n  <p class="actions">\n    %s\n  </p>\n</div>\n'
            "</body>\n</html>\n"
            % (
                html.escape(self.site.get("title", "Mana")),
                manadoc.ASSETS_OUTPUT_NAME,
                html.escape(self.site.get("title", "Mana")),
                html.escape(self.site.get("tagline", "")),
                buttons,
            ),
        )


def available_languages(site, request: str):
    configured = site.get("languages") or {}
    default = site.get("default_language")
    if request not in ("default", "all") and request not in configured:
        raise manadoc.ConfigError(f"unknown language '{request}'")

    codes = []
    for code, settings in configured.items():
        directory = manadoc.PAGES_DIR / code
        if directory.is_dir() and any(directory.glob("*.md")):
            codes.append(code)
        elif not (settings or {}).get("optional") and code == default:
            raise manadoc.ConfigError(f"{manadoc.repository_relative(directory)} has no manuscripts")

    if request == "all":
        if not codes:
            raise manadoc.ConfigError("no language has any manuscripts")
        return codes
    wanted = default if request == "default" else request
    if wanted not in codes:
        raise manadoc.ConfigError(
            f"{manadoc.repository_relative(manadoc.PAGES_DIR / wanted)} has no manuscripts"
        )
    return [wanted]


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--language", default="all", help="language code, 'default', or 'all'")
    parser.add_argument("--output", default="build/pages", type=Path)
    parser.add_argument("--config", default=manadoc.PAGES_CONFIG, type=Path)
    parser.add_argument("--ref", default="master", help="repository ref used for source links")
    manadoc.configure_output()
    arguments = parser.parse_args(argv)

    try:
        site = manadoc.load_yaml(arguments.config) or {}
        languages = available_languages(site, arguments.language)
    except manadoc.ConfigError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1

    output = manadoc.reset_directory(Path(arguments.output))
    builder = SiteBuilder(site, output, arguments.ref)

    built = []
    for code in languages:
        built += builder.build_language(code, languages)
    builder.build_root(languages)

    stylesheet = output / "theme" / "site.css"
    stylesheet.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(builder.stylesheet_path, stylesheet)
    assets = manadoc.copy_assets(
        [manadoc.Language(code=code, label=code, source=code) for code in languages],
        output / manadoc.ASSETS_OUTPUT_NAME,
    )
    # GitHub Pages must serve the generated files as they are.
    manadoc.write_text(output / ".nojekyll", "")

    for problem in builder.problems:
        print(f"warning: {problem}", file=sys.stderr)

    print(
        "Generated %d pages and %d assets for %s in %s"
        % (len(built), len(assets), ", ".join(languages), output)
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
