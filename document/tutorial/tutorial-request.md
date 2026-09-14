# Action を Request する

前のページでは、Actor の中に複数の Action を定義しました。

このページでは、一方の Actor から別の Actor に「その Action を実行してほしい」と伝える **Request** を学びます。

## このページで分かること

- `request` は Actor に Action の実行を依頼する
- `Actor->action` は Action を指し示す書き方である
- `request` の第1引数は Priority である
- Priority の値が大きいほど優先度が高い
- 通常の `request` は、依頼した Action の終了を待たない

## 別の Actor に Action を依頼する

まず、次のプログラムを見てみましょう。

```mana
const int kTalkPriority = 1;

actor Mother
{
    action main
    {
        print("Mother: Dinner is ready!\n");
        request(kTalkPriority, Child->answer);
    }
}

actor Child
{
    action answer
    {
        print("Child: OK!\n");
    }
}
```

`Mother` の `main` Action から、`Child` の `answer` Action へ Request を送っています。

実行すると、次のような結果になります。

```text
Mother: Dinner is ready!
Child: OK!
```

これで、2つの Actor が連携しました。

## `request` の書き方

基本形は次の通りです。

```mana
request(priority, Actor->action);
```

今回の例では、

```mana
request(kTalkPriority, Child->answer);
```

と書いています。

これは、

> Priority `kTalkPriority` で、`Child` Actor の `answer` Action を実行してほしい

という意味です。

## `Actor->action` は Action への参照

Mana では、Actor が持つ Action を指し示すために `->` を使います。

```mana
Child->answer
```

左側の `Child` が Actor、右側の `answer` が Action です。

例えば次の Actor があるとします。

```mana
actor Guard
{
    action talk
    {
    }

    action warn
    {
    }
}
```

それぞれの Action は、次のように参照できます。

```mana
Guard->talk
Guard->warn
```

この `->` は Mana で Action を扱うための重要な記号です。

## 第1引数は Priority

`request` の第1引数は **Priority（優先度）** です。

```mana
request(1, Child->answer);
```

ここで `1` は1秒後という意味でも、1フレーム後という意味でもありません。

**値が大きいほど高い Priority** を表します。

ただし、Priority の詳しい動作は後のチュートリアルで扱います。今は「Action の実行には優先度を付けて Request する」と覚えてください。

数値を直接書くより、意味の分かる定数を使うことを推奨します。

```mana
const int kTalkPriority = 1;
request(kTalkPriority, Child->answer);
```

こうすると、`1` が何を意味しているのかコードから読み取りやすくなります。

## Request は「関数呼び出し」とは少し違う

C や C++ などの関数呼び出しに慣れている場合、`request` を普通の関数呼び出しと同じものだと考えない方がよいでしょう。

```mana
request(kTalkPriority, Child->answer);
```

は、`Child` の処理へ直接ジャンプして、終了するまで `Mother` が待つという意味ではありません。

Request は `Child` Actor に対して Action の実行を依頼します。`Child` は自分の実行状態と Priority に従って、その Request を処理します。

通常の `request` を送った側は、その Action の完了を待たずに自分の処理を続けます。

例えば、

```mana
actor Mother
{
    action main
    {
        print("Mother: request\n");
        request(1, Child->answer);
        print("Mother: continue\n");
    }
}

actor Child
{
    action answer
    {
        print("Child: answer\n");
    }
}
```

では、`request` の次にある `Mother` の処理は、`Child->answer` の終了を待つための記述にはなっていません。

「開始を待ちたい」「終了まで待ちたい」といった同期方法は、後のチュートリアルで扱います。

## Mana では Actor 同士が Request で連携する

Mana の特徴の一つは、Actor がそれぞれ Action を持ち、Request によって互いに連携できることです。

```text
Mother
  |
  | request(1, Child->answer)
  v
Child
  |
  +-- answer
```

ゲームイベントなら、例えば次のような関係を作れます。

```text
Player
  |
  | request
  v
NPC->talk

NPC
  |
  | request
  v
Door->open
```

複雑なイベントでも、Actor ごとに役割を分け、Action と Request の関係として記述できます。

## 少し変更してみる

`Child` にもう一つ Action を追加してみましょう。

```mana
const int kActionPriority = 1;

actor Mother
{
    action main
    {
        request(kActionPriority, Child->cleanRoom);
    }
}

actor Child
{
    action answer
    {
        print("Child: OK!\n");
    }

    action cleanRoom
    {
        print("Child: Cleaning my room.\n");
    }
}
```

`request` の Action 参照を書き換えるだけで、依頼する行動を変更できます。

## ここまでで覚えておきたいこと

- Actor は Action を持つ
- `request` で Actor に Action の実行を依頼できる
- Action は `Actor->action` と書いて参照する
- `request` の第1引数は Priority
- Priority は大きい値ほど優先度が高い
- 通常の `request` は相手の Action の終了を待たない

ここまでで、Mana の中心となる **Actor / Action / Request** の基本を使えるようになりました。

## 次に学ぶこと

次は、プログラムの状態を扱うための **変数** を学びます。
