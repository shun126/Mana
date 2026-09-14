# 関数で処理をまとめる

同じ計算や処理を何度も使う場合は、関数としてまとめると読みやすくなります。

Mana の関数は、通常のプログラミング言語と同じように、引数を受け取り、必要なら戻り値を返せます。

## このページで分かること

- 関数は処理を名前付きでまとめる
- 引数を使って値を受け取れる
- `return` で結果を返せる
- 戻り値が不要な関数には `void` を使う
- Function と Action は役割が異なる

## 関数を作る

まず、受け取った値を2倍にする関数を作ってみます。

```mana
int doubleValue(int value)
{
    return value * 2;
}
```

最初の `int` は、この関数が整数を返すことを表します。

`doubleValue` が関数名です。

括弧の中の、

```mana
int value
```

は、関数が受け取る値です。これを **引数** と呼びます。

## 関数を呼び出す

関数は名前の後ろに括弧を書いて呼び出します。

```mana
int result = doubleValue(10);
```

`doubleValue(10)` の結果は `20` です。

Actor から使うと、次のように書けます。

```mana
int doubleValue(int value)
{
    return value * 2;
}

actor Calculator
{
    action main
    {
        int result = doubleValue(10);
        print("result: %d\n", result);
    }
}
```

実行結果は次のようになります。

```text
result: 20
```

## 複数の引数を受け取る

引数は複数指定できます。

```mana
int add(int a, int b)
{
    return a + b;
}
```

呼び出すときは、定義した順番で値を渡します。

```mana
int result = add(10, 20);
```

この場合、`result` は `30` になります。

## 戻り値を返す `return`

値を返す関数では `return` を使います。

```mana
int getMax(int a, int b)
{
    if (a > b)
    {
        return a;
    }

    return b;
}
```

`return` が実行されると、その関数の処理はそこで終了し、呼び出し元へ値を返します。

## 値を返さない関数

結果を返す必要がない場合は、戻り値の型に `void` を指定します。

```mana
void printSeparator()
{
    print("-----\n");
}
```

呼び出し方は同じです。

```mana
printSeparator();
```

処理を共通化したいだけなら `void` 関数が便利です。

## Function と Action の違い

Mana では、Function と Action を分けて考えることが重要です。

関数は、今実行している処理の中から普通に呼び出し、終了したら呼び出し元へ戻ります。

```mana
int result = add(10, 20);
```

一方 Action は Actor が持つ行動で、Request の対象になります。

```mana
request(1, NPC->talk);
```

大まかには次のように考えられます。

| Function | Action |
| --- | --- |
| 共通処理や計算をまとめる | Actor の行動を表す |
| 通常の呼び出しで実行する | Request できる |
| 呼び出し元へ戻る | Actor の実行モデルと Priority の影響を受ける |
| 値を返せる | Actor 間の連携に使う |

この違いは Mana を使う上で重要です。

例えば、HP の計算は Function にまとめ、NPC に会話を始めてもらう処理は Action として Request する、といった分け方ができます。

## どこまでを関数にするか

短い処理を何でも関数にする必要はありません。

次のような処理は関数にまとめる候補になります。

- 同じ計算を複数箇所で使う
- 一つの処理に名前を付けると意味が分かりやすくなる
- Action の中が長くなり、細かい処理を分離したい

ただし、Actor 同士の行動連携を単なる関数呼び出しへ置き換えてしまうと、Mana の Actor / Action / Request という構造が見えにくくなります。

「Actor の行動」は Action、「その行動の中で使う共通処理」は Function、という分け方から始めると理解しやすいでしょう。

## `native` 関数について

Mana には、C++ 側に実装された処理を呼び出す `native` 関数もあります。

```mana
native void playSound(int id);
```

ゲームエンジンとの連携に重要な機能ですが、通常関数とは実装場所が異なります。`native` は Integration と Language Reference で詳しく扱います。

## ここまでで覚えておきたいこと

- 関数は処理を名前付きでまとめる
- 引数で値を渡せる
- `return` で戻り値を返せる
- 戻り値が不要なら `void` を使う
- Function は通常の処理呼び出し、Action は Actor の行動
- Actor 間の連携には Action / Request を使う

## 次に学ぶこと

ここまでで、変数・条件分岐・繰り返し・関数という基本的なプログラミング要素がそろいました。

次は Mana 固有の実行モデルへ戻り、**Priority** が Request と Action の実行にどのように影響するかを学びます。
