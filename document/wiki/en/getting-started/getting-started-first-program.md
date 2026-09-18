# Your first Mana program

You print some text, then change that text yourself. Use the terminal you set up on the [setup page](./getting-started-installation.md).

## 1. Create a source file

Type the following **whole file** into your editor and save it in the Mana folder as `hello.mn`. Save it as UTF-8 plain text.

```mana
actor Hello
{
    action main
    {
        print("Hello, Mana!\n");
    }
}
```

## 2. Run it

In the terminal in the Mana folder, enter the following.

```text
mana hello.mn
```

**Expected output:**

```text
Hello, Mana!
```

If you see it, it worked. If not, check that the file has not become `hello.mn.txt`, that you saved it, and that the terminal's working folder is the Mana folder.

To compare with the example, you can also use the [finished code that comes with Mana](../../../../examples/tutorial/01-hello.mn).

```text
mana examples/tutorial/01-hello.mn
```

## 3. Read the code

`actor Hello` defines an Actor named `Hello`. `Hello` is a name you chose.

The `action main` inside it defines work the Actor does. `main` is a special Action name that runs at startup. For now, remember that this is where you write the first thing to run.

`print("Hello, Mana!\n");` is a statement that outputs text. In the examples so far, output goes to the terminal.

If you follow which `{` matches which `}`, you can see that `print` is inside `main`, and `main` is inside `Hello`.

## 4. Change it and predict the result

**Replace** the `print` line with these two lines.

```mana
print("Welcome!\n");
print("The gate is closed.\n");
```

Save, then run `mana hello.mn` again.

```text
Welcome!
The gate is closed.
```

Inside this Action, statements run from top to bottom. Swap the two lines and the output swaps too.

Next, [Compiling, running and fixing errors](./getting-started-compile-and-run.md) shows what happens before a program runs, and how to investigate when something is wrong.
