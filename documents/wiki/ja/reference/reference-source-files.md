# ソースファイルと import / include

Mana のソースファイルは通常 `.mn` 拡張子を使用します。

大きなプログラムでは、`import` または `include` を使って複数のソースファイルを一つのコンパイルへ取り込めます。

## import

```mana
import "npc.mn";
```

`import` は指定したソースを読み込みます。同じ解決済みパスのソースがすでに読み込まれている場合、現行Lexerは2回目以降の読み込みを省略します。

そのため、通常のファイル分割には `import` を推奨します。

```text
main.mn
 ├─ import "npc.mn"
 └─ import "event.mn"
```

取り込まれた定義は同じコンパイル結果にまとめられ、一つの Program Image が生成されます。

## include

```mana
include "common.mn";
```

`include` も指定したソースを読み込みますが、`import` と異なり重複読み込みの抑止を行いません。

同じファイルを複数回 `include` すると、同じ宣言を複数回解析することになり、重複定義エラーの原因になる場合があります。

通常は `import` を使用し、同じソースを意図的に再読込する必要がある場合だけ `include` を検討してください。

## パスの解決

読み込み対象のパスは SourceResolver によって解決されます。

標準のファイルベースの利用では、現在読み込んでいるソースファイルの場所を基準に相対パスを解決できます。

```text
project/
├─ main.mn
└─ actors/
   └─ npc.mn
```

```mana
import "actors/npc.mn";
```

組み込み時に独自 SourceResolver を使う場合、実際のパス解決規則はその実装に依存します。

## 前方参照

Mana は取り込まれたソースを含む構文木を作成した後、シンボルと namespace のセマンティック解析を行います。

そのため、Actor、Module、namespace などは別ファイルに分かれていても、解析可能な名前であれば定義順だけを理由に参照できなくなる設計ではありません。

```mana
// main.mn
import "enemy.mn";

actor Controller
{
    action main
    {
        request(1, Enemy->think);
    }
}
```

```mana
// enemy.mn
actor Enemy
{
    action think
    {
    }
}
```

## namespace とは独立している

ファイル名やディレクトリ構造から namespace が自動生成されることはありません。

```text
actors/enemy.mn
```

というファイルを作っても、自動的に `actors::Enemy` になるわけではありません。

名前の論理的な整理には `namespace` を明示的に使用します。

## CLI の強制インクルード

`mana` コマンドには `-I filename` があり、ソースコードを書き換えずにコンパイル対象へファイルを強制追加できます。

```bash
mana -I common.mn main.mn
```

`-I` は複数回指定できます。

## 関連項目

- [Namespace と using](./reference-namespace.md)
- [CLI](./reference-cli.md)
- [ソースコードの構造](./reference-source-code.md)
