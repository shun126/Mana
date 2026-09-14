# 変数

変数は値を保持するための名前です。

## 宣言

```mana
int count;
float speed;
bool opened;
```

初期値を同時に指定できます。

```mana
int count = 0;
float speed = 1.5;
bool opened = false;
```

## 代入

```mana
count = 10;
speed = 2.0;
```

複合代入も使用できます。

```mana
count += 1;
speed *= 2.0;
```

## グローバル変数

Actor や Function の外側で宣言します。

```mana
int gScore = 0;
```

グローバル変数の初期化は、通常の Actor の `init` より前に実行されます。

## Actor 変数

Actor の中、Action の外側へ変数を宣言する構文があります。ただし、2026-09-16 に現行ソースからビルドした処理系では、以下のように Action 内から変数名を参照すると `incomplete type name 'mOpened'` でコンパイルに失敗することを確認しています。

以下は現行制約を示す例で、そのまま実行できる完成例ではありません。

```mana
actor Door
{
    bool mOpened;

    action init
    {
        mOpened = false;
    }
}
```

Actor の状態を Action 間で保持する設計ですが、現時点の実行教材では [グローバル変数を一つの Actor が管理する例](../tutorial/tutorial-variables.md)を使っています。

## ローカル変数

Action や Function のブロック内で宣言します。

```mana
actor Counter
{
    action main
    {
        int count = 0;
        count = count + 1;
    }
}
```

ローカル変数は、その処理中に使う一時的な値です。

## Struct のメンバー

```mana
struct Position
{
    float x;
    float y;
}

actor PositionExample
{
    action main
    {
        Position p;
        p.x = 10.0;
        p.y = 20.0;
    }
}
```

## 固定長配列

変数宣言では固定長配列を使用できます。

```mana
actor ArrayExample
{
    action main
    {
        int values[4];
        values[0] = 10;
        values[1] = 20;
    }
}
```

配列サイズには正の整数リテラル、または整数定数の名前を指定できます。

```mana
const int kValueCount = 4;

actor ArrayWithConstant
{
    action main
    {
        int values[kValueCount];
    }
}
```

宣言子には複数の `[]` を続けて記述することもできます。

```mana
int grid[4][8];
```

配列要素は `[]` で参照します。

```mana
values[index]
```

実行時に決まる添字が配列範囲外になった場合、現行VMは `ScriptError` としてその Actor を停止し、範囲外アクセスを続行しません。

## `allocate` と `static`

トップレベルでは、VM の変数メモリを明示的に構成するための `allocate` と `static` を使用できます。通常のゲームロジックより低レベルな機能です。

### `allocate`

```mana
allocate 1024
{
    int gReservedValue;
}
```

`allocate N { ... }` はグローバル変数領域にサイズを明示した領域を確保し、その中へ変数を配置します。`N` はバイト数を表す整数リテラルです。

### `static`

```mana
static
{
    float gStaticValue;
}
```

`static { ... }` の変数は、通常のグローバル変数とは別の VM static 変数領域へ配置されます。

サイズを明示する形式もあります。

```mana
static allocate 512
{
    int gStaticReservedValue;
}
```

現行コンパイラは、明示した領域に宣言された変数が収まるかを検査します。

Mana の `static` は VM 内の static 変数領域を選ぶ構文です。C++ の `static` と同じ意味やリンケージ規則を持つものとして解釈しないでください。

## 主なスコープ

| 宣言場所 | 主な用途 |
| --- | --- |
| グローバル | プログラム全体で共有する値 |
| Actor 内 | Actor ごとの状態 |
| Action / Function 内 | 一時的な値 |
| Struct 内 | Struct のメンバー |
| `static` ブロック | VM の static 変数領域 |

変更しない値には `const` を使用します。

## 関連項目

- [型](./reference-types.md)
- [定数](./reference-constants.md)
- [式](./reference-expressions.md)
- [Struct](./reference-struct.md)
- [Error Handling](../integration/integration-error-handling.md)
