# Action の開始や終了を待つ

通常の `request` は、別の Actor に Action の実行を依頼した後、その完了を待たずに自分の処理を続けます。

```mana
request(kTalkPriority, NPC->talk);
print("continue\n");
```

しかしゲームイベントでは、

- 相手が Action を開始してから次へ進みたい
- 相手の Action が終了してから次へ進みたい
- すでに動いている Actor が一定の Priority まで下がるのを待ちたい

という場面があります。

Mana には、そのための待機・同期用の命令があります。

## このページで分かること

- `request` は相手の開始や終了を待たない
- `awaitStart` は Request した Action が開始できる状態になるまで待つ
- `awaitCompletion` は Request した Priority の処理が完了するまで待つ
- `join` は新しい Request を送らず、対象 Actor の Priority が指定値以下になるまで待つ
- `yield` は自分の Action の実行をいったん譲る
- `awaitStart` / `awaitCompletion` で自分自身の Action を待つことはできない

## 4つの命令を先に整理する

このページで扱う命令は、役割を分けて考えると理解しやすくなります。

| 命令 | Requestする | 待つもの |
| --- | --- | --- |
| `request` | する | 待たない |
| `awaitStart` | する | RequestしたActionの開始 |
| `awaitCompletion` | する | RequestしたPriorityの完了 |
| `join` | しない | 対象ActorのPriority |

`yield` はこの表とは少し異なり、自分自身の実行をいったん中断して他の処理へ実行機会を渡すために使います。

## `request` は待たない

まず通常の `request` を確認します。

```mana
actor Director
{
    action main
    {
        request(10, NPC->talk);
        print("Director continues.\n");
    }
}
```

`request` は NPC に `talk` の実行を依頼しますが、`NPC->talk` が終了するまで `Director` を停止させる命令ではありません。

相手の処理結果を待つ必要がない場合は、通常の `request` が最も単純です。

## 開始を待つ `awaitStart`

別の Actor が高い Priority の Action を実行中の場合、送った Request はすぐに実行対象にならないことがあります。

その Action が開始できる状態になるまで待ちたい場合は `awaitStart` を使います。

```mana
const int kTalkPriority = 10;

actor Director
{
    action main
    {
        print("request talk\n");
        awaitStart(kTalkPriority, NPC->talk);
        print("talk started\n");
    }
}

actor NPC
{
    action talk
    {
        print("NPC: Hello.\n");
    }
}
```

書き方は `request` とよく似ています。

```mana
awaitStart(priority, Actor->action);
```

`awaitStart` はまず Action を Request し、対象 Actor の実行 Priority が指定した Priority 以下になるまで呼び出し側を待機させます。

要求した Priority より高い Action が対象 Actor で実行されている場合、その高い Priority の処理が終わり、要求した Action が開始できる状態になるまで待つことになります。

```text
NPC Priority 100 : emergency
                      |
Director              | awaitStart(10, NPC->talk)
  waiting ------------+
                      |
                      v
NPC Priority 10  : talk
                      |
Director resumes <----+
```

## 終了を待つ `awaitCompletion`

Request した処理が終わってから次へ進みたい場合は `awaitCompletion` を使います。

```mana
const int kTalkPriority = 10;

actor Director
{
    action main
    {
        print("before talk\n");
        awaitCompletion(kTalkPriority, NPC->talk);
        print("after talk\n");
    }
}

actor NPC
{
    action talk
    {
        print("NPC: Hello.\n");
    }
}
```

基本形は次の通りです。

```mana
awaitCompletion(priority, Actor->action);
```

`awaitCompletion` は Action を Request した後、対象 Actor の Priority が指定した Priority より低くなるまで待ちます。

Priority 10 で Request した Actionなら、対象 Actor が Priority 10 以上の処理を終えて 10 未満へ戻ったところで、呼び出し側の処理が再開します。

```text
Director                    NPC
   |                         |
   | awaitCompletion(10, ...) |
   |------------------------>|
   |        waiting          | talk : Priority 10
   |                         |
   |                         | talk ends
   |<------------------------|
   | continue                |
```

会話、演出、ドアの開閉など、「相手の処理が終わった後で次のイベントへ進む」場面で使いやすい命令です。

## `awaitStart` と `awaitCompletion` の違い

違いは、どこで呼び出し側が再開するかです。

