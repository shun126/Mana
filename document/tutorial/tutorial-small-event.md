# 小さなゲームイベントを作る

ここまで学んだ Mana の機能を組み合わせて、小さなゲームイベントを作ってみましょう。

今回作るのは、次のようなイベントです。

1. 案内役の NPC に話しかける
2. 警備員が門を開ける
3. 画面通知を表示する
4. もう一度 NPC に話しかけると、会話内容が変わる

この例では、Actor / Action / Request、Priority、状態変数、条件分岐、待機、複数ファイル、namespace を一つの流れで使います。

## このページで分かること

- 複数の Actor を役割ごとに分ける
- Actor が状態を保持する
- `awaitCompletion` で Action の終了を待つ
- `request` で待たずに別 Actor へ処理を依頼する
- 複数ファイルと namespace を組み合わせる
- 小さなイベントを Mana で組み立てる考え方

## 作るイベント

全体の関係は次のようになります。

```text
EventController
    |
    | awaitCompletion
    v
Guide.talk
    |
    | awaitCompletion
    v
Guard.openGate
    |
    | request
    v
Notice.show

EventController
    |
    | awaitCompletion
    v
Guide.talk  （2回目は会話が変わる）
```

`EventController` がイベント全体の進行を担当します。

`Guide`、`Guard`、`Notice` は、それぞれ自分の役割だけを担当します。

## ファイルを2つに分ける

今回は次の2ファイルを作ります。

```text
main.mn
└─ town.mn
```

`town.mn` に町の Actor をまとめ、`main.mn` にイベント進行を書きます。

## `town.mn` を作る

```mana
namespace Game::Town
{
    const int kTalkPriority = 10;
    const int kGatePriority = 5;
    const int kNoticePriority = 1;

    actor Guide
    {
        int mTalkCount;

        action init
        {
            mTalkCount = 0;
        }

        action talk
        {
            if (mTalkCount == 0)
            {
                print("Guide: この先の門を開けてもらいます。\n");
            }
            else
            {
                print("Guide: 門は開いています。お気をつけて。\n");
            }

            mTalkCount = mTalkCount + 1;
        }
    }

    actor Guard
    {
        action openGate
        {
            print("Guard: 門を開けます。\n");
        }
    }

    actor Notice
    {
        action show
        {
            print("Notice: 門が開きました。\n");
        }
    }
}
```

### Guide は会話回数を覚える

`Guide` は Actor 変数 `mTalkCount` を持っています。

```mana
int mTalkCount;
```

`init` Action で最初に `0` を設定します。

```mana
action init
{
    mTalkCount = 0;
}
```

Mana VM は Actor の `init` を起動時の初期化処理として実行し、その後 `main` を開始します。

`talk` では、この値によって会話を変えます。

```mana
if (mTalkCount == 0)
{
    // 最初の会話
}
else
{
    // 2回目以降の会話
}
```

Action が終わっても Actor 変数は残るため、次に `talk` が実行されたときも前回の状態を利用できます。

## `main.mn` を作る

```mana
import "town.mn";

using Game::Town;

actor EventController
{
    action main
    {
        print("Event: 開始\n");

        awaitCompletion(kTalkPriority, Guide->talk);
        awaitCompletion(kGatePriority, Guard->openGate);

        request(kNoticePriority, Notice->show);

        awaitCompletion(kTalkPriority, Guide->talk);

        print("Event: 終了\n");
    }
}
```

`main.mn` が今回の入口です。

```mana
import "town.mn";
```

によって `town.mn` を同じコンパイル単位へ読み込みます。

続いて、

```mana
using Game::Town;
```

と書くことで、`Guide`、`Guard`、`Notice` を短い名前で参照しています。

## イベントを順番に進める

最初の会話は、

```mana
awaitCompletion(kTalkPriority, Guide->talk);
```

で開始します。

`awaitCompletion` は Request を送り、その Priority の Action が終了するまで呼び出し側の進行を待ちます。

そのため、`Guide->talk` が終わってから次へ進みます。

次に、

```mana
awaitCompletion(kGatePriority, Guard->openGate);
```

