"""Verify executable documentation examples with Python 3.9 or newer."""
import argparse
import re
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
EXAMPLES = ROOT / "examples" / "tutorial"
ARTICLES = ROOT / "documents" / "wiki" / "ja"
CASES = {
    "01-hello.mn": ("getting-started/getting-started-first-program.md", "Hello, Mana!\n"),
    "02-actor.mn": ("tutorial/tutorial-actor-and-action.md", "Guide: Ready.\n"),
    "03-request.mn": ("tutorial/tutorial-request.md", "Event: Request.\nGuide: Welcome!\n"),
    "04-event.mn": ("tutorial/tutorial-small-event.md", "Guide: Welcome!\nGate: Open.\nEvent: Finished.\n"),
    "05-variables.mn": ("tutorial/tutorial-variables.md", "Talk count: 1\nTalk count: 2\n"),
    "06-conditions.mn": ("tutorial/tutorial-conditions.md", "Guide: Welcome!\nGate: Open.\n"),
    "07-loops.mn": ("tutorial/tutorial-loops.md", "Remaining: 3\nRemaining: 2\nRemaining: 1\nGate: Open.\n"),
    "08-functions.mn": ("tutorial/tutorial-functions.md", "Missing keys: 2\n"),
    "09-priority.mn": ("tutorial/tutorial-priority.md", "Guide: Talk begins.\nGuide: Watch out!\nGuide: Talk resumes.\nEvent: Finished.\n"),
    "10-yield.mn": ("tutorial/tutorial-wait-and-synchronization.md", "Guide: Step 1.\nGuide: Step 2.\nGuide: Step 3.\n"),
    "11-files/main.mn": ("tutorial/tutorial-multiple-files.md", "Guide: Welcome!\nGate: Open.\nEvent: Finished.\n"),
    "12-namespace/main.mn": ("tutorial/tutorial-namespace.md", "Guide: Welcome!\nGate: Open.\nEvent: Finished.\n"),
}


def blocks(document, language):
    fence = chr(96) * 3
    return re.findall(fence + language + r"\r?\n(.*?)" + fence, document, re.S)


