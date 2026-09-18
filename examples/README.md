# Mana examples

Runnable Mana programs. Run them from the repository root.

| Directory | Contents |
| --- | --- |
| [`tutorial/`](tutorial/) | The finished code of every chapter of the Japanese tutorial, one independent program per file. 日本語チュートリアルの各章の完成コードです。 |
| [`language-tour/`](language-tour/) | One program that uses most language features together: constants, structs, global and static variables, functions, actors, modules and phantoms. `make test` compiles and runs it on every CI build. 言語機能をひととおり使う総合サンプルで、CI のコンパイル確認にも使っています。 |

```bash
mana examples/tutorial/01-hello.mn
mana examples/language-tour/main.mn -I natives.mh
```

`language-tour/main.mn` includes the other files of that directory. `natives.mh`
declares native functions and is passed with `-I`, the way an application would
supply its own native function declarations. A path given to `-I` is resolved
from the directory of the source file, not the current directory, so it is
written as plain `natives.mh`.

## Keeping the tutorial code in step with the manual

The code in `tutorial/` also appears in the manuscripts under
`document/wiki/ja/`. After changing either, check that they still agree and
that every example prints what the manual says:

```bash
python document/tools/verify-examples.py x64/Release/mana.exe   # Windows
python document/tools/verify-examples.py driver/mana             # Linux / macOS
```
