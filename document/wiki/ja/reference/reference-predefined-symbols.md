# 定義済みシンボル

Mana には、実行中の Actor や Request の文脈を参照するための定義済みシンボルがあります。

## 一覧

| 名前 | 型 / 種類 | 意味 |
| --- | --- | --- |
| `self` | `actor` | 現在実行している Actor |
| `sender` | `actor` | 現在の Action を Request した Actor |
| `priority` | `int` | 現在実行中の Action の Priority |
| `this` | Struct の受信側 | Struct メンバーFunctionで現在の Struct インスタンスを参照するための予約語 |
| `Nil` | `Nil` | 空の参照を表す特殊値 |

## self

```mana
actor Worker
{
    action main
    {
        request(1, self->update);
    }

    action update
    {
    }
}
```

`self` は現在の Actor を表します。

Action や Actor 上で実行される Function から、自分自身へ Request を送る場合などに使用できます。

## sender

```mana
actor Receiver
{
    action receive
    {
        request(1, sender->reply);
    }
}
```

`sender` は、その Action を Request した Actor を表します。

ただし VM 自身が起動時に送る `init` / `main` などのシステム Request では、送信元 Actor が存在しません。`sender` が常に有効な Actor を指すことを前提にしないでください。

## priority

```mana
actor Worker
{
    action work
    {
        print("%d\n", priority);
    }
}
```

`priority` は現在実行している Action の Priority を `int` として取得します。

Request に指定した Priority と、現在どの割り込みレベルで動いているかを調べたい場合に使用します。

## this

`this` は Struct のメンバーFunctionで、現在の Struct インスタンスを参照するための予約語です。

コンパイラ内部では `this` という受信側識別子として解決されます。通常のグローバルFunctionやActionで一般的な Actor 自己参照として使うものではありません。Actor 自身を参照する場合は `self` を使用します。

## Nil

`Nil` は空の参照を表す特殊値です。

```mana
Nil
```

**先頭の `N` は大文字です。** 現行Lexerは `Nil` を予約語として認識し、`nil` は同じトークンとして扱いません。

`Nil` は通常の数値定数とは異なる専用型を持ち、定数式では使用できません。

## true / false

真偽値リテラルとして `true` と `false` も使用できます。

```mana
bool visible = true;
bool finished = false;
```

これらは `bool` 型のリテラルです。

## 関連項目

- [Actor](./reference-actor.md)
- [Action](./reference-action.md)
- [Request](./reference-request.md)
- [Struct](./reference-struct.md)