で警備員の処理が終了するまで待ちます。

この2つは、イベントの順序を保証したい処理です。

```text
Guide.talk 完了
    ↓
Guard.openGate 完了
    ↓
次の処理
```

という順番になります。

## 通知は待たずに依頼する

門を開けた後、通知表示は次のようにしています。

```mana
request(kNoticePriority, Notice->show);
```

ここでは `awaitCompletion` ではなく `request` を使っています。

イベント進行側は `Notice->show` の終了を待つ必要がないからです。

`request` の直後には、そのまま次の処理へ進めます。

```text
EventController ── request ──> Notice.show
      |
      +── そのまま次へ進む
```

このように、

- 終了を待つ必要がある処理は `awaitCompletion`
- 依頼だけして先へ進みたい処理は `request`

と考えると、イベントを組み立てやすくなります。

## 2回目の会話が変わる

最後にもう一度、

```mana
awaitCompletion(kTalkPriority, Guide->talk);
```

を実行します。

1回目の `talk` で、

```mana
mTalkCount = mTalkCount + 1;
```

が実行されているため、今度は `mTalkCount` が `0` ではありません。

そのため2回目は、

```text
Guide: 門は開いています。お気をつけて。
```

という別の会話になります。

これは、Actor が自分の状態を保持できることを利用した簡単なイベント進行です。

## 実行する

`main.mn` を指定して実行します。

```bash
mana main.mn
```

主な流れは次のようになります。

```text
Event: 開始
Guide: この先の門を開けてもらいます。
Guard: 門を開けます。
Guide: 門は開いています。お気をつけて。
Event: 終了
```

`Notice` は通常の `request` で実行しているため、通知の出力位置は EventController の進行と厳密には同期していません。

ここでは、それが意図した使い方です。

順序を保証したいのであれば `awaitCompletion` を使います。

## Actor ごとに責務を分ける

この例では、一つの大きな Action にすべてを書いていません。

| Actor | 役割 |
| --- | --- |
| `EventController` | イベント全体の進行 |
| `Guide` | NPC の会話と会話状態 |
| `Guard` | 門を開ける行動 |
| `Notice` | 通知表示 |

実際のゲームでは、これらが C++ 側のゲームオブジェクトやシステムと `native` 関数を通じて連携することもあります。

Mana 側では「誰が、どの Action を、どの順番で実行するか」を記述する、という分け方ができます。

## Priority の数字に名前を付ける

今回は、

```mana
const int kTalkPriority = 10;
const int kGatePriority = 5;
const int kNoticePriority = 1;
```

と定数にしました。

次のように直接数字を書くこともできます。

```mana
awaitCompletion(10, Guide->talk);
```

しかし、規模が大きくなると数字だけでは意味が分かりにくくなります。

Priority に名前を付けることで、コードを読んだときに役割を理解しやすくなります。

## さらに発展させるなら

このイベントには、まだ多くの機能を追加できます。

例えば、

- プレイヤーが鍵を持っているときだけ門を開ける
- 3回目以降は別の会話にする
- 門を開ける途中で別の高 Priority Action を割り込ませる
- 複数の NPC が同時に行動する
- C++ 側の `native` 関数でアニメーションやサウンドを再生する

といった発展が考えられます。

Mana では、こうしたゲーム中の行動を Actor と Action に分け、Request と Priority で連携させていきます。

## Tutorial で学んだこと

ここまでの Tutorial では、次の内容を扱いました。

- Actor と Action
- Request
- 変数
- 条件分岐
- 繰り返し
- Function
- Priority
- `awaitStart` / `awaitCompletion` / `join` / `yield`
- 複数ファイル
- namespace / `using`

これで、Mana の基本的なコードを読み、小規模なゲームイベントを組み立てるための基礎がそろいました。

## 次に読むもの

Tutorial の次は **Mana の考え方（Concepts）** へ進みます。

Tutorial では「どう書くか」を中心に学びました。

Concepts では、Actor、Action、Request、Priority、スケジューリングなどが **なぜそのように動くのか** を、Mana の実行モデルとして整理します。
