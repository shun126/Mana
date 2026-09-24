# Request

Request は、Actor に対して Action の実行を要求する仕組みです。

Mana では、Actor 同士の協調を通常の Function 呼び出しだけで表すのではなく、Request と Priority を使って表現します。

## `request`

構文:

```mana
request(priority, actor_expression->actionName);
```

例:

```mana
request(10, NPC->talk);
```

第1引数は Priority、第2引数は Action 参照です。

Priority は値が大きいほど高くなります。

## Action 参照

推奨構文は `->` です。

```mana
NPC->talk
```

namespace を含む場合:

```mana
Game::NPC::Shopkeeper->talk
```

Actor 型の式も使用できます。

```mana
actor target;
request(1, target->talk);
```

現行Parserは `expression->actionName` を Action 参照として受け付けます。

旧形式:

```mana
NPC::talk
```

も互換構文として認識されますが、deprecated warning が出ます。新しいコードでは使用しないでください。

## `request` の動作

通常の `request` は Action の完了を待ちません。

```mana
request(10, NPC->talk);
print("continue\n");
```

Request を送った Actor は、そのまま後続処理を続けます。

対象 Actor 側では Priority によって処理されます。

- 要求 Priority が現在より高い: 現在の処理へ割り込む
- 要求 Priority が現在より低い: 後で実行するため保持する
- 要求 Priority が現在と同じ: その Priority の実行状態がすでに存在するため、新しい Request は受理されない

同じ Priority に複数の Action をキューとして積む仕組みではありません。

## Request が受理されない条件

現行VMの `Actor::Request` では、少なくとも次の場合に Request が失敗します。

- Priority が VM の最低割り込み Priority 以下
- 対象 Actor が halt 状態
- 対象 Actor が `refuse()` 状態
- 同じ Priority の Request がすでに存在する
- 指定した Action が存在しない

スクリプトの `request` 文自体には成否を返す戻り値はありません。

## `awaitStart`

```mana
awaitStart(priority, actor_expression->actionName);
```

Request を送り、対象 Actor がその Priority を開始できる状態になるまで呼び出し側を待機させます。

要求が受理された場合、対象 Actor の現在 Priority が要求 Priority 以下になった時点で待機を解除します。要求した Action の最初の文が実行済みであることまでは保証しません。

```mana
awaitStart(10, NPC->talk);
```

## `awaitCompletion`

```mana
awaitCompletion(priority, actor_expression->actionName);
```

Request を送り、その Priority の処理が完了するまで待機します。

要求が受理された場合、対象 Actor の現在 Priority が要求 Priority 未満になった時点で待機を解除します。要求ごとの完了通知を保持して待つ仕組みではありません。

```mana
awaitCompletion(10, NPC->talk);
```

## await の要求が受理されない場合

`awaitStart` と `awaitCompletion` は、最初の Request が受理されなければ待たずに次へ進みます。同じ Priority が空くまで再要求する命令ではありません。待機から戻ったことだけでは、要求した Action が実行されたことや、その行動が成功したことを保証しません。

## 自分自身への await

`awaitStart` と `awaitCompletion` で `self` を対象にすると、現行VMはスクリプトエラーにします。

```mana
awaitCompletion(10, self->talk); // error
```

自分自身を待機対象にすると、待機中の Actor 自身が進まなければ完了できないためです。

通常の `request` で自分自身へ Request することは可能です。

## `sender`

Request が受理されると、送信元 Actor は対象 Action の `sender` として記録されます。

```mana
actor NPC
{
    action talk
    {
        // sender はこの Action を Request した Actor
    }
}
```

VM 起動時のシステム Request など、送信元 Actor が存在しない場合もあります。

## `join` との違い

`join` は新しい Request を送りません。

```mana
join(0, NPC);
```

対象 Actor がすでに実行している処理の Priority を監視し、指定 Priority 以下になるまで待ちます。

| 命令 | 新しい Request | 待機 |
| --- | --- | --- |
| `request` | 送る | しない |
| `awaitStart` | 送る | 開始可能になるまで |
| `awaitCompletion` | 送る | 完了まで |
| `join` | 送らない | 既存 Priority が指定値以下になるまで |

## 関連項目

- [Actor](./reference-actor.md)
- [Action](./reference-action.md)
- [実行制御](./reference-execution-control.md)
- [Request と Priority](../concepts/concept-request-and-priority.md)
