# Module

`module` は、複数の Actor で再利用する Action やメンバー定義をまとめるための仕組みです。

Module 自体は実行主体ではありません。`actor` から `extend` することで、Module に含まれる定義を Actor 側で利用します。

## 構文

```mana
module CommonActions
{
    action greet
    {
        print("Hello\n");
    }
}

actor Villager
{
    extend CommonActions;
}
```

`extend` の基本形は次の通りです。

```mana
extend ModuleName;
```

## Module に書けるもの

現行文法では、Module の本体は Actor と同じ `actions` 文法を使用します。

そのため、Module には次のような定義を書けます。

- Action
- メンバー変数
- 定数
- `extend`

```mana
module Talkable
{
    int mTalkCount;

    const int kTalkPriority = 10;

    action talk
    {
        mTalkCount = mTalkCount + 1;
    }
}
```

## namespace 内の Module

Module は namespace 内に定義できます。

```mana
namespace Game::NPC
{
    module Talkable
    {
        action talk
        {
        }
    }
}
```

完全修飾名で指定できます。

```mana
actor Villager
{
    extend Game::NPC::Talkable;
}
```

`using` で namespace を探索対象へ追加した場合は、短い名前でも参照できます。

```mana
using Game::NPC;

actor Villager
{
    extend Talkable;
}
```

## Module は Actor ではない

Module は Program Image に定義情報として含まれますが、通常の Actor のように VM ロード時に実行主体として生成されません。

そのため、Module 自体へ `request` を送って独立実行させる用途ではなく、Actor へ共通定義を追加するために使用します。

## 名前の衝突

`extend` によって取り込まれる定義と Actor 側の定義で同名シンボルが発生した場合、コンパイラのシンボル解決規則の対象になります。

再利用用 Module では、同名定義による上書きを前提にせず、役割が明確に分かれる名前を使用することを推奨します。

## 関連項目

- [Actor](./reference-actor.md)
- [Action](./reference-action.md)
- [Namespace](./reference-namespace.md)
