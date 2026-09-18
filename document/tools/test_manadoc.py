"""Tests for the Mana documentation tools.

    python -m unittest discover -s document/tools
"""

from __future__ import annotations

import sys
import tempfile
import textwrap
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import manadoc

WIKI_YML = textwrap.dedent(
    """\
    languages:
      ja:
        default: true
        label: 日本語
        source: ja
        suffix: ""
      en:
        label: English
        source: en
        suffix: "-en"
        optional: true

    pages:
      Home:
        title:
          ja: Home
        mode: single
        files:
          - Home.md
      Tutorial:
        title:
          ja: Tutorial
          en: Tutorial
        files:
          - tutorial/README.md
          - tutorial/tutorial-request.md
      Language-Reference:
        title:
          ja: Language Reference
          en: Language Reference
        files:
          - reference/reference-request.md

    toc:
      ja: 目次
      en: Contents

    repository:
      url: https://example.invalid/owner/repo
      ref: master
      document_root: document
    """
)


class DocumentTree:
    """A throwaway repository laid out like the real one."""

    def __init__(self, directory: Path):
        self.root = directory
        self.document = directory / "document"
        (self.document / "wiki").mkdir(parents=True)
        (self.document / "assets" / "common").mkdir(parents=True)
        (self.document / "assets" / "ja" / "diagrams").mkdir(parents=True)
        (self.document / "pages").mkdir(parents=True)
        self.write("document/wiki/wiki.yml", WIKI_YML)
        self.write("document/assets/common/logo_small.png", "png")
        self.write("document/assets/ja/diagrams/request-flow.svg", "<svg/>")
        self.write("examples/tutorial/03-request.mn", "actor Event {}")

    def write(self, relative: str, text: str):
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        return path

    def apply(self, test: unittest.TestCase):
        """Point the module at this tree for the duration of one test."""
        for name, value in {
            "REPOSITORY_DIR": self.root,
            "DOCUMENT_DIR": self.document,
            "WIKI_DIR": self.document / "wiki",
            "PAGES_DIR": self.document / "pages",
            "ASSETS_DIR": self.document / "assets",
            "WIKI_CONFIG": self.document / "wiki" / "wiki.yml",
            "PAGES_CONFIG": self.document / "pages" / "site.yml",
        }.items():
            original = getattr(manadoc, name)
            setattr(manadoc, name, value)
            test.addCleanup(setattr, manadoc, name, original)


JAPANESE_PAGES = {
    "wiki/ja/Home.md": """\
# Mana ドキュメント

[チュートリアル](./tutorial/README.md)から始めます。
""",
    "wiki/ja/tutorial/README.md": """\
# Mana チュートリアル

1. [Action を Request する](./tutorial-request.md)
""",
    "wiki/ja/tutorial/tutorial-request.md": """\
# Action を Request する

![Request の流れ](../../../assets/ja/diagrams/request-flow.svg)

[構文](../reference/reference-request.md)と
[完成コード](../../../../examples/tutorial/03-request.mn)と
[GitHub](https://github.com/shun126/Mana)を参照します。

## 使い方

コードの見出しは変えません。

```mana
# not a heading
actor Event {}
```
""",
    "wiki/ja/reference/reference-request.md": """\
# request

[チュートリアル](../tutorial/tutorial-request.md)を参照します。
""",
}


class MarkdownTest(unittest.TestCase):
    def test_code_blocks_are_recognised(self):
        lines = ["text", "```", "# inside", "```", "# outside"]
        self.assertEqual(
            manadoc.code_line_mask(lines), [False, True, True, True, False]
        )

    def test_headings_are_demoted_outside_code(self):
        text = "# Title\n\n```\n# comment\n```\n\n## Section\n"
        demoted = manadoc.demote_headings(text)
        self.assertIn("## Title", demoted)
        self.assertIn("\n# comment\n", demoted)
        self.assertIn("### Section", demoted)

    def test_level_six_headings_are_reported(self):
        problems = []
        manadoc.demote_headings("###### Deep\n", on_issue=problems.append)
        self.assertEqual(len(problems), 1)

    def test_inline_code_is_left_alone(self):
        text = "See `[not a link](x.md)` and [a link](x.md)."
        found = [target for _, _, target in manadoc.iter_links(text)]
        self.assertEqual(found, ["x.md"])

    def test_document_title(self):
        self.assertEqual(manadoc.document_title("# Actor と Action\n\nbody"), "Actor と Action")


