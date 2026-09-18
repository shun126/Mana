# Mana ドキュメント

このディレクトリは、Mana のドキュメント原稿と、公開用の生成ツールを置く場所です。
読む目的でドキュメントを探している場合は、[GitHub Wiki](https://github.com/shun126/Mana/wiki) を参照してください。

## ディレクトリ構成

```text
document/
├─ wiki/        GitHub Wiki の原稿
├─ pages/       GitHub Pages 公式サイトの原稿
├─ assets/      Wiki / Pages 共通素材
└─ tools/       export / build / check ツール
```

チュートリアルの完成コードは、実行できるプログラムなのでリポジトリ直下の [`examples/tutorial/`](../examples/tutorial/) に置いています。

| ディレクトリ | 内容 |
| --- | --- |
| `wiki/<language>/` | マニュアルの source of truth。章ごとに分割した編集用原稿 |
| `wiki/wiki.yml` | Wiki ページの結合順、言語設定、ページ名 |
| `pages/<language>/` | 公式サイトの source of truth |
| `pages/site.yml` | サイトの言語設定と Wiki へのリンク先 |
| `pages/theme/` | サイトのレイアウトとスタイル |
| `assets/common/` | 言語に依存しない素材（ロゴなど） |
| `assets/<language>/` | 言語に依存する素材（日本語を含む図解など） |
| `tools/` | 生成・検査ツール |

## 運用方針

- **日本語が既定言語**です。`wiki/ja/` と `pages/ja/` が正式版にあたります。
- **`ja/` と `en/` は同じパス構造**にします。英訳を追加するときも、日本語版と同じファイル名を使います（`wiki/ja/tutorial/tutorial-request.md` に対して `wiki/en/tutorial/tutorial-request.md`）。英語版だけ別名にしないでください。
- **Wiki は章ごとに 1 ページへ結合して公開**します。`wiki/ja/tutorial/` の各ファイルは `Tutorial` という 1 ページになります。
- **Wiki の生成物は Git 管理しません。** `build/` は `.gitignore` 済みです。
- **Pages の生成 HTML も Git 管理しません。** `gh-pages` ブランチは使わず、GitHub Pages の artifact deploy で公開します。
- **公開先を直接編集しないでください。** GitHub Wiki を直接編集しても、次の公開で上書きされます。原稿はかならずこのディレクトリで直します。
- **画像は `assets/` の 1 か所だけ**に置きます。Wiki 用と Pages 用に同じ画像を二重管理しないでください。生成時に出力先へコピーされます。

原稿から公開までは一方向です。

```text
document/wiki/     ──┐
document/pages/    ──┼─→ document/tools/ ─→ build/ ─→ GitHub Wiki / GitHub Pages
document/assets/   ──┘
```

## Wiki の構成

`wiki/wiki.yml` の `pages` が、公開される Wiki ページと結合順を決めます。日本語版は次の 6 ページです。

| Wiki ページ | 原稿 |
| --- | --- |
| `Home` | `wiki/ja/Home.md` |
| `Getting-Started` | `wiki/ja/getting-started/` |
| `Tutorial` | `wiki/ja/tutorial/` |
| `Concepts` | `wiki/ja/concepts/` |
| `Language-Reference` | `wiki/ja/reference/` |
| `Integration` | `wiki/ja/integration/` |

`_Sidebar.md` も自動生成されます。全ページ下部のフッター `_Footer.md` は `wiki.yml` の `footer` から生成します。フッターは全言語で共通なので、日本語と英語を併記しています。

生成時には次の処理が行われます。

- 各原稿のトップレベル見出し（`#`）を 1 段階下げて、`##` として 1 ページへ並べます。コードブロック内の `#` は変更しません。
- 原稿ごとにファイル名由来のアンカー（例: `<a id="tutorial-request"></a>`）を出力します。`Tutorial#tutorial-request` という URL が、見出し文を変えても壊れません。英語版でも同じアンカー ID を使います。
- ページ先頭に目次を自動生成します。順番は `wiki.yml` のファイル順と一致します。
- 原稿間の Markdown リンクを、同一ページなら `#anchor`、別ページなら `Language-Reference#reference-request` のように書き換えます。
- 画像リンクを `assets/ja/diagrams/...` へ書き換え、必要な素材を出力先へコピーします。
- 原稿の外にあるリポジトリ内のファイル（`examples/tutorial/` の完成コードなど）へのリンクは、GitHub 上のファイルへのリンクになります。外部 URL は変更しません。

## ローカルで確認する

Python 3.12 以降が必要です。PyYAML があれば使いますが、入っていなくても動きます。

```bash
# 依存パッケージ（任意）
python -m pip install -r document/tools/requirements.txt

# 原稿の検査
python document/tools/check-docs.py

# Wiki の生成
python document/tools/export-wiki.py --language ja --output build/wiki

# 公式サイトの生成
python document/tools/build-pages.py --language ja --output build/pages

# ツールのテスト
python -m unittest discover -s document/tools -t document/tools

# 完成コードと教材の一致・実行結果の検証（ビルド済み mana が必要）
python document/tools/verify-examples.py x64/Release/mana.exe
```

生成した `build/pages/index.html` はブラウザでそのまま開けます。`build/wiki/*.md` は GitHub Wiki へ公開される内容そのものです。

`--language all` を指定すると、原稿が存在する言語をすべて生成します。原稿のない optional な言語はスキップされます。

`check-docs.py` は次を検査します。

- `wiki.yml` に書かれた原稿が存在し、同じ原稿が二重に登録されていないこと
- `wiki/<language>/` の原稿がすべてどこかのページに登録されていること
- 原稿内の内部リンクと画像の参照先が存在すること
- 生成したページが空でないこと
- 既定言語に対して他言語の原稿が揃っているか（optional な言語の不足は警告）
- `pages/` に既定言語の `index.md` があり、Wiki への導線と画像が揃っていること

警告もエラーとして扱いたい場合は `--strict` を付けます。

## 公開

| Workflow | 公開先 | 実行条件 |
| --- | --- | --- |
| `.github/workflows/publish-wiki.yml` | `https://github.com/shun126/Mana.wiki.git` | `master` への push、`workflow_dispatch` |
| `.github/workflows/publish-pages.yml` | GitHub Pages | `master` への push、`workflow_dispatch` |

どちらも `document/` 配下の関係するファイルと自身の workflow ファイルが変更されたときだけ動きます。Pull Request では検査と生成だけを行い、本番へは公開しません。

Wiki の同期はミラー方式です。生成結果が Wiki の全内容になるため、`wiki.yml` から外したページは Wiki からも消えます。差分がないときは commit も push も行いません。

### GitHub 側で一度だけ必要な設定

コードだけでは変更できない設定が 2 つあります。

1. **Wiki の初期化**
   GitHub の Wiki タブを開き、ページを 1 つ作成して保存します。最初のページが作られるまで `Mana.wiki.git` は存在せず、workflow は clone に失敗します。
2. **Pages の公開方法**
   `Settings` → `Pages` → `Build and deployment` → `Source` で **`GitHub Actions`** を選択します。

Wiki への push は、まず GitHub Actions の標準トークンで行います。標準トークンで push できない場合に限り、`Settings` → `Secrets and variables` → `Actions` で `WIKI_TOKEN` に Wiki への書き込み権限を持つトークンを登録してください。secret があればそちらが使われます。

## 英語版

Wiki と公式サイトには英語版があります（`document/wiki/en/`、`document/pages/en/`）。日本語版が原本で、英語版はその翻訳です。

**日本語の原稿を変更したら、同じ相対パスの英語の原稿も更新してください。** `check-docs.py` が検出できるのはファイルの欠落だけで、内容が古くなっていることは検出できません。

英語の原稿は、次の手順で追加・更新します。ディレクトリ構成は対称なので、再編は必要ありません。

1. `document/wiki/en/` に、`document/wiki/ja/` と**同じ相対パス・同じファイル名**で英訳を置きます。
2. 日本語を含む図がある場合は `document/assets/en/diagrams/` に、日本語版と同じファイル名で英語版を置きます。文字を含まない素材は `document/assets/common/` のままで構いません。
3. 公式サイトの英語版は `document/pages/en/index.md` です（作成済み）。サイトの図解は `document/assets/en/diagrams/` の英語版を使います。
4. `python document/tools/check-docs.py` を実行し、翻訳漏れの警告を確認します。
5. `python document/tools/export-wiki.py --language all --output build/wiki` で確認します。

英語 Wiki は `Home-en`、`Tutorial-en` のように `-en` を付けたページ名で生成されます（`wiki.yml` の `suffix`）。

翻訳は少しずつ公開できます。英語は `optional` な言語なので、**Wiki ページ単位で、そのページの原稿がすべて揃ったものから公開**されます。

- 原稿が揃っていないページは公開されず、`check-docs.py` と `export-wiki.py` が「何ファイル中いくつ翻訳済みか」を警告として表示します。エラーにはならないので、日本語 Wiki の公開は止まりません。
- 公開済みの英語ページから未公開のページへのリンクは、日本語版の同じ節（`Tutorial#tutorial-request` など）へつながります。アンカーは言語共通なので、英語版が公開されると自動的に英語ページへのリンクに変わります。
- Sidebar の English 見出しには、公開済みの英語ページだけが並びます。
- 各ページの見出しの下には、そのページがある言語を並べた言語バー（`🌐 **日本語** · [English](Tutorial-en)`）が入ります。今読んでいる言語が太字になり、両言語が揃ったページにだけ表示されます。
- GitHub の Wiki は日本語の `Home` から開くため、日本語 `Home` の冒頭に英語の案内（`wiki.yml` の `notice`）を入れ、英語の読者を `Home-en` へ案内します。

公式サイトは日本語 `/ja/` と英語 `/en/` の 2 言語です。ルート `/` は読者の言語のページへ移動します。

- サイトのヘッダーで以前に言語を選んでいれば、その言語を開きます（ブラウザに保存）。
- そうでなければブラウザの第一言語を見て、サイトにある言語（日本語）ならそのページを開きます。
- それ以外の言語の読者は `site.yml` の `fallback_language`（英語）を開きます。
- JavaScript が無効なときは、言語を選ぶページとして表示されます。
- 1 言語だけをビルドした場合（`--language en` など）は、その言語へ移動します。

各ページのヘッダーには全言語が並び、今読んでいる言語が強調表示されます。

サイトから Wiki へのリンクは `wiki:Tutorial` のように書きます。表示言語の Wiki ページが公開済みならそのページ（`Tutorial-en`）へ、未公開なら日本語のページ（`Tutorial`）へつながります。英語 Wiki の翻訳が進むと、サイトを作り直すだけでリンクが英語ページに切り替わります。このため `publish-pages.yml` は `document/wiki/` の変更でも実行されます。

## 原稿を書くときの注意

- 本文は現行の Compiler / VM 実装と照合済みです。構造の整理や公開方法の変更で、仕様の記述を書き換えないでください。
- 原稿間のリンクは相対パスで書きます（`./tutorial-request.md`、`../reference/reference-request.md`）。生成時に Wiki 用のリンクへ変換されます。
- 画像は `../../../assets/<language>/diagrams/...` のように `document/assets/` を参照します。
- 原稿を増やしたら `wiki.yml` の `files` に追加します。追加し忘れは `check-docs.py` がエラーにします。
- チュートリアルのコードを変えたら、`examples/tutorial/` の完成コードも同じ内容にそろえ、`verify-examples.py` で確認します。
