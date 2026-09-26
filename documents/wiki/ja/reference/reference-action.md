# Action

`action` は Actor が実行する処理単位です。

Function と異なり、Action は Request の対象になり、Priority に従って開始・中断・再開されます。

## 構文

```mana
action actionName
{
    statements
}
```

例:

```mana
actor NPC
{
    action talk
    {
        print("Hello\n");
    }
}
```

## 引数と戻り値

現行構文では Action は引数を持たず、戻り値も持ちません。

```mana
action talk
{
}
```

Actor 間で値を共有したい場合は、Actor の状態、グローバルデータ、Struct、Native Function などを用途に応じて使います。

## Action を実行する

Action は `request`、`awaitStart`、`awaitCompletion` などから実行できます。

```mana
request(1, NPC->talk);
```

`NPC->talk` は Action 参照です。

## `->` と `::`

Action 参照には `->` を使います。

```mana
NPC->talk
```

namespace の修飾には `::` を使います。

```mana
Game::NPC::Shopkeeper->talk
```

旧形式の `Actor::action` はコンパイラに互換構文として残っていますが、deprecated warning が出ます。新しいコードでは `->` を使用してください。

## `init` と `main`

`init` と `main` は VM 起動時に特別扱いされる Action 名です。

```mana
actor Example
{
    action init
    {
    }

    action main
    {
    }
}
```

現行VMは全 Actor に対して、`init` を最高優先度（2147483647）、`main` を Priority 0 で Request します。全 Actor の `init` 完了を待ってから `main` を開始する仕組みではありません。各 Actor は自分の `init` が終了すると、予約済みの Action を優先度順に実行します。

## Action 実行中の定義済み値

Action では、現在の実行状態を表す定義済み値を利用できます。

- `self` : 現在の Actor
- `sender` : この Action を Request した Actor
- `priority` : 現在実行中の Priority

例:

```mana
actor NPC
{
    action talk
    {
        print("priority = %d\n", priority);
    }
}
```

`sender` は Request の送信元を保持するため、どの Actor から実行を依頼されたかを識別する用途に使えます。

詳細は [定義済みシンボル](./reference-predefined-symbols.md) を参照してください。

## Priority と中断

Actor が Action を実行中に別の Request を受けると、Priority によって実行順が決まります。

- より高い Priority: 現在の Action に割り込む
- より低い Priority: 現在の Action が終わるまで保留される
- 同じ Priority: 現行VMでは同じ Priority の Request がすでに存在すると新しい Request は受理されない

Priority は数値が大きいほど高くなります。

詳細は [Request](./reference-request.md) と [実行制御](./reference-execution-control.md) を参照してください。

## Function との違い

| Action | Function |
| --- | --- |
| Actor の実行単位 | 通常のサブルーチン |
| Request の対象 | 通常の関数呼び出し |
| Priority を持つ | Priority を持たない |
| 中断・再開され得る | 呼び出し元の処理として実行 |
| 現行構文では引数・戻り値なし | 引数・戻り値を持てる |

## 関連項目

- [Actor](./reference-actor.md)
- [Function](./reference-functions.md)
- [Request](./reference-request.md)
- [実行制御](./reference-execution-control.md)
- [定義済みシンボル](./reference-predefined-symbols.md)
