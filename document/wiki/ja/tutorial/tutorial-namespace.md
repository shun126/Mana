# namespace で名前を整理する

町にも別の場所にも `Guide` がいるとき、名前を区別する必要があります。**namespace（名前空間）**は、名前をグループに分ける仕組みです。

前の章の二つのファイルを、次の内容に置き換えます。

## 町の Actor を Town にまとめる

**lesson/town.mn のファイル全体：**

```mana
namespace Town
{
    actor Guide
    {
        action talk
        {
            print("Guide: Welcome!\n");
        }
    }

    actor Gate
    {
        action open
        {
            print("Gate: Open.\n");
        }
    }
}
```

`Guide` の完全な名前は `Town::Guide`、`Gate` は `Town::Gate` になりました。`::` は名前空間を含む名前の区切りです。

## 使う側で名前を指定する

**lesson/main.mn のファイル全体：**

```mana
import "town.mn";

using Town;

actor Event
{
    action main
    {
        awaitCompletion(10, Guide->talk);
        awaitCompletion(10, Gate->open);
        print("Event: Finished.\n");
    }
}
```

`using Town;` を書くと、`Town` 内の名前を短い形で参照できます。この例の `Guide` は `Town::Guide` を指します。

## 実行する

Mana フォルダーから実行します。

```text
mana lesson/main.mn
```

**期待する出力：**

```text
Guide: Welcome!
Gate: Open.
Event: Finished.
```

[同梱の main.mn](../../../../examples/tutorial/12-namespace/main.mn)と [town.mn](../../../../examples/tutorial/12-namespace/town.mn)も使えます。

```text
mana examples/tutorial/12-namespace/main.mn
```

## 一つ変えてみる

`using Town;` を削除し、main の二つの依頼を次に置き換えてください。

```mana
awaitCompletion(10, Town::Guide->talk);
awaitCompletion(10, Town::Gate->open);
```

同じ出力になれば、完全な名前で参照できています。

| 記号 | たどるもの |
| --- | --- |
| `::` | 名前空間。例：`Town::Guide` |
| `->` | Actor が持つ Action。例：`Town::Guide->talk` |

別の名前空間にも `Guide` があり、複数の `using` によってどちらか分からなくなる場合は、完全な名前で指定します。

## ファイルと名前空間は別の整理

`town.mn` というファイル名だけでは `Town` は作られません。逆に、一つの名前空間を複数ファイルへ分けることもできます。

まずは役割でファイルを分け、名前が衝突する、あるいは所属を明示したくなったときに名前空間を加えると扱いやすくなります。詳細は [Namespace リファレンス](../reference/reference-namespace.md)を参照してください。

## ここから自分のイベントへ

チュートリアルでは、文字を表示するところから始め、会話と門の順序、変数による記憶、条件、繰り返し、関数、割り込み、待機、ファイルと名前の整理を学びました。

復習するなら、この町の `Guide` に [会話回数](./tutorial-variables.md)を追加し、[条件分岐](./tutorial-conditions.md)で初回と2回目の会話を変えてみてください。進行役から会話を2回依頼し、それぞれの出力を確かめます。

目的に応じて次へ進んでください。

- Actor がどのように進むか理解する：[Mana の考え方](../concepts/README.md)
- 構文や制約を調べる：[言語リファレンス](../reference/README.md)
- 本物のゲームの処理につなぐ：[組み込みガイド](../integration/README.md)
