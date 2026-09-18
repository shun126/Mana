# Mana チュートリアル

案内役が話し、門が開く小さなイベントを作ります。3章目で一度完成させ、その後で状態、条件、繰り返し、同期を学びます。最後に同じイベントを複数のファイルと名前空間へ整理します。

[はじめての Mana プログラム](../getting-started/getting-started-first-program.md)を実行できたところから始めます。プログラミング経験は前提にしません。

## 読む順番

1. [Actor と Action](./tutorial-actor-and-action.md)
2. [Action を Request する](./tutorial-request.md)
3. [会話して門を開ける](./tutorial-small-event.md)
4. [変数で状態を覚える](./tutorial-variables.md)
5. [条件によって処理を変える](./tutorial-conditions.md)
6. [処理を繰り返す](./tutorial-loops.md)
7. [関数で処理をまとめる](./tutorial-functions.md)
8. [Priority で割り込みと復帰を扱う](./tutorial-priority.md)
9. [待機と同期を使い分ける](./tutorial-wait-and-synchronization.md)
10. [複数のファイルに分ける](./tutorial-multiple-files.md)
11. [namespace で名前を整理する](./tutorial-namespace.md)

## この教材の使い方

- 「ファイル全体」のコードは、そのまま保存して実行できます。前の章のコードへ追加せず、置き換えます。
- 「抜粋」「置き換える例」などは、指定した場所へ入れるコードです。
- 実行コマンドはターミナルへ、Mana のコードはエディタへ入力します。
- 各章の期待する出力を確認してから、「一つ変えてみる」に進みます。
- 数字の表示や `yield()` は、秒数を待つ機能とは限りません。教材では出力で処理の流れを観察します。

[完成コード一覧](../../../../examples/tutorial/README.md)からも実行できます。各例は独立したプログラムです。全部をまとめて import する用途ではありません。

構文を調べたいときは [言語リファレンス](../reference/README.md)、仕組みを整理したいときは [Mana の考え方](../concepts/README.md)を参照してください。
