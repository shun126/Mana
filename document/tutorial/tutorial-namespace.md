# namespace で名前を整理する

複数の Actor や struct を作っていくと、同じ名前を使いたくなることがあります。

例えば、町の警備を担当する `Guard` と、ダンジョンの敵として登場する `Guard` を別のものとして扱いたい場合です。

Mana では **namespace** を使って名前をグループ化できます。

## このページで分かること

- `namespace` は名前の衝突を避けるための仕組みである
- `::` で namespace を含む名前を指定できる
- `using` を使うと namespace 内の名前を短く書ける
- `->` と `::` は役割が異なる
- namespace や `using` は前方参照できる
- ファイル分割と namespace は別の仕組みである

## namespace を作る

次の例では、`Game::AI` namespace の中に `Enemy` Actor を定義しています。

```mana
namespace Game::AI
{
    actor Enemy
    {
        action think
        {
            print("Enemy: think\n");
        }
    }
}
```

この `Enemy` の完全な名前は、

```text
Game::AI::Enemy
```

です。

`::` は namespace の階層を区切ります。

## 完全な名前で参照する

namespace の外側からは、完全な名前を使って Actor を参照できます。

```mana
const int kThinkPriority = 1;

namespace Game::AI
{
    actor Enemy
    {
        action think
        {
            print("Enemy: think\n");
        }
    }
}

actor Controller
{
    action main
    {
        request(kThinkPriority, Game::AI::Enemy->think);
    }
}
```

ここでは、

```mana
Game::AI::Enemy->think
```

と書いています。

読み方を分解すると、

```text
Game::AI::Enemy -> think
└──── Actor ────┘   └Action
```

となります。

`::` は namespace の名前をたどるために使い、`->` は Actor の Action を指し示すために使います。

この二つは役割が違います。

## 同じ Actor 名を使える

namespace を分けると、同じ短い名前を別の用途で使用できます。

```mana
namespace Town
{
    actor Guard
    {
        action talk
        {
            print("Town Guard\n");
        }
    }
}

namespace Dungeon
{
    actor Guard
    {
        action talk
        {
            print("Dungeon Guard\n");
        }
    }
}
```

それぞれは、

```text
Town::Guard
Dungeon::Guard
```

という別の名前です。

使う側でどちらなのか明示できます。

```mana
request(1, Town::Guard->talk);
request(1, Dungeon::Guard->talk);
```

## `using` で短く書く

毎回完全な namespace を書くと、コードが長くなることがあります。

```mana
request(1, Game::AI::Enemy->think);
```

そこで `using` を使えます。

```mana
using Game::AI;
```

こうすると、そのスコープでは `Game::AI` 内の名前を未修飾名で探索できるようになります。

```mana
const int kThinkPriority = 1;

namespace Game::AI
{
    actor Enemy
    {
        action think
        {
            print("Enemy: think\n");
        }
    }
}

using Game::AI;

actor Controller
{
    action main
    {
        request(kThinkPriority, Enemy->think);
    }
}
```

`Enemy` とだけ書いていますが、`using Game::AI;` によって `Game::AI::Enemy` を見つけられます。

## namespace 全体ではなく Actor を指定する `using`

`using` には、Actor や module を名前付きで取り込む使い方もあります。

```mana
using Game::AI::Enemy;
```

この場合は `Enemy` という短い名前でその Actor を参照できます。

```mana
using Game::AI::Enemy;

actor Controller
{
    action main
    {
        request(1, Enemy->think);
    }
}
```

チュートリアルの段階では、次の二つを覚えておけば十分です。

```mana
using Game::AI;        // namespace を探索対象へ追加
using Game::AI::Enemy; // Actor を短い名前で参照
```

`using` の厳密な名前解決規則や module への適用は Language Reference で扱います。

## `using` を使いすぎない

`using` は便利ですが、多くの namespace を同時に追加すると、どの名前を指しているのか分かりにくくなることがあります。

例えば、

```mana
using Town;
using Dungeon;
```

の両方に `Guard` が存在すると、

```mana
Guard->talk
```

だけでは一つに決められません。

このような場合は、完全な名前を使って意図を明確にします。

```mana
Town::Guard->talk
Dungeon::Guard->talk
```

短く書くことより、どの Actor を使っているか分かりやすいことを優先してください。

## namespace は階層化できる

namespace は複数の階層を持てます。

```mana
namespace Game::Character::Enemy
{
    actor Goblin
    {
        action attack
        {
        }
    }
}
```

完全な名前は、

```text
Game::Character::Enemy::Goblin
```

です。

プロジェクトが大きくなったときに、機能や用途ごとに名前を整理できます。

ただし、最初から深い階層を作る必要はありません。必要になった段階で整理する方が扱いやすいでしょう。

## namespace とファイルは独立している

前のページで説明したファイル分割と namespace は別の仕組みです。

例えば `enemy.mn` というファイルに、

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

と書くこともできます。

別の `controller.mn` では、

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

と書けます。

それらを入口ファイルから読み込みます。

```mana
import "enemy.mn";
import "controller.mn";
```

ファイルはコードを物理的に整理し、namespace はManaの名前を論理的に整理します。

## namespace と `using` も前方参照できる

Mana は読み込んだソース全体をパースした後で名前を解決します。

そのため、`using` より後に namespace が定義されていても解決できます。

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

これはコンパイル時にソース全体から名前を解決できるためです。

ただし、読みやすさのために常に前方参照を使う必要はありません。「定義順に縛られずファイルを整理できる」という利点として考えるとよいでしょう。

## ゲームでの分け方の例

例えば、次のような namespace を考えられます。

```text
Game
├─ Character
│  ├─ NPC
│  └─ Enemy
├─ Event
└─ Gimmick
```

Mana では、例えば次のように表せます。

```mana
namespace Game::Character::NPC
{
    actor Shopkeeper
    {
        action talk
        {
        }
    }
}

namespace Game::Gimmick
{
    actor Door
    {
        action open
        {
        }
    }
}
```

これは一例です。namespace の設計はゲーム側の責務や規模に合わせて決めてください。

## ここまでで覚えておきたいこと

- `namespace` は名前をグループ化して衝突を避ける
- namespace の区切りには `::` を使う
- Action の参照には `->` を使う
- `using Namespace;` で、その namespace を名前探索の対象にできる
- `using Namespace::Actor;` で Actor を短い名前から参照できる
- 名前が曖昧になる場合は完全な名前を書く
- namespace / `using` は前方参照できる
- ファイル分割と namespace は別の仕組みである

## 次に学ぶこと

ここまでで、Mana の基本構文、Actor 間の連携、Priority と同期、複数ファイル、namespace まで扱いました。

次は、これまで学んだ内容を組み合わせて **小さなゲームイベント** を完成させます。
