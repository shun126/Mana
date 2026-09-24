# Actor-oriented という考え方

Mana は、ゲーム内にある複数の処理を **Actor という独立した実行単位に分けて考える** ためのスクリプト言語です。

NPC、扉、イベント進行、敵AI、演出制御などを一つの巨大な処理にまとめるのではなく、それぞれに役割を持たせ、必要なときに Action を Request して協調させます。

## 何を解決したいのか

ゲームでは、多くの処理が同時進行しているように見えます。

例えば、

- NPC が会話する
- 敵がプレイヤーを追いかける
- 扉が開く
- イベント管理が進行状態を覚える
- 演出が始まる

といった処理があります。

これらを一つの長い処理へ集めると、ある処理の都合が別の処理へ影響しやすくなり、状態管理も複雑になります。

Mana では、役割ごとに Actor を分けます。

```text
EventController
 ├─ Guide
 ├─ Gate
 └─ Guard
```

Actor は、それぞれ自分の状態と Action を持ちます。

## Actor はキャラクターだけではない

Actor という名前から、ゲームキャラクターだけを表すように見えるかもしれません。

しかし Mana の Actor は、**独立して Action を実行する基本的な実行単位**です。

例えば次のようなものを Actor にできます。

- NPC
- 敵
- 扉やスイッチなどのギミック
- イベント進行管理
- シーン制御
- 会話管理
- 演出管理

重要なのは、見た目のあるオブジェクトかどうかではなく、**独立した責務と行動を持たせたいか**です。

## Actor 同士は Request で協調する

Actor が別の Actor に何かをしてほしい場合、Mana では Action を直接関数呼び出しするのではなく Request します。

```mana
request(3, Guard->move);
```

この形にすることで、依頼する側は Guard の内部処理を細かく知る必要がありません。

```text
EventController
      │
      │ Request
      ▼
    Guard
      │
      └─ move Action
```

「誰が何をするか」を Actor と Action に分け、「いつ実行するか」を Request と Priority で調整するのが Mana の中心的な考え方です。

## 共有された時間の中で独立して動く

Mana VM は複数の Actor を順に進めます。

そのため、OS のスレッドを Actor ごとに作るわけではありません。各 Actor が別々の実行状態を持ち、VM の進行の中で少しずつ動くことで、ゲームから見ると複数の処理が並行して進んでいるように扱えます。

このドキュメントでは、この性質を **協調的な疑似並列実行** として説明します。

実際のCPU並列処理やマルチスレッド処理とは区別してください。

## 一般的な Actor Model との関係

Mana は Actor を中心に設計されていますが、学術的な Actor Model をそのまま実装した言語ではありません。

Mana 独自の仕組みとして、例えば次があります。

- Action
- Request
- Priority
- Action の割り込みと再開
- グローバル変数
- Mana VM による協調実行

そのため、Mana の `actor` は「一般的な Actor Model の actor と完全に同じもの」と考えるより、**ゲームの処理を独立した実行主体へ分けるための Mana 独自の Actor** と考える方が正確です。

## Actor に分ける基準

何でも Actor に分ければよいわけではありません。

Actor に向いているのは、例えば次のような処理です。

- 独立した状態を持つ
- 外部から行動を依頼される
- 他の処理とは別のタイミングで動く
- Priority による割り込みを扱いたい
- ゲーム上の責務として名前を付けやすい

一方、単純な計算や共通処理は Function の方が自然です。

```text
Actor / Action
    ゲーム上の「誰が何をするか」

Function
    Action の中で使う計算や共通処理
```

## Actor-oriented に考える利点

Actor ごとに責務を分けると、ゲームの処理を次のように捉えやすくなります。

```text
NPC は talk する
Door は open する
Guard は move する
EventController はそれらを Request する
```

コードを「命令の長い列」として見るのではなく、**複数の実行主体が互いに依頼しながらゲームを進める構造**として捉えられることが Mana の特徴です。

## ここまでで覚えておきたいこと

- Mana は Actor を中心に処理を分ける
- Actor はキャラクターに限定されない
- Actor は状態と Action を持つ
- Actor 同士は Request で協調する
- Mana の Actor は学術的 Actor Model と完全に同一ではない
- VM が複数 Actor を協調的に進める

## 次に読む

次は、Actor が持つ **Action** が通常の Function と何が違うのかを詳しく見ていきます。

[Actor と Action の役割](./concept-actor-and-action.md)
