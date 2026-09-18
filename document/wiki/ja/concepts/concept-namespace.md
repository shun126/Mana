# Namespace

`namespace` は、Actor、Module、struct などの名前を論理的に整理し、同じ短い名前が衝突するのを避けるための仕組みです。

Tutorial では書き方を学びました。ここでは、Mana における Namespace の役割を整理します。

## ファイルと Namespace は別のもの

ファイル分割は、ソースコードを物理的に整理する方法です。

Namespace は、プログラム中の名前を論理的に整理する方法です。

```text
ファイル
  -> どこにコードを書くか

namespace
  -> その名前がどの領域に属するか
```

例えば `npc.mn` に書かれているから自動的に `NPC` namespace に入るわけではありません。

## 名前に意味のある階層を与える

```mana
namespace Game::Town
{
    actor Guard
    {
    }
}

namespace Game::Dungeon
{
    actor Guard
    {
    }
}
```

どちらも短い名前は `Guard` ですが、完全な名前は異なります。

```text
Game::Town::Guard
Game::Dungeon::Guard
```

このように、同じ役割名を別の文脈で安全に使えます。

## `::` は名前の所属を表す

Mana では `::` を Namespace の修飾に使います。

```mana
request(10, Game::Town::Guard->talk);
```

一方、`->` は Actor と Action の関係を表します。

```text
Game::Town::Guard -> talk
^^^^^^^^^^^^^^^^^    ^^^^
Actor の名前         Action
```

この2つは役割が異なります。

## `using` は名前探索を補助する

完全修飾名を毎回書く代わりに、`using` で探索対象を追加できます。

```mana
using Game::Town;

actor EventController
{
    action main
    {
        request(10, Guard->talk);
    }
}
```

`using Game::Town;` は、未修飾名を解決するときに `Game::Town` も探索対象にします。

また、特定の Actor や Module を名前として取り込む使い方もできます。

```mana
using Game::Town::Guard;
```

現行実装では、`using` の対象として Namespace と Actor / Module が扱われます。

## 名前解決はパース後に行われる

Mana コンパイラはソースを読み込んだ後、セマンティック解析の段階で名前を解決します。

そのため、次のように `using` の後で Namespace を定義するコードも解決できます。

```mana
using Game::Town;

actor Controller
{
    action main
    {
        request(1, Guard->talk);
    }
}

namespace Game::Town
{
    actor Guard
    {
        action talk
        {
        }
    }
}
```

これは、単純に上から1行ずつ名前を確定しているわけではなく、コンパイル単位全体を見て意味を解析しているためです。

## 複数ファイルでも同じ名前空間を使える

Namespace は一つのファイルだけに閉じた仕組みではありません。

複数のソースから同じ論理的な Namespace を使うことで、大きなゲームを役割ごとに分割できます。

```text
character.mn  -> Game::Character
npc.mn        -> Game::Character::NPC
enemy.mn      -> Game::Character::Enemy
event.mn      -> Game::Event
```

ファイル構成と Namespace 構成を似せると分かりやすくなる場合はありますが、両者が同一である必要はありません。

## `using` を増やしすぎない

`using` はコードを短くできますが、複数の Namespace に同名シンボルがあると、名前が曖昧になることがあります。

その場合は完全修飾名で意図を明示します。

```mana
request(10, Game::Town::Guard->talk);
```

短さよりも、どの Actor を参照しているかが明確であることを優先してください。

## Namespace は実行単位ではない

Namespace 自体が VM 上で実行されたり、Actor のような状態を持ったりするわけではありません。

Namespace はあくまで **コンパイル時の名前整理と名前解決の仕組み** です。

この点は Actor や Module との重要な違いです。

## まとめ

- Namespace は名前を論理的に整理する
- ファイル分割とは別の仕組み
- `::` は Namespace の修飾に使う
- `->` は Actor の Action 参照に使う
- `using` は名前探索を補助する
- 名前解決はセマンティック解析で行われるため前方参照が可能
- 曖昧な場合は完全修飾名を書く

次は、Mana のソースがどのように実行されるかを **Compiler と VM** の関係から整理します。