class AnchorTest(unittest.TestCase):
    def test_file_name_becomes_the_anchor(self):
        self.assertEqual(manadoc.anchor_for("tutorial/tutorial-request.md"), "tutorial-request")

    def test_section_overview_uses_its_directory(self):
        self.assertEqual(manadoc.anchor_for("tutorial/README.md"), "tutorial")

    def test_root_overview(self):
        self.assertEqual(manadoc.anchor_for("README.md"), "home")


class ConfigTest(unittest.TestCase):
    def test_fallback_parser_matches_pyyaml(self):
        try:
            import yaml
        except ImportError:
            self.skipTest("PyYAML is not installed")
        self.assertEqual(
            manadoc._load_simple_yaml(WIKI_YML, Path("wiki.yml")), yaml.safe_load(WIKI_YML)
        )

    def test_block_scalars_match_pyyaml(self):
        try:
            import yaml
        except ImportError:
            self.skipTest("PyYAML is not installed")
        text = textwrap.dedent(
            """\
            literal: |
              first line
              second line
            stripped: |-
              no trailing newline
            folded: >
              one paragraph
              continues here

              second paragraph
            after: done
            """
        )
        self.assertEqual(manadoc._load_simple_yaml(text, Path("x.yml")), yaml.safe_load(text))

    def test_real_configuration_parses_with_the_fallback(self):
        document = Path(__file__).resolve().parent.parent
        for path in (document / "wiki" / "wiki.yml", document / "pages" / "site.yml"):
            with self.subTest(path.name):
                manadoc._load_simple_yaml(path.read_text(encoding="utf-8"), path)

    def test_page_front_matter_parses_with_the_fallback(self):
        """The website manuscripts must load without PyYAML installed too."""
        pages = Path(__file__).resolve().parent.parent / "pages"
        found = sorted(pages.glob("*/*.md"))
        self.assertTrue(found, "no website manuscripts were found")
        for path in found:
            with self.subTest(path.name):
                text = path.read_text(encoding="utf-8")
                self.assertTrue(text.startswith("---"), "front matter is missing")
                header = text[text.index("\n") + 1:text.find("\n---", 3)]
                parsed = manadoc._load_simple_yaml(header, path)
                self.assertIn("title", parsed)


