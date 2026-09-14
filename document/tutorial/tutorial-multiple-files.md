# プログラムを複数のファイルに分ける

ここまでのチュートリアルでは、Actor や関数を一つの `.mn` ファイルに書いてきました。

小さなプログラムならそれでも問題ありませんが、ゲームイベントや NPC が増えると、一つのファイルだけでは見通しが悪くなります。

このページでは、Mana のプログラムを複数のソースファイルへ分ける方法を学びます。

## このページで分かること

- Mana のプログラムは複数の `.mn` ファイルへ分割できる
- 別のソースファイルは `import` または `include` で読み込む
- 普段は重複読み込みを防ぐ `import` を使うと安全である
- 読み込まれたソースは、一つのプログラムとしてまとめてコンパイルされる
- ファイルを分けても、それだけでは namespace は作られない
- Mana では前方参照を利用できる

## Actor を別のファイルへ移す

まず、次のような2ファイル構成を考えます。

```text
main.mn
npc.mn
```

`npc.mn` には NPC の Actor を定義します。

```mana
actor NPC
{
    action talk
    {
        print("NPC: Hello!\n");
    }
}
```

`main.mn` からこのファイルを読み込みます。

```mana
import "npc.mn";

const int kTalkPriority = 1;

actor Controller
{
    action main
    {
        request(kTalkPriority, NPC->talk);
    }
}
```

実行するファイルは `main.mn` です。

```bash
mana main.mn
```

`main.mn` をコンパイルするときに `npc.mn` も読み込まれるため、`Controller` から `NPC->talk` を Request できます。

## `import` の書き方

基本形は次の通りです。

```mana
import "filename.mn";
```

例えば、

```mana
import "npc.mn";
import "event/door.mn";
```

のように複数のファイルを読み込めます。

既定のファイル読み込みでは、相対パスは **その `import` を書いたファイルが置かれているディレクトリ** を基準に解決されます。

例えば、

```text
script/
├─ main.mn
└─ npc/
   └─ shopkeeper.mn
```

という配置なら、`main.mn` からは次のように書けます。

```mana
import "npc/shopkeeper.mn";
```

## `import` は同じソースを一度だけ読み込む

複数のファイルから同じ共通ファイルを利用することがあります。

```text
main.mn
├─ npc.mn
│  └─ common.mn
└─ door.mn
   └─ common.mn
```

このような場合でも `import` は、同じ解決先のソースを何度も読み込みません。

そのため、通常のソース分割では `import` を使うと扱いやすくなります。

## `include` との違い

Mana には `include` もあります。

```mana
include "common.mn";
```

`include` と `import` は、どちらも別のソースを現在のコンパイルへ取り込みます。

違いは **重複読み込みを防ぐかどうか** です。

| 書き方 | 同じソースを再び指定した場合 |
| --- | --- |
| `import "file.mn";` | 一度読み込んだソースは再び読み込まない |
| `include "file.mn";` | 指定されるたびに読み込む |

通常は `import` を使うことをおすすめします。

`include` は、同じソースを意図的に複数回展開する必要がある場合に使います。単に Actor、struct、関数、定数などを共有したいだけなら、重複定義を避けやすい `import` の方が安全です。

## 複数ファイルでも一つのプログラムになる

Mana では、読み込んだファイルを一つずつ別々のプログラムとして完成させてからリンクするわけではありません。

入口となるソースと、そこから `import` / `include` されたソースを読み込み、全体を一つのプログラムとして解析して Program Image を生成します。

```text
main.mn
   |
   +-- import npc.mn
   |
   +-- import door.mn
   |
   v
Mana Compiler
   |
   v
1つの Program Image
```

この考え方は、後で namespace や前方参照を理解するときにも重要です。

## 定義より先に名前を使える

Mana の名前解決は、ソース全体を読み込んだ後のセマンティック解析で行われます。

そのため、定義より前に名前を使う **前方参照** が可能です。

例えば、次のように `NPC` を先に参照しても、同じコンパイル対象の中で後から `NPC` が定義されれば解決できます。

```mana
const int kTalkPriority = 1;

actor Controller
{
    action main
    {
        request(kTalkPriority, NPC->talk);
    }
}

actor NPC
{
    action talk
    {
        print("Hello!\n");
    }
}
```

同じ考え方は、`import` された別ファイルとの間でも利用できます。

ただし、前方参照できるからといって、ファイルの依存関係を複雑にする必要はありません。共通定義、Actor、イベントなど、役割ごとに整理しておく方が読みやすくなります。

## ファイル名は名前空間ではない

ここは重要です。

例えば、

```text
npc/enemy.mn
npc/shopkeeper.mn
```

というディレクトリへ分けても、`npc` という Mana の namespace が自動的に作られるわけではありません。

ファイルと namespace は別の仕組みです。

- ファイル分割: ソースコードを管理しやすくする
- namespace: Mana のシンボル名を整理し、名前の衝突を避ける

大きなプロジェクトでは、この二つを組み合わせます。

## 役割ごとに分ける

最初は、次のような分け方で十分です。

```text
script/
├─ main.mn
├─ common.mn
├─ npc/
│  ├─ villager.mn
│  └─ guard.mn
└─ event/
   └─ gate.mn
```

`main.mn` を入口にするなら、例えば次のようにします。

```mana
import "common.mn";
import "npc/villager.mn";
import "npc/guard.mn";
import "event/gate.mn";
```

ファイル数が増えてきたら、次のページで扱う namespace を使って名前も整理します。

## ここまでで覚えておきたいこと

- Mana のプログラムは複数の `.mn` ファイルへ分割できる
- 別ファイルは `import` または `include` で読み込む
- `import` は同じソースの重複読み込みを防ぐ
- `include` は指定されるたびにソースを読み込む
- 読み込まれたソース全体から一つの Program Image が作られる
- 前方参照が可能である
- ファイルやディレクトリの名前は Mana の namespace にはならない

## 次に学ぶこと

次は、プログラムが大きくなったときに名前を整理するための **namespace と using** を学びます。
