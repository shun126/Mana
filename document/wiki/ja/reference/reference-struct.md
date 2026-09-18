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
actor StatusExample
{
    action main
    {
        Status status;
        status.hp = 100;
        status.mp = 20;
    }
}
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

actor UnitExample
{
    action main
    {
        Unit unit;
        unit.position.x = 10.0;
        unit.hp = 100;
    }
}
```

メンバー参照には `.` を使います。

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

actor CounterExample
{
    action main
    {
        Counter counter;
        counter.reset();
    }
}
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

actor TransformExample
{
    action main
    {
        Transform transform;
        transform.reset();
    }
}
```

呼び出し側の構文は通常のメンバー Function と同じです。

C++ 側との対応方法は [Native Function](./reference-native.md) と [Native Functions Integration](../integration/integration-native-functions.md) を参照してください。

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
- [Native Function](./reference-native.md)
