# Function

Function は、値を受け取り、処理をまとめ、必要に応じて値を返す通常のサブルーチンです。

Action と違い、Function は Request の対象ではなく、呼び出した処理の流れの中で同期的に実行されます。

## 構文

```mana
return_type functionName(arguments)
{
    statements
}
```

例:

```mana
int add(int a, int b)
{
    return a + b;
}
```

呼び出し:

```mana
int value = add(2, 3);
```

## 引数

引数は型と名前を並べて宣言します。

```mana
float distance(float x, float y)
{
    return x + y;
}
```

型には組み込み型、`actor`、ユーザー定義型を使用できます。

## 戻り値

戻り値がある Function は `return expression;` で値を返します。

```mana
int getCount()
{
    return 10;
}
```

戻り値がない場合は `void` を使用します。

```mana
void reset()
{
    return;
}
```

`void` Function から値を返すこと、または値を返す Function で `return;` のみを書くことはコンパイルエラーです。

## Struct のメンバー Function

Function は `struct` の中にも定義できます。

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

呼び出しには `.` を使います。

```mana
Counter counter;
counter.reset();
```

Struct のメンバー Function については [Struct](./reference-struct.md) を参照してください。

## Action との違い

| Function | Action |
| --- | --- |
| 通常の関数呼び出しで実行 | Request で実行できる |
| 引数を持てる | 現行構文では引数を持たない |
| 戻り値を持てる | 現行構文では戻り値を持たない |
| 呼び出し元の処理の一部 | Actor の実行単位 |

ゲーム内の独立した行動は Action、Action の内部で再利用する処理は Function、と分けると整理しやすくなります。

## Native Function

C++ 側へ接続する Function は `native` で宣言します。

```mana
native void playSound(string name);
```

Native Function の詳細は別ページで扱います。

## 関連項目

- [Struct](./reference-struct.md)
- [Action](./reference-action.md)
- Native Function（追加予定）
