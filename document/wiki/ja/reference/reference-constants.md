# 定数

変更しない値には `const` を使用します。

## 構文

```text
const 型 名前 = 定数式;
```

例:

```mana
const int kMaxCount = 10;
const float kSpeed = 2.5;
const bool kDebug = false;
const string kMessage = "Hello";
```

## 代入はできない

`const` で宣言した名前へ後から代入することはできません。

```mana
const int kMaxCount = 10;

// エラー
kMaxCount = 20;
```

Compiler はこのような代入を診断します。

## 初期値は定数式

`const` の初期値には、コンパイル時に評価できる式を使用します。

```mana
const int kBase = 10;
const int kDouble = kBase * 2;
```

実行時にしか値が決まらない Function 呼び出しなどは、定数初期値として使用できません。

`Nil` は専用型であり、現行コンパイラでは定数式として使用できません。

## Priority に名前を付ける

Priority のような数値に意味のある名前を付ける用途に向いています。

```mana
const int kTalkPriority = 10;
const int kMovePriority = 5;
```

```mana
request(kTalkPriority, NPC->talk);
```

数値を直接書くより、用途が分かりやすくなります。

## 配列サイズに使う

整数定数は固定長配列のサイズにも使用できます。

```mana
const int kValueCount = 4;

actor ArrayExample
{
    action main
    {
        int values[kValueCount];
    }
}
```

## `define` / `undef` について

現在の Lexer には旧形式の `define` / `undef` トークンが残っていますが、現行 `Parser.yy` の宣言構文には組み込まれていません。

したがって、新しいドキュメントでは `define` / `undef` を現行の定数宣言構文として扱いません。新しいコードでは `const 型 名前 = 値;` を使用してください。

## 関連項目

- [型](./reference-types.md)
- [変数](./reference-variables.md)
- [式](./reference-expressions.md)
- [定義済みシンボル](./reference-predefined-symbols.md)
