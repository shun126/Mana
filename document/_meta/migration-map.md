# 旧ドキュメント移行表

このファイルは、従来の Mana ドキュメントを新しい構成へ移行するための編集者向けメモです。

旧ファイルは、新しいドキュメントへの移行が完了するまで削除しません。

| 旧ドキュメント | 主な移行先 | 方針 |
| --- | --- | --- |
| `primer_introduction-ja.md` | Getting Started / Concepts | Mana の目的と特徴を再構成する |
| `primer_chapter_1-ja.md` | Getting Started / Tutorial / Concepts | コンパイル説明、Actor / Action / Request、Priority を分離する |
| `primer_chapter_2-ja.md` | Tutorial / Concepts / Reference | sender、lock、関数などを用途別に分離する |
| `primer_chapter_3-ja.md` | Tutorial / Reference | 型、変数、演算子、制御文などを入門と仕様に分離する |
| `primer_chapter_4-ja.md` | Reference | include / import と複数ファイル構成を整理する |
| `primer_chapter_5-ja.md` | Concepts / Reference | module、phantom などを現行実装に合わせて正式化する |
| `primer_afterwords-ja.md` | README または廃止 | 現在も必要な内容だけ再利用する |
| `undocumented_features-ja.md` | Reference / Integration | native、namespace / using、phantom などを正式リファレンスへ昇格する |

## 移行時にそのままコピーしないもの

旧文書には、現在の実装と異なる説明が含まれています。文章をそのまま移動するのではなく、現行のソースコード、テスト、サンプルを確認してから書き直します。

特に次の項目は再確認します。

- 定数宣言の構文
- `mana` コマンドのコンパイルと実行方法
- Action 参照の `->`
- `namespace` / `using`
- `native`
- `struct`
- `module` / `extend`
- `phantom`
- `request` / `awaitStart` / `awaitCompletion` / `join`
- `yield` / `lock` / `rollback` / `refuse`
- `self` / `sender` / `priority` / `this` / `nil`

## 削除の条件

旧ページは、その内容が新ドキュメントへ移行され、リンク切れや重要な情報の欠落がないことを確認した後に削除します。
