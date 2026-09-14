# 文

Mana で使用する基本的な文をまとめます。

## 式文

式の後ろに `;` を付けます。

```mana
count = count + 1;
update();
```

## ブロック

```mana
{
    int count = 0;
    count += 1;
}
```

## if / else

```mana
if (condition)
{
    print("true\n");
}
else
{
    print("false\n");
}
```

## while

```mana
while (condition)
{
    update();
}
```

## do / while

```mana
do
{
    update();
}
while (condition);
```

## for

```mana
for (int i = 0; i < 10; ++i)
{
    print("loop\n");
}
```

## loop

終了条件を式として持たない繰り返しには `loop` を使用できます。

```mana
loop
{
    update();
}
```

## break

現在の繰り返しや `switch` から抜けます。

```mana
while (true)
{
    if (finished)
        break;
}
```

## continue

現在の反復の残りを飛ばし、次の反復へ進みます。

```mana
for (int i = 0; i < 10; ++i)
{
    if (i == 5)
        continue;

    print("run\n");
}
```

## switch

```mana
switch (value)
{
case 0:
    print("zero\n");
    break;

case 1:
    print("one\n");
    break;

default:
    print("other\n");
    break;
}
```

## return

Function から戻ります。

```mana
int add(int a, int b)
{
    return a + b;
}
```

## print

```mana
print("Hello\n");
```

## Request 系

Actor の Action を依頼する構文です。

```mana
request(10, Enemy->think);
awaitStart(10, Enemy->think);
awaitCompletion(10, Enemy->think);
```

詳細な待機条件や Priority との関係は、Request と実行制御のリファレンスで扱います。

## yield と join

Action の進行を協調させるための構文として `yield` と `join` があります。

詳細は実行制御リファレンスで扱います。

## 関連項目

- [式](./reference-expressions.md)
- [演算子](./reference-operators.md)
- [Request と Priority](../concepts/concept-request-and-priority.md)
- [Mana の実行モデル](../concepts/concept-execution-model.md)
