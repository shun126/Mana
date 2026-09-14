# 変数で状態を覚える

ここでは、Mana で値を覚えておくための **変数** を学びます。

## このページで分かること

- 変数は値を保存するための名前付きの領域である
- 変数には型がある
- Action の中で使うローカル変数と、Actor が保持する変数がある
- `=` で値を代入できる
- 宣言と同時に初期値を設定できる

## 変数とは

ゲームでは、現在の HP、会話回数、フラグ、残り時間など、途中で変化する値を扱います。

その値を保存しておくものが変数です。

```mana
int hp = 100;
```

この文は、`int` 型の `hp` という変数を作り、最初の値として `100` を入れています。

`int` は整数を扱う型です。

## 値を変更する

変数の値は後から変更できます。

```mana
int hp = 100;
hp = 80;
```

右側の値を左側の変数へ入れる操作を **代入** と呼びます。

計算した結果を代入することもできます。

```mana
int hp = 100;
int damage = 20;

hp = hp - damage;
```

この結果、`hp` は `80` になります。

## Action の中で使うローカル変数

Action の中で宣言した変数は、その処理のために使うローカル変数です。

```mana
actor PlayerStatus
{
    action main
    {
        int hp = 100;
        int damage = 20;

        hp = hp - damage;
        print("HP: %d\n", hp);
    }
}
```

実行結果は次のようになります。

```text
HP: 80
```

ここで `hp` と `damage` は `main` Action の中で使う変数です。

Action の外からこのローカル変数を直接参照することはできません。

## Actor が状態を持つ

Actor 自身に値を覚えさせたい場合は、Action の外、Actor の中に変数を宣言します。

```mana
actor PlayerStatus
{
    int mHp;

    action main
    {
        mHp = 100;
        mHp = mHp - 20;

        print("HP: %d\n", mHp);
    }
}
```

`mHp` は `PlayerStatus` Actor が持つ変数です。

このような変数は、同じ Actor の複数の Action から参照できます。

ゲームでは、例えば次のような状態を Actor に持たせることができます。

```text
NPC
 ├─ mTalkCount
 ├─ mIsAngry
 └─ mEventState
```

Actor が自分に関係する状態を持つことで、処理の責任範囲を分かりやすくできます。

## よく使う型

Mana には複数のデータ型があります。

入門では、まず次の型を覚えれば十分です。

| 型 | 用途の例 |
| --- | --- |
| `int` | HP、回数、番号 |
| `float` | 距離、時間、割合 |
| `bool` | ON / OFF、成立 / 不成立 |
| `string` | 文字列 |

例えば次のように宣言できます。

```mana
int count = 3;
float distance = 10.5;
bool enabled = true;
string message = "Hello";
```

Mana には `char` や `short` など他の型もあります。型の完全な一覧は Language Reference で扱います。

## 変数名には役割が分かる名前を付ける

次のような名前より、

```mana
int a;
int b;
```

次のように意味が分かる名前を付けた方が読みやすくなります。

```mana
int hp;
int damage;
```

Actor が保持する状態については、このプロジェクトのコーディング規約では `m` 接頭辞を推奨しています。

```mana
int mHp;
int mTalkCount;
bool mIsOpen;
```

これは Mana 言語そのものの制約ではなく、読みやすくするための命名規則です。

## グローバル変数について

Actor の外にも変数を宣言できます。

```mana
int gEventCount;
```

このような変数は複数の Actor から参照できますが、誰が値を変更したのか分かりにくくなりやすいため、入門ではなるべく Actor の状態として持たせることをおすすめします。

グローバル変数や `static`、メモリ配置の詳しい仕様は Language Reference で扱います。

## ここまでで覚えておきたいこと

- 変数は値を保存する
- 変数には型がある
- `=` で値を代入する
- Action 内の変数はローカル変数
- Actor 内に宣言した変数は Actor の状態として使える
- 状態はできるだけ、その状態を管理する Actor に持たせる

## 次に読む

次は [条件によって処理を変える](./tutorial-conditions.md) で、変数の値に応じて処理を分岐させます。
