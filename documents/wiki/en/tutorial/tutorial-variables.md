# Remembering state with variables

You talk to the guide twice and print how many times you have talked. A **variable** stores the value.

## Run it and see

This is the **whole file**. Replace `lesson.mn` in the Mana folder with it, save, and run it with `mana lesson.mn`.

```mana
int gTalkCount = 0;

actor Event
{
    action main
    {
        awaitCompletion(10, Guide->talk);
        awaitCompletion(10, Guide->talk);
    }
}

actor Guide
{
    action talk
    {
        gTalkCount = gTalkCount + 1;
        print("Talk count: %d\n", gTalkCount);
    }
}
```

**Expected output:**

```text
Talk count: 1
Talk count: 2
```

You can also run the [finished code that comes with Mana](../../../../examples/tutorial/05-variables.mn) with `mana examples/tutorial/05-variables.mn`.

## Type, name and value

`int gTalkCount = 0;` declares a variable that holds an integer and sets its first value to 0. `int` is the **type**, the kind of value it holds, and `gTalkCount` is the variable's name.

`gTalkCount = gTalkCount + 1;` is an **assignment** that adds 1 to the current value and stores the result. Unlike an equation in maths, it computes the value on the right and puts it into the left. The first time it goes from 0 to 1, the second time from 1 to 2.

## Printing a value

In `print("Talk count: %d\n", gTalkCount);`, `%d` marks where the integer passed after it is printed. `\n` is a line break.

Printing a variable shows how far the work has got and how the value has changed.

## Keeping a value across Actions

This variable is declared outside any Actor, so it is a **global variable**. Several Actors can refer to it, but in this example only `Guide` changes it. Deciding who is responsible for changing a value makes its changes easier to follow.

Starting the name with `g` is a convention that makes it easy to tell it is global; the language does not require it.

The variable keeps its value after the conversation's Action ends. But if you quit the program and start it again, it starts from 0 again. Nothing has been saved to a file.

Mana can also declare state variables inside an Actor. However, the implementation checked for these lessons has a problem: such a variable cannot be referred to by name inside an Action. So the runnable lessons use a global variable. For details, see the current limitations in the [variables reference](../reference/reference-variables.md).

## Local variables used inside one piece of work

A variable declared inside an Action is a **local variable** used by that work.

**Example that replaces the body of talk:**

```mana
int count = 0;
count = count + 1;
print("Talk count: %d\n", count);
```

Here each conversation starts from `count = 0`, so both times it prints 1.

## Common types

The following are **examples of declarations written inside an Action**.

```mana
int count = 3;
float distance = 1.5;
bool hasKey = true;
string message = "Welcome!";
```

| Type | Value it stores |
| --- | --- |
| `int` | An integer |
| `float` | A number that can have a fractional part |
| `bool` | One of two values, `true` or `false` |
| `string` | A string of text |

## Change one thing

Go back to the finished code and add a line to `Event` that requests a third conversation. If it prints up to `Talk count: 3`, you have confirmed that the state is kept.

## Read next

In [Changing behaviour with conditions](./tutorial-conditions.md), you branch on a value.
