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

Function では呼び出し元へ戻ります。

```mana
int add(int a, int b)
{
    return a + b;
}
```

Action の中でも `return;` を使用できます。この場合は現在の Action を終了し、その Priority を解放します。下位 Priority に中断中の Action があれば、VM はそこへ復帰できます。

```mana
actor NPC
{
    action talk
    {
        if (sender == Nil)
            return;

        print("Hello\n");
    }
}
```

Action は戻り値を持たないため、Action では `return expression;` を使用しません。

## goto とラベル

ラベルは `identifier:`、分岐は `goto identifier;` で記述できます。

```mana
actor GotoExample
{
    action main
    {
        goto Done;
        print("skip\n");

Done:
        print("done\n");
    }
}
```

存在しないラベルへの `goto` はコンパイル時の名前解決エラーになります。

通常の条件分岐や繰り返しで表現できる場合は `if`、`switch`、`while`、`for` などの構造化された制御文を優先することを推奨します。

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

詳細な待機条件や Priority との関係は [Request](./reference-request.md) と [実行制御](./reference-execution-control.md) を参照してください。

## Action の実行制御

Action の進行、Priority の巻き戻し、Request の受付状態などを制御する文があります。

```text
yield
join
rollback
halt
lock
refuse
comply
```

正確な構文と動作は [実行制御](./reference-execution-control.md) にまとめています。

## 関連項目

- [式](./reference-expressions.md)
- [演算子](./reference-operators.md)
- [Request](./reference-request.md)
- [実行制御](./reference-execution-control.md)
- [Request と Priority](../concepts/concept-request-and-priority.md)
- [Mana の実行モデル](../concepts/concept-execution-model.md)
