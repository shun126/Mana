# Statements

This page lists the basic statements used in Mana.

## Expression statements

Put `;` after an expression.

```mana
count = count + 1;
update();
```

## Blocks

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

For a loop with no exit condition written as an expression, use `loop`.

```mana
loop
{
    update();
}
```

## break

Leaves the current loop or `switch`.

```mana
while (true)
{
    if (finished)
        break;
}
```

## continue

Skips the rest of the current iteration and moves on to the next.

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

In a Function, it returns to the caller.

```mana
int add(int a, int b)
{
    return a + b;
}
```

`return;` can also be used inside an Action. In that case it ends the current Action and releases its Priority. If there is a suspended Action at a lower Priority, the VM can return to it.

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

An Action has no return value, so `return expression;` is not used in an Action.

## goto and labels

A label is written `identifier:`, and a jump `goto identifier;`.

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

A `goto` to a label that doesn't exist is a name resolution error at compile time.

Where ordinary branches and loops can express it, we recommend preferring structured control statements such as `if`, `switch`, `while` and `for`.

## print

```mana
print("Hello\n");
```

## The Request family

Syntax for asking an Actor to run an Action.

```mana
request(10, Enemy->think);
awaitStart(10, Enemy->think);
awaitCompletion(10, Enemy->think);
```

For the detailed waiting conditions and how they relate to Priority, see [Request](./reference-request.md) and [Execution control](./reference-execution-control.md).

## Controlling how Actions run

There are statements that control how an Action progresses, winding Priority back, whether Requests are accepted, and so on.

```text
yield
join
rollback
halt
lock
refuse
comply
```

The exact syntax and behaviour are gathered in [Execution control](./reference-execution-control.md).

## Related topics

- [Expressions](./reference-expressions.md)
- [Operators](./reference-operators.md)
- [Request](./reference-request.md)
- [Execution control](./reference-execution-control.md)
- [Request and Priority](../concepts/concept-request-and-priority.md)
- [Mana's execution model](../concepts/concept-execution-model.md)
