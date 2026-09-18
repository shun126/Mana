# 入門・チュートリアルの完成コード

[準備ページ](../../document/wiki/ja/getting-started/getting-started-installation.md)で設定したターミナルを使い、Mana フォルダーから実行してください。各行が独立した例です。

| 学ぶこと | コマンド |
| --- | --- |
| 文字を表示する | `mana examples/tutorial/01-hello.mn` |
| 定義と実行 | `mana examples/tutorial/02-actor.mn` |
| 行動を依頼する | `mana examples/tutorial/03-request.mn` |
| 会話して門を開ける | `mana examples/tutorial/04-event.mn` |
| 会話回数を覚える | `mana examples/tutorial/05-variables.mn` |
| 鍵があれば開ける | `mana examples/tutorial/06-conditions.mn` |
| 回数を数える | `mana examples/tutorial/07-loops.mn` |
| 必要数を計算する | `mana examples/tutorial/08-functions.mn` |
| 割り込みと復帰 | `mana examples/tutorial/09-priority.mn` |
| 実行を譲る | `mana examples/tutorial/10-yield.mn` |
| ファイルを分ける | `mana examples/tutorial/11-files/main.mn` |
| 名前空間を使う | `mana examples/tutorial/12-namespace/main.mn` |

出力と練習問題は [チュートリアル](../../document/wiki/ja/tutorial/README.md)に掲載しています。例は UTF-8 で保存しています。

## 文書編集者向けの検証

Python 3 とビルド済み Mana を用意し、Mana フォルダーから実行します。

```text
python document/tools/verify-examples.py x64/Release/mana.exe
```

Linux では実行ファイルを `driver/mana` に置き換えてください。完成例の出力と終了コード、教材の変更例、意図的なコンパイルエラー、Markdown の完成コードと同梱ファイルの一致を検証します。
