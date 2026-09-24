# Namespace と using

`namespace` は、Actor、Module、Struct、Function、変数、定数などの名前を階層化して整理するための仕組みです。

## namespace

```mana
namespace Game::AI
{
    actor Enemy
    {
        action think
        {
        }
    }
}
```

完全修飾名は `::` で区切ります。

```mana
Game::AI::Enemy
```

`::` は namespace の名前を修飾する演算子です。Action 参照で使う `->` とは役割が異なります。

```mana
request(1, Game::AI::Enemy->think);
```

## using で namespace を探索対象へ追加する

```mana
using Game::AI;

actor Controller
{
    action main
    {
        request(1, Enemy->think);
    }
}
```

`using Game::AI;` により、未修飾名 `Enemy` の解決候補として `Game::AI::Enemy` が追加されます。

## Actor / Module を using する

現行実装では `using` の対象として namespace だけでなく Actor / Module も解決できます。

```mana
namespace Game::AI
{
    actor Enemy
    {
        action think
        {
        }
    }
}

using Game::AI::Enemy;
```

この場合、最後の名前 `Enemy` がエイリアスとして現在のスコープへ追加されます。

`using` のシンボル対象は現行実装では Actor / Module に限定されています。Struct や通常Functionなどを同じ方式で `using` する構文としては扱いません。

## スコープ

`using` は、その宣言が属する namespace スコープの名前解決へ影響します。

namespace を抜けると、その内側で追加した `using` スコープも終了します。

## 前方参照

Mana はパース後に全体のシンボルと namespace を解析するため、後方で定義される namespace や Actor / Module を `using` から参照できます。

```mana
using Game::AI;

actor Controller
{
    action main
    {
        request(1, Enemy->think);
    }
}

namespace Game::AI
{
    actor Enemy
    {
        action think
        {
        }
    }
}
```

## 曖昧な名前

複数の候補が同じ未修飾名として見つかる場合、コンパイラは曖昧な参照としてエラーにします。

代表的な診断には次があります。

- `ambiguous using`
- `ambiguous symbol reference`
- `ambiguous type reference`
- `ambiguous actor reference`
- `unresolved using`

曖昧になる場合は完全修飾名を使用してください。

## ファイルと namespace は別の概念

ソースファイルを分けても、自動的に namespace が作られるわけではありません。

- ファイル: ソースコードを物理的に整理する単位
- namespace: 名前を論理的に整理する単位

複数ファイルを一つの Program Image へまとめながら、namespace で名前の衝突を避けられます。

## 関連項目

- [ソースファイルと import / include](./reference-source-files.md)
- [Module](./reference-module.md)
- [Request](./reference-request.md)
