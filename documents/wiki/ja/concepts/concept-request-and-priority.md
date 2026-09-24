# Request と Priority

Mana では、Actor に Action を実行してもらうときに Request を使います。

```mana
request(3, Guard->move);
```

この式は、`Guard` Actor に対して `move` Action を Priority 3 で依頼します。

## Request は関数呼び出しではない

通常の関数呼び出しは、呼び出した処理が終わるまでその場で待ちます。

Request はそれとは異なり、対象 Actor の実行状態へ Action を登録します。

```text
EventController
      │
      │ request(3, Guard->move)
      ▼
    Guard
      │
      └─ priority 3 : move
```

通常の `request` は、Request を発行した側が対象 Action の終了を待ちません。

対象 Action の開始や終了を待つ必要がある場合は `awaitStart` や `awaitCompletion` を使います。

## Priority は Action の重要度を表す

Mana では数値が大きいほど Priority が高くなります。

例えば Guard が Priority 1 の `patrol` を実行中に、Priority 5 の `damage` が Request されたとします。

```text
priority 5 : damage   ← 実行
priority 1 : patrol   ← 中断
```

高い Priority の Action が先に実行され、元の Action は途中の状態を保持したまま中断されます。

`damage` が終了すると、保存されていた `patrol` の位置へ戻って再開できます。

## 低い Priority の Request

現在実行中の Action より低い Priority の Request は、すぐには実行されません。

```text
現在
priority 5 : battle

新しいRequest
priority 2 : talk
```

この場合 `talk` は Priority 2 の実行候補として保持され、Priority 5 の処理が終わった後に実行できる状態になります。

つまり Priority は単なる並べ替えの数値ではなく、**Actor の中でどの Action を今実行し、どの Action を待たせるか**を決める仕組みです。

## 同じ Priority は一つだけ

現在の Mana 実装では、一つの Actor の同じ Priority に複数の Request を保持できません。

すでに Priority 3 の Request が登録されている状態で、別の Priority 3 の Action を Request すると、その新しい Request は受け付けられません。

```text
Guard
priority 3 : talk

request(3, Guard->move)
        ↓
同じPriorityが使用中なので受け付けられない
```

このため Priority は、「重要度」であると同時に Actor 内の実行スロットのような役割も持っています。

Priority を細かく数値化しすぎるより、ゲーム側で意味のある段階を決めて使う方が管理しやすくなります。

例えば、

```text
1 : 通常行動
3 : 会話・イベント
5 : ダメージ反応
8 : 強制演出
```

のように用途を決められます。

数値そのものより、プロジェクト内での意味を揃えることが重要です。

## Request が受け付けられない場合

Request は常に成功するとは限りません。

現在の実装では、例えば次のような場合に Request は受け付けられません。

- 使用できない最低Priority以下を指定した
- Actor が停止している
- Actor が Request を拒否している
- 同じ Priority がすでに使われている
- 指定した Action が存在しない

通常の Mana スクリプトでは Request の成否を直接戻り値として受け取る構文ではありませんが、実行モデルを理解するうえでは「Request は依頼であり、必ず新しい Action が開始されるとは限らない」と覚えておくとよいでしょう。

## `sender`

Request を受けた Action では、誰がその Request を送ったかを `sender` から参照できます。

```mana
actor Guard
{
    action talk
    {
        if (sender == Guide)
        {
            print("Guide requested talk\n");
        }
    }
}
```

これにより、同じ Action でも Request 元に応じて動作を変えられます。

## Request と待機

用途によって、Request 系の命令を使い分けます。

| 構文 | 呼び出し側の動作 |
| --- | --- |
| `request` | 依頼したら先へ進む |
| `awaitStart` | 指定PriorityのActionが実行可能な段階まで待つ |
| `awaitCompletion` | 指定PriorityのActionが完了するまで待つ |
| `join` | 対象ActorのPriorityが指定値以下になるまで待つ |

`request` は Actor 同士を疎結合に連携させたい場合、`awaitCompletion` はイベントの順番を明確にしたい場合に向いています。ただし、await 系は要求が受理されなければ待たずに進みます。受理後も対象 Actor の Priority を条件に待つため、正確な解除条件は [Request リファレンス](../reference/reference-request.md)で確認してください。

## Priority は Actor ごとに管理される

Priority はゲーム全体で一つの実行順位表を作るものではありません。

それぞれの Actor が、自分に届いた Request と現在の Priority を管理します。

```text
Guide
priority 3 : talk

Guard
priority 5 : damage
priority 1 : patrol

Gate
priority 2 : open
```

各 Actor は独立した実行状態を持ち、VM がそれらを順番に進めます。

そのため「Guard の Priority 5 が Guide の Priority 3 より先に実行される」という単純な全体順位ではありません。

Priority は **同じ Actor の中で Action をどう割り込み・保留するか**を決めるものです。

## Priority を使う理由

ゲームでは、現在の行動を中断してでも優先したい処理があります。

例えば、

```text
歩く
  ↓
敵を見つける
  ↓
戦う
  ↓
ダメージを受ける
  ↓
戦闘へ戻る
```

これを大量の状態分岐だけで表現すると、元の処理へ戻る管理が複雑になります。

Mana では Priority ごとに実行状態を保持することで、割り込み前の Action へ戻れる構造を持っています。

## ここまでで覚えておきたいこと

- Request は Actor に Action の実行を依頼する
- 通常の `request` は終了を待たない
- Priority は大きいほど高い
- 高い Priority は現在の Action に割り込める
- 低い Priority は後で実行するために保持される
- 同じ Actor の同じ Priority には複数 Request を保持できない
- Priority は Actor ごとに管理される
- Request 元は `sender` から参照できる

## 次に読む

次は、これらの Request と Priority が Mana VM の中でどのように実行されるのかを全体像として整理します。

[Mana の実行モデル](./concept-execution-model.md)