def run(executable, source, expected, *, fails=False, options=()):
    result = subprocess.run(
        [str(executable), *options, str(source)], cwd=ROOT,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=10,
        encoding="utf-8", errors="replace",
    )
    output = result.stdout.replace("\r\n", "\n")
    if fails:
        assert result.returncode != 0, f"{source}: invalid source was accepted"
        assert "error" in (output + result.stderr).lower(), result
    else:
        assert result.returncode == 0, f"{source}:\n{output}{result.stderr}"
        assert output.strip() == expected.strip(), (
            f"{source}: expected {expected!r}, got {output!r}\n{result.stderr}"
        )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mana", type=Path, help="Path to the built Mana executable")
    args = parser.parse_args()
    executable = args.mana.resolve()
    assert executable.is_file(), f"Executable not found: {executable}"
    count = 0
    for name, (page, expected) in CASES.items():
        source = EXAMPLES / name
        document = (ARTICLES / page).read_text(encoding="utf-8")
        assert source.read_text(encoding="utf-8").strip() in [
            b.strip() for b in blocks(document, "mana")
        ], f"{name}: code differs from the article"
        assert expected.strip() in [b.strip() for b in blocks(document, "text")], (
            f"{name}: expected output differs from the article"
        )
        run(executable, source, expected)
        count += 1
        if "/" in name:
            companion = source.with_name("town.mn")
            assert companion.read_text(encoding="utf-8").strip() in [
                b.strip() for b in blocks(document, "mana")
            ], f"{companion}: code differs from the article"

    variants = [
        ("05-variables.mn",
         "        awaitCompletion(10, Guide->talk);\n    }",
         "        awaitCompletion(10, Guide->talk);\n        awaitCompletion(10, Guide->talk);\n    }",
         "Talk count: 1\nTalk count: 2\nTalk count: 3\n"),
        ("06-conditions.mn", "hasKey = true", "hasKey = false",
         "Guide: Welcome!\nEvent: Find the key.\n"),
        ("07-loops.mn", "remaining = 3", "remaining = 0", "Gate: Open.\n"),
        ("07-loops.mn", "remaining = 3", "remaining = 5",
         "Remaining: 5\nRemaining: 4\nRemaining: 3\nRemaining: 2\nRemaining: 1\nGate: Open.\n"),
        ("08-functions.mn", "remainingKeys(3, 1)", "remainingKeys(3, 5)", "Missing keys: 0\n"),
        ("09-priority.mn", "kEmergencyPriority = 100", "kEmergencyPriority = 10",
         "Guide: Talk begins.\nGuide: Talk resumes.\nEvent: Finished.\n"),
        ("02-actor.mn", "Guide: Ready.", "Guide: Waiting.", "Guide: Waiting.\n"),
        ("03-request.mn", "talk", "greet", "Event: Request.\nGuide: Welcome!\n"),
        ("04-event.mn",
         "        awaitCompletion(10, Guide->talk);\n        awaitCompletion(10, Gate->open);",
         "        awaitCompletion(10, Gate->open);\n        awaitCompletion(10, Guide->talk);",
         "Gate: Open.\nGuide: Welcome!\nEvent: Finished.\n"),
    ]
    with tempfile.TemporaryDirectory(prefix="mana-doc-") as directory:
        target = Path(directory) / "exercise.mn"
        for name, before, after, expected in variants:
            code = (EXAMPLES / name).read_text(encoding="utf-8")
            assert before in code
            target.write_text(code.replace(before, after), encoding="utf-8")
            run(executable, target, expected)
            count += 1

        article = (ARTICLES / "getting-started/getting-started-compile-and-run.md").read_text(encoding="utf-8")
        target.write_text(blocks(article, "mana")[0], encoding="utf-8")
        run(executable, target, "", fails=True)
        count += 1

        article = (ARTICLES / "tutorial/tutorial-conditions.md").read_text(encoding="utf-8")
        code = (EXAMPLES / "05-variables.mn").read_text(encoding="utf-8")
        old_body = '        mTalkCount = mTalkCount + 1;\n        print("Talk count: %d\\n", mTalkCount);'
        new_body = "\n".join("        " + line for line in blocks(article, "mana")[-1].strip().splitlines())
        assert old_body in code
        target.write_text(code.replace(old_body, new_body), encoding="utf-8")
        run(executable, target, "Guide: Welcome!\nGuide: Welcome back!\n")
        count += 1

        # Execute the replacement fragments printed in the lessons.
        fragments = [
            ("getting-started/getting-started-first-program.md", 1,
             "01-hello.mn", '        print("Hello, Mana!\\n");',
             "Welcome!\nThe gate is closed.\n"),
            ("tutorial/tutorial-variables.md", 1,
             "05-variables.mn", old_body, "Talk count: 1\nTalk count: 1\n"),
            ("tutorial/tutorial-loops.md", 1,
             "07-loops.mn", None, "Step: 0\nStep: 1\nStep: 2\nGate: Open.\n"),
        ]
        for page, index, name, old_fragment, expected in fragments:
            document = (ARTICLES / page).read_text(encoding="utf-8")
            fragment = "\n".join("        " + line for line in blocks(document, "mana")[index].strip().splitlines())
            code = (EXAMPLES / name).read_text(encoding="utf-8")
            if old_fragment is None:
                start = code.index("        int remaining")
                end = code.index("\n    }", start)
                old_fragment = code[start:end]
            assert old_fragment in code
            target.write_text(code.replace(old_fragment, fragment), encoding="utf-8")
            run(executable, target, expected)
            count += 1

        code = (EXAMPLES / "12-namespace/main.mn").read_text(encoding="utf-8")
        code = code.replace("using Town;", "").replace("Guide->", "Town::Guide->").replace("Gate->", "Town::Gate->")
        target.write_text(code, encoding="utf-8")
        (target.parent / "town.mn").write_text((EXAMPLES / "12-namespace/town.mn").read_text(encoding="utf-8"), encoding="utf-8")
        run(executable, target, CASES["12-namespace/main.mn"][1])
        count += 1

        # Verify the compile/save/execute instructions, including no automatic run.
        program_image = Path(directory) / "hello.mx"
        run(executable, EXAMPLES / "01-hello.mn", "", options=("-o", str(program_image)))
        assert program_image.is_file()
        run(executable, program_image, "Hello, Mana!\n", options=("--execute",))
        count += 2

        # Built-in delay must also be registered when executing a saved image.
        program_image = Path(directory) / "delay.mx"
        run(executable, EXAMPLES / "10-yield.mn", "", options=("-o", str(program_image)))
        run(executable, program_image, CASES["10-yield.mn"][1], options=("--execute",))
        count += 2

    print(f"PASS: {count} executions; article code and expected output match.")


if __name__ == "__main__":
    main()
