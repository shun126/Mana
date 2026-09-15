# 型

Mana は静的に型を持つ言語です。変数、引数、戻り値などには型を指定します。

## 基本型

現在の Lexer で認識される基本型は次のとおりです。

| 型 | 概要 |
| --- | --- |
| `void` | 戻り値を持たない Function などで使用 |
| `char` | 8 bit 整数型 |
| `short` | 16 bit 整数型 |
| `bool` | 真偽値 |
| `int` | 32 bit 整数型 |
| `float` | 32 bit 浮動小数点型 |
| `string` | 文字列型 |
| `pointer` | 低レベルな参照を扱う型 |

例:

```mana
int count = 10;
float speed = 2.5;
bool opened = false;
string name = "Guard";
```

## `void`

`void` は値を保持する通常の変数型ではなく、主に「戻り値がない」ことを表します。

```mana
void reset()
{
}
```

## `bool`

`bool` は `true` または `false` を扱います。

```mana
bool enabled = true;
```

条件式では比較結果や論理演算の結果を使用できます。

## `string`

`string` は文字列を扱います。

```mana
string message = "Hello";
```

文字列リテラルは `"` で囲みます。

## `actor` 型

Actor への参照を Function や native Function の引数として扱う場合、`actor` 型を使用できます。

```mana
void notify(actor target)
{
}
```

`self` や `sender` なども Actor 参照として扱われます。

## 定義済み複合型

現行コンパイラは、よく使う値型として次の型をあらかじめ登録しています。

| 型 | メンバー |
| --- | --- |
| `vec2` | `float x`, `float y` |
| `vec3` | `float x`, `float y`, `float z` |
| `vec4` | `float x`, `float y`, `float z`, `float w` |
| `rotator` | `float pitch`, `float yaw`, `float roll` |
| `color` | `float r`, `float g`, `float b`, `float a` |

これらは Struct と同様にメンバーを `.` で参照できます。

```mana
actor BuiltInTypeExample
{
    action main
    {
        vec3 position;
        position.x = 10.0;
        position.y = 20.0;
        position.z = 30.0;

        color tint;
        tint.r = 1.0;
        tint.g = 0.5;
        tint.b = 0.25;
        tint.a = 1.0;
    }
}
```

`transform` は現行コンパイラでは定義済み型として登録されていません。

## Struct 型

`struct` でユーザー定義型を作成できます。

```mana
struct Position
{
    float x;
    float y;
}

Position p;
```

Struct の詳細は [Struct](./reference-struct.md) で扱います。

## `pointer`

`pointer` は主に VM と native Function の境界など、低レベルな用途で使用する型です。

通常のゲームイベント記述では、まず `int`、`float`、`bool`、`string`、`actor`、定義済み複合型、Struct 型を中心に使用することを推奨します。

## 型チェック

Mana Compiler は、代入、Function 呼び出し、戻り値、演算などで型の整合性を確認します。

型が合わないコードは、原則としてコンパイル時に診断されます。

## 関連項目

- [変数](./reference-variables.md)
- [定数](./reference-constants.md)
- [式](./reference-expressions.md)
- [Struct](./reference-struct.md)