```text
                 Target Actor

Request --------> [ Action starts ] -------- [ Action ends ]
                         ^                         ^
                         |                         |
                   awaitStart                awaitCompletion
                    resumes                     resumes
```

相手が開始したことだけ確認できればよい場合は `awaitStart`、相手の処理が終わる必要がある場合は `awaitCompletion` を使います。

## 自分自身を `await` しない

次のようなコードは使用できません。

```mana
actor NPC
{
    action main
    {
        awaitCompletion(10, self->talk);
    }

    action talk
    {
    }
}
```

`awaitStart` と `awaitCompletion` は、待機している Actor 自身を対象にはできません。実行するとスクリプトエラーになります。

自分自身を待つと、待っている Actionを進めるために自分自身の実行が必要になるためです。

自分の Action の Priority 制御には `request` や Action の構成を使い、別 Actor との同期に `awaitStart` / `awaitCompletion` を使うと考えると分かりやすいでしょう。

## 既存の Actor を待つ `join`

`join` は `awaitStart` / `awaitCompletion` と違い、**新しい Request を送りません**。

すでに何らかの Action を実行している Actor の Priority が、指定値以下になるまで待ちます。

```mana
join(0, NPC);
```

基本形は次の通りです。

```mana
join(priority, Actor);
```

例えば NPC が Priority 10 の Action を実行中なら、

```mana
join(0, NPC);
```

は NPC の Priority が 0 以下になるまで待ちます。

`join` は「どの Action を Request したか」ではなく、対象 Actor の現在の Priority を条件として待機する命令です。

この違いは重要です。

```text
awaitCompletion
    Requestを送る
    +
    そのPriorityが完了するまで待つ

join
    Requestは送らない
    +
    既存ActorのPriorityが指定値以下になるまで待つ
```

特定の Action を開始して完了まで待つなら `awaitCompletion` の方が意図を表しやすく、すでに進行中の Actor の状態を待ち合わせるなら `join` が適しています。

## 実行をいったん譲る `yield`

`yield()` は別 Actor へ Request を送る命令ではありません。

現在実行している Action をその実行タイミングではいったん中断し、Mana VM に実行機会を返します。

```mana
actor Worker
{
    action main
    {
        print("step 1\n");
        yield();
        print("step 2\n");
    }
}
```

`yield()` の後も Action 自体が終了するわけではありません。次に実行機会が来たとき、続きから処理されます。

長く続く処理やループで、他の Actor にも実行機会を渡したい場合などに利用できます。

```mana
actor Worker
{
    action main
    {
        int count = 0;

        while (count < 10)
        {
            print("%d\n", count);
            count++;
            yield();
        }
    }
}
```

ただし `yield()` は「1秒待つ」「1フレーム待つ」といった時間指定の命令ではありません。どのタイミングで再び実行されるかは Mana VM を動かすホスト側の更新方法にも関係します。

## イベント進行では何を使うか

例えば、NPC に会話してもらい、会話が終わったら Door を開けるイベントを考えます。

```mana
const int kTalkPriority = 10;
const int kDoorPriority = 10;

actor EventDirector
{
    action main
    {
        awaitCompletion(kTalkPriority, NPC->talk);
        request(kDoorPriority, Door->open);
    }
}

actor NPC
{
    action talk
    {
        print("NPC: Welcome!\n");
    }
}

actor Door
{
    action open
    {
        print("Door opened.\n");
    }
}
```

このコードでは、

```text
NPC の talk を Request
        ↓
talk が終わるまで待つ
        ↓
Door の open を Request
```

というイベントの順序がコードから読み取れます。

Mana では、このように Actor / Action / Request と待機命令を組み合わせてイベントの進行を記述できます。

## ここまでで覚えておきたいこと

- `request` は依頼だけを行い、相手の完了を待たない
- `awaitStart` は Request を送り、対象 Actor がその Priority まで到達するのを待つ
- `awaitCompletion` は Request を送り、対象 Actor がその Priority 未満になるのを待つ
- `join` は Request を送らず、対象 Actor が指定 Priority 以下になるのを待つ
- `yield()` は現在の Action の実行をいったん Mana VM に返す
- `awaitStart` / `awaitCompletion` で自分自身を待つことはできない

## 次に学ぶこと

ここまでで、Mana の Actor / Action / Request / Priority と基本的な同期方法を使えるようになりました。

次はプログラムを複数のソースファイルへ分ける方法を学びます。
