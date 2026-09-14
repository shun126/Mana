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

Actor の中、Action の外側で宣言した変数は、その Actor が保持する値になります。

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

Actor 変数は Action が終了しても保持されます。

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

Position p;
p.x = 10.0;
p.y = 20.0;
```

## 主なスコープ

| 宣言場所 | 主な用途 |
| --- | --- |
| グローバル | プログラム全体で共有する値 |
| Actor 内 | Actor ごとの状態 |
| Action / Function 内 | 一時的な値 |
| Struct 内 | Struct のメンバー |

変更しない値には `const` を使用します。

## 関連項目

- [型](./reference-types.md)
- [定数](./reference-constants.md)
- [式](./reference-expressions.md)
