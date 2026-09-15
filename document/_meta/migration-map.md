# 旧ドキュメント移行表

このファイルは、従来の Mana ドキュメントから現行ドキュメントへ何を移行したかを記録する編集者向けメモです。

日本語版 v2 の主要セクション完成前QAで、旧ページに残っていた現行機能を再確認し、新しい Tutorial / Concepts / Reference / Integration へ移行しました。

| 旧ドキュメント | 主な移行先 | 移行内容 |
| --- | --- | --- |
| `primer_introduction-ja.md` | Getting Started / Concepts | Mana の目的、Actor-oriented、Request中心の考え方を再構成 |
| `primer_chapter_1-ja.md` | Getting Started / Tutorial / Concepts | コンパイル、Actor / Action / Request、Priority を用途別に分離 |
| `primer_chapter_2-ja.md` | Tutorial / Concepts / Reference | sender、Function、実行モデルを現行実装に合わせて再構成 |
| `primer_chapter_3-ja.md` | Tutorial / Reference | 型、変数、配列、演算子、制御文、goto などを移行 |
| `primer_chapter_4-ja.md` | Tutorial / Reference | `import` / `include`、複数ファイル構成を移行 |
| `primer_chapter_5-ja.md` | Concepts / Reference | Module / `extend`、Phantom を現行実装に合わせて正式化 |
| `primer_afterwords-ja.md` | Reference / Integration | 現行CLI、Diagnostics、Error Handlingへ必要な内容を再構成。旧エラー一覧等は廃止 |
| `undocumented_features-ja.md` | Reference / Integration | native、namespace / using、Phantom、SourceResolver等を正式文書へ移行 |

英語版の旧Primerも、日本語版v2の安定後に新構成から翻訳し直す方針とし、旧英語ページは原本として継続利用しません。

## QAで再確認した項目

- 定数宣言は `const 型 名前 = 値;`
- `define` / `undef` はLexerにトークンが残るが現行Parserの宣言構文ではない
- `mana` CLI の現行オプションと `--execute`
- Action 参照は `->`、`::` はnamespace修飾
- `namespace` / `using`
- `native` とStruct native Function
- `struct`
- `module` / `extend`
- `phantom`
- `request` / `awaitStart` / `awaitCompletion` / `join`
- `yield` / `lock` / `rollback` / `halt` / `refuse` / `comply`
- `self` / `sender` / `priority` / `this` / `Nil`
- 固定長配列と実行時添字チェック
- `static` / `allocate`
- `goto` / ラベル
- 定義済み複合型 `vec2` / `vec3` / `vec4` / `rotator` / `color`

## 旧ページの扱い

旧ページは現行仕様の原本にはしません。

Git の履歴から参照できるため、移行完了後は `document/` から削除し、`document/` を現行ドキュメントだけの source of truth とします。

今後仕様を変更した場合は、旧Primerではなく現行の Reference と実装・テストを同時に更新します。
