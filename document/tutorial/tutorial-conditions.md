# 条件によって処理を変える

ゲームの処理では、「HP が 0 以下なら倒れる」「鍵を持っているなら扉を開く」のように、条件によって動作を変える場面が多くあります。

Mana では `if` を使って条件分岐を記述します。

## このページで分かること

- `if` で条件が成立したときだけ処理できる
- `else` で条件が成立しなかった場合を記述できる
- 比較演算子で値を比べられる
- `bool` を条件として使える
- `&&`、`||`、`!` で複数の条件を組み合わせられる

## `if` の基本

まずは HP を調べる例です。

```mana
actor Player
{
    action main
    {
        int hp = 20;

        if (hp > 0)
        {
            print("Player is alive.\n");
        }
    }
}
```

`hp > 0` が成立しているときだけ、`{` と `}` の中が実行されます。

## 値を比較する

よく使う比較は次の通りです。

| 書き方 | 意味 |
| --- | --- |
| `a == b` | a と b が等しい |
| `a != b` | a と b が等しくない |
| `a < b` | a が b より小さい |
| `a <= b` | a が b 以下 |
| `a > b` | a が b より大きい |
| `a >= b` | a が b 以上 |

`=` と `==` は意味が違います。

```mana
hp = 10;
```

は代入です。

```mana
hp == 10
```

は比較です。

初めのうちは、この2つの書き間違いに注意してください。

## `else` を使う

条件が成立しなかった場合の処理は `else` で書けます。

```mana
actor Door
{
    action main
    {
        bool hasKey = true;

        if (hasKey)
        {
            print("The door opens.\n");
        }
        else
        {
            print("The door is locked.\n");
        }
    }
}
```

`hasKey` が `true` なら最初のブロック、`false` なら `else` 側のブロックが実行されます。

## `bool` は true / false を表す

`bool` は「はい / いいえ」「ON / OFF」のような2つの状態を扱う型です。

```mana
bool isOpen = false;
bool hasKey = true;
```

`if` の条件には `bool` をそのまま使用できます。

```mana
if (hasKey)
{
    print("You have the key.\n");
}
```

逆の条件を調べたいときは `!` を使えます。

```mana
if (!hasKey)
{
    print("You need a key.\n");
}
```

## 複数の条件を組み合わせる

「両方が成立している」ことを調べる場合は `&&` を使います。

```mana
if (hasKey && isDoorEnabled)
{
    print("The door opens.\n");
}
```

「どちらかが成立している」ことを調べる場合は `||` を使います。

```mana
if (hasKey || isDebugMode)
{
    print("The door opens.\n");
}
```

入門では次の3つを覚えておけば十分です。

| 演算子 | 意味 |
| --- | --- |
| `&&` | 両方が成立 |
| `||` | どちらかが成立 |
| `!` | 条件を反転 |

## ゲームイベントの状態で分岐する

Actor が持つ変数と `if` を組み合わせると、状態によって Action の処理を変えられます。

```mana
actor NPC
{
    int mTalkCount;

    action main
    {
        mTalkCount = 2;

        if (mTalkCount == 0)
        {
            print("NPC: Nice to meet you.\n");
        }
        else
        {
            print("NPC: Hello again.\n");
        }
    }
}
```

この考え方は、会話イベント、クエスト進行、敵 AI、ギミックなど多くのゲーム処理で使えます。

## `switch` について

Mana には `switch` / `case` もあります。

状態の種類が多い場合には便利ですが、最初は `if` / `else` だけで十分です。`switch` の詳しい構文は Language Reference で扱います。

## ここまでで覚えておきたいこと

- `if` は条件が成立したときだけ処理する
- `else` は条件が成立しなかった場合の処理を書く
- `==` は比較、`=` は代入
- `bool` は `true` / `false` を扱う
- `&&`、`||`、`!` で条件を組み合わせられる

## 次に読む

次は [処理を繰り返す](./tutorial-loops.md) で、同じ処理を複数回実行する方法を学びます。