class WikiGenerationTest(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.tree = DocumentTree(Path(directory.name))
        for relative, text in JAPANESE_PAGES.items():
            self.tree.write("document/" + relative, text)
        self.tree.apply(self)
        self.config = manadoc.load_wiki_config(manadoc.WIKI_CONFIG)
        self.japanese = self.config.language("ja")

    def generate(self, language=None):
        self.problems = []
        generator = manadoc.WikiGenerator(
            self.config, language or self.japanese, on_issue=self.problems.append
        )
        return generator, generator.build_all()

    def test_pages_are_joined_in_configured_order(self):
        _, pages = self.generate()
        self.assertEqual(
            sorted(pages), ["Home", "Language-Reference", "Tutorial"]
        )
        tutorial = pages["Tutorial"]
        self.assertLess(
            tutorial.index("Mana チュートリアル"), tutorial.index("Action を Request する")
        )

    def test_page_starts_with_title_and_table_of_contents(self):
        _, pages = self.generate()
        head = pages["Tutorial"].splitlines()[:8]
        self.assertEqual(head[0], "# Tutorial")
        self.assertEqual(head[2], "## 目次")
        self.assertIn("- [Mana チュートリアル](#tutorial)", head)
        self.assertIn("- [Action を Request する](#tutorial-request)", head)

    def test_manuscript_headings_are_demoted(self):
        _, pages = self.generate()
        self.assertIn("\n## Action を Request する\n", pages["Tutorial"])
        self.assertIn("\n### 使い方\n", pages["Tutorial"])
        self.assertIn("\n# not a heading\n", pages["Tutorial"])

    def test_explicit_anchors_are_emitted(self):
        _, pages = self.generate()
        self.assertIn('<a id="tutorial-request"></a>', pages["Tutorial"])
        self.assertIn('<a id="tutorial"></a>', pages["Tutorial"])

    def test_links_inside_a_page_become_anchors(self):
        _, pages = self.generate()
        self.assertIn("[Action を Request する](#tutorial-request)", pages["Tutorial"])

    def test_links_to_another_page_name_that_page(self):
        _, pages = self.generate()
        self.assertIn("[構文](Language-Reference#reference-request)", pages["Tutorial"])
        self.assertIn("[チュートリアル](Tutorial#tutorial-request)", pages["Language-Reference"])

    def test_links_to_a_section_overview_name_the_page(self):
        _, pages = self.generate()
        self.assertIn("[チュートリアル](Tutorial)", pages["Home"])

    def test_images_point_at_the_published_assets(self):
        generator, pages = self.generate()
        self.assertIn("![Request の流れ](assets/ja/diagrams/request-flow.svg)", pages["Tutorial"])
        self.assertIn("ja/diagrams/request-flow.svg", generator.used_assets)

    def test_links_that_leave_the_manuscripts_point_at_the_repository(self):
        _, pages = self.generate()
        self.assertIn(
            "[完成コード](https://example.invalid/owner/repo/blob/master/examples/tutorial/03-request.mn)",
            pages["Tutorial"],
        )

    def test_external_links_are_untouched(self):
        _, pages = self.generate()
        self.assertIn("[GitHub](https://github.com/shun126/Mana)", pages["Tutorial"])

    def test_single_mode_publishes_the_manuscript_unchanged(self):
        _, pages = self.generate()
        self.assertTrue(pages["Home"].startswith("# Mana ドキュメント"))
        self.assertNotIn("## 目次", pages["Home"])

    def test_unlisted_manuscript_link_is_reported(self):
        self.tree.write(
            "document/wiki/ja/tutorial/tutorial-request.md",
            "# Action を Request する\n\n[未登録](./tutorial-loops.md)\n",
        )
        self.tree.write("document/wiki/ja/tutorial/tutorial-loops.md", "# Loops\n")
        self.generate()
        self.assertTrue(any("not listed in wiki.yml" in problem for problem in self.problems))

    def test_missing_manuscript_fails(self):
        (self.tree.document / "wiki" / "ja" / "tutorial" / "tutorial-request.md").unlink()
        with self.assertRaises(manadoc.ConfigError):
            self.generate()


class SidebarAndTranslationTest(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.tree = DocumentTree(Path(directory.name))
        for relative, text in JAPANESE_PAGES.items():
            self.tree.write("document/" + relative, text)
        self.tree.apply(self)
        self.config = manadoc.load_wiki_config(manadoc.WIKI_CONFIG)

    def add_english(self):
        for relative, text in JAPANESE_PAGES.items():
            self.tree.write("document/" + relative.replace("/ja/", "/en/"), text)

    def test_one_language_sidebar_has_no_headings(self):
        sidebar = manadoc.build_sidebar(self.config, [self.config.language("ja")])
        self.assertNotIn("## 日本語", sidebar)
        self.assertIn("- [Tutorial](Tutorial)", sidebar)
        self.assertIn("- [Language Reference](Language-Reference)", sidebar)

    def test_two_language_sidebar_groups_by_language(self):
        self.add_english()
        sidebar = manadoc.build_sidebar(
            self.config, [self.config.language("ja"), self.config.language("en")]
        )
        self.assertIn("## 日本語", sidebar)
        self.assertIn("## English", sidebar)
        self.assertIn("- [Tutorial](Tutorial-en)", sidebar)

    def test_no_language_link_while_english_is_missing(self):
        _, pages = self.generate("ja")
        self.assertNotIn("[English]", pages["Tutorial"])

    def test_language_links_appear_once_english_exists(self):
        self.add_english()
        _, japanese = self.generate("ja")
        _, english = self.generate("en")
        self.assertIn("[English](Tutorial-en)", japanese["Tutorial"])
        self.assertIn("[日本語](Tutorial)", english["Tutorial-en"])

    def test_english_pages_reuse_the_japanese_anchors(self):
        self.add_english()
        _, english = self.generate("en")
        self.assertIn('<a id="tutorial-request"></a>', english["Tutorial-en"])
        self.assertIn("[構文](Language-Reference-en#reference-request)", english["Tutorial-en"])

    def test_present_languages_reports_only_written_ones(self):
        self.assertEqual([language.code for language in self.config.present_languages()], ["ja"])
        self.add_english()
        self.assertEqual(
            sorted(language.code for language in self.config.present_languages()), ["en", "ja"]
        )

    def generate(self, code):
        self.problems = []
        generator = manadoc.WikiGenerator(
            self.config, self.config.language(code), on_issue=self.problems.append
        )
        return generator, generator.build_all()


class PartialTranslationTest(unittest.TestCase):
    """An optional language is published page by page as it is translated."""

    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.tree = DocumentTree(Path(directory.name))
        for relative, text in JAPANESE_PAGES.items():
            self.tree.write("document/" + relative, text)
        # Only Home and the reference have been translated.
        for relative in ("wiki/ja/Home.md", "wiki/ja/reference/reference-request.md"):
            self.tree.write(
                "document/" + relative.replace("/ja/", "/en/"), JAPANESE_PAGES[relative]
            )
        self.tree.apply(self)
        self.config = manadoc.load_wiki_config(manadoc.WIKI_CONFIG)
        self.english = self.config.language("en")

    def generate(self, code):
        generator = manadoc.WikiGenerator(self.config, self.config.language(code))
        return generator.build_all()

    def test_only_complete_pages_are_built(self):
        self.assertEqual(sorted(self.generate("en")), ["Home-en", "Language-Reference-en"])

    def test_links_to_untranslated_pages_use_the_default_language(self):
        english = self.generate("en")
        self.assertIn("[チュートリアル](Tutorial)", english["Home-en"])
        self.assertIn(
            "[チュートリアル](Tutorial#tutorial-request)", english["Language-Reference-en"]
        )

    def test_sidebar_lists_only_published_pages(self):
        sidebar = manadoc.build_sidebar(
            self.config, [self.config.language("ja"), self.english]
        )
        english = sidebar.split("## English", 1)[1]
        self.assertIn("- [Home](Home-en)", english)
        self.assertIn("- [Language Reference](Language-Reference-en)", english)
        self.assertNotIn("Tutorial-en", sidebar)

    def test_language_link_only_on_translated_pages(self):
        japanese = self.generate("ja")
        self.assertIn("[English](Language-Reference-en)", japanese["Language-Reference"])
        self.assertNotIn("[English]", japanese["Tutorial"])

    def test_language_without_finished_pages_is_left_out_of_the_sidebar(self):
        (self.tree.document / "wiki" / "en" / "Home.md").unlink()
        (self.tree.document / "wiki" / "en" / "reference" / "reference-request.md").write_text(
            "# request\n", encoding="utf-8"
        )
        self.config.pages = [page for page in self.config.pages if page.key == "Tutorial"]
        sidebar = manadoc.build_sidebar(
            self.config, [self.config.language("ja"), self.english]
        )
        self.assertNotIn("English", sidebar)

    def test_site_links_follow_the_published_translation(self):
        self.assertEqual(
            self.config.page_name_for("Language-Reference", "en"), "Language-Reference-en"
        )
        self.assertEqual(self.config.page_name_for("Tutorial", "en"), "Tutorial")
        self.assertEqual(self.config.page_name_for("Tutorial", "ja"), "Tutorial")
        self.assertEqual(self.config.page_name_for("Home", "fr"), "Home")
        with self.assertRaises(manadoc.ConfigError):
            self.config.page_name_for("No-Such-Page", "en")

    def test_required_language_still_fails_on_a_missing_page(self):
        self.english.optional = False
        with self.assertRaises(manadoc.ConfigError):
            self.generate("en")

    def test_check_docs_reports_warnings_not_errors(self):
        check_docs = _load_script("check-docs.py")
        report = check_docs.Report()
        check_docs.check_wiki_language(self.config, self.english, report)
        check_docs.check_wiki_translations(self.config, report)
        self.assertEqual(report.errors, [])
        self.assertTrue(
            any("Tutorial-en is not published yet" in message for message in report.warnings)
        )


def _load_script(name: str):
    """Import one of the hyphenated command line tools as a module."""
    import importlib.util

    path = Path(__file__).resolve().parent / name
    spec = importlib.util.spec_from_file_location(name.replace("-", "_")[:-3], path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class AssetTest(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.tree = DocumentTree(Path(directory.name))
        self.tree.apply(self)

    def test_common_and_language_assets_are_published(self):
        destination = self.tree.root / "build" / "assets"
        copied = manadoc.copy_assets([manadoc.Language("ja", "日本語", "ja")], destination)
        self.assertIn("common/logo_small.png", copied)
        self.assertIn("ja/diagrams/request-flow.svg", copied)
        self.assertTrue((destination / "common" / "logo_small.png").is_file())

    def test_assets_of_other_languages_are_not_published(self):
        self.tree.write("document/assets/en/diagrams/request-flow.svg", "<svg/>")
        copied = manadoc.copy_assets(
            [manadoc.Language("ja", "日本語", "ja")], self.tree.root / "build" / "assets"
        )
        self.assertNotIn("en/diagrams/request-flow.svg", copied)


if __name__ == "__main__":
    unittest.main()
