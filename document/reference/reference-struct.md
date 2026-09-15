# Struct

`struct` は、複数の値と、それらを扱う Function を一つの型としてまとめるための機能です。

## 構文

```mana
struct TypeName
{
    members
}
```

例:

```mana
struct Status
{
    int hp;
    int mp;
}
```

変数として使います。

```mana
Status status;
status.hp = 100;
status.mp = 20;
```

## メンバー変数

Struct の中には変数を宣言できます。

```mana
struct CharacterData
{
    string name;
    int level;
    float speed;
    actor owner;
}
```

別の Struct をメンバーに持つこともできます。

```mana
struct Position
{
    float x;
    float y;
}

struct Unit
{
    Position position;
    int hp;
}
```

メンバー参照には `.` を使います。

```mana
Unit unit;
unit.position.x = 10.0;
unit.hp = 100;
```

## メンバー Function

Struct 内には通常の Function を定義できます。

```mana
struct Counter
{
    int value;

    void reset()
    {
        value = 0;
    }
}
```

呼び出し:

```mana
Counter counter;
counter.reset();
```

現行コンパイラでは、Struct のメンバー Function からも通常の Mana の処理を記述できます。例えば Actor を引数として受け取り、Request を送ることもできます。

```mana
struct Helper
{
    void call(actor target)
    {
        request(1, target->talk);
    }
}
```

## Native メンバー Function

Struct の中には `native` Function も宣言できます。

```mana
struct Transform
{
    native void reset();
}
```

呼び出し側の構文は通常のメンバー Function と同じです。

```mana
Transform transform;
transform.reset();
```

C++ 側との対応方法は Native Function のリファレンスで扱います。

## Struct と Actor の違い

Struct は値をまとめるデータ型です。Actor のように独立した Action 実行主体にはなりません。

| Struct | Actor |
| --- | --- |
| データ型 | 実行主体 |
| Function を持てる | Action と状態を持てる |
| Request の対象ではない | Request の対象になる |
| 変数として保持する | VM が Actor インスタンスを管理する |

## 関連項目

- [Function](./reference-functions.md)
- [Actor](./reference-actor.md)
- Native Function（追加予定）
