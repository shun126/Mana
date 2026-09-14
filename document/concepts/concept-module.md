# Module

`module` は、複数の Actor で再利用したい Action やメンバー定義をまとめるための仕組みです。

Mana では、Actor ごとに似た Action を何度も書くより、共通部分を Module として定義し、必要な Actor から `extend` して利用できます。

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

この例では、`Villager` が `CommonActions` の定義を取り込みます。

## Module の役割

Module は、実行主体そのものではありません。

`actor` のように VM 起動時にインスタンスが作られて自分で Action を実行するのではなく、Actor に共通機能を与えるための再利用単位です。

概念的には次のように考えられます。

```text
module CommonActions
        |
        | extend
        v
actor Villager

module CommonActions
        |
        | extend
        v
actor Guard
```

一つの Module を複数の Actor から利用できます。

## 継承より「部品の再利用」と考える

`extend` という名前から、C++ や Java のクラス継承を想像するかもしれません。

しかし、Mana の Module はクラス階層を作るための仕組みとして考えるより、**Actor に共通の定義を追加する部品**として理解する方が適切です。

例えば、

- 会話用 Action
- 共通リアクション
- 共通の待機処理
- 複数種類の NPC が共有する振る舞い

などを Module にまとめられます。

## namespace と組み合わせられる

Module は namespace 内にも定義できます。

```mana
namespace Game::NPC
{
    module Talkable
    {
        action talk
        {
            print("Hello\n");
        }
    }
}
```

完全修飾名で利用できます。

```mana
actor Villager
{
    extend Game::NPC::Talkable;
}
```

`using` で名前空間を探索対象にした場合は、短い名前でも参照できます。

```mana
using Game::NPC;

actor Villager
{
    extend Talkable;
}
```

現行コンパイラでは、このような namespace / `using` を通した Module の名前解決も行われます。

## Actor と Module の違い

| | Actor | Module |
| --- | --- | --- |
| 実行主体になる | はい | いいえ |
| VM 起動時に Actor として生成される | はい | いいえ |
| Action を定義できる | はい | はい |
| `extend` される側になる | 通常は使わない | はい |
| 主な目的 | 独立した実行単位 | 共通定義の再利用 |

## Module を使う判断

複数の Actor が同じ意味の振る舞いを持つときに、Module は有効です。

一方、単にコードが少し似ているだけなら、必ずしも Module に分ける必要はありません。

「この振る舞いは複数の Actor に共通する一つの役割か」という観点で判断すると整理しやすくなります。

## 同名定義について

Actor 自身と Module の両方に同じ名前の Action やメンバーを定義した場合の詳細な解決規則は、Concepts では扱いません。

再利用コードでは同名定義に依存せず、役割が分かる名前に分けることを推奨します。正確な衝突規則は Language Reference で扱います。

## まとめ

- Module は Actor に共通機能を与える再利用単位
- Module 自体は実行主体ではない
- Actor から `extend ModuleName;` で利用する
- クラス継承より、Actor に加える「部品」と考えると理解しやすい
- namespace / `using` と組み合わせて整理できる

次は、Actor の定義を実行時に生成するための **Phantom** を説明します。
