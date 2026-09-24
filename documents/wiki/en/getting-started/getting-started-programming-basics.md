# Writing a program as text

This page helps you tell apart the place where you write code and the place where you run it. You can read it before you have set up the Mana tools.

## Editor and terminal

A **text editor** is an app for typing text and saving it to a file. Mana code is saved as plain text, not in a document format that stores text colours or paragraph styles.

A **terminal** is a window where you type commands to run tools. On Windows you use PowerShell, and on Linux a shell.

In these lessons, each code box is labelled with what it is for.

**Example source code — what you type in the editor:**

```mana
actor Hello
{
    action main
    {
        print("Hello, Mana!\n");
    }
}
```

**Example command — what you type in the terminal:**

```text
mana hello.mn
```

You do not write the command into the source file. `mana` is the name of the tool, and `hello.mn` is the file it processes. How to point to the tool is set up on the next page.

## File names and where to save

The `.mn` in `hello.mn` is the **extension**: the end of a file name that shows what the file is for. Mana source files normally use it.

When you save, check the following.

- Name the file `hello.mn`. Make sure it has not become `hello.mn.txt`.
- Use UTF-8 as the character encoding. The examples in these lessons are saved and used as UTF-8.
- Remember the folder you saved it in. A folder is also called a **directory**.
- Save after editing. The command reads what has been saved to disk.

If you cannot see extensions on Windows, turn on file name extensions in File Explorer.

## Symbols have meaning too

`print("Hello, Mana!\n");` is an instruction that prints text.

| Written as | Role |
| --- | --- |
| `"Hello, Mana!\n"` | The text to print. A string is enclosed in straight double quotes `"` |
| `\n` | A line break inside the string |
| `;` | The end of this statement |
| `{` and `}` | Enclose the extent of a definition or a block of work |

Type brackets, semicolons and other symbols in half-width (ASCII) characters. Upper and lower case are different, as in `Hello` and `hello`.

The spaces at the start of a line are called **indentation**. To make it easy to see which block a line belongs to, these lessons add four spaces each time a line goes one level further in. Indentation does not let you leave out any brackets or `;` that are needed.

Everything from `//` to the end of the line is a **comment**. You can write explanations there for people to read.

**Example inside an Action:**

```mana
// Print the welcome text
print("Welcome!\n");
```

## Write, save, run, check

A program does not have to be finished in one go. Make a small change and check the result. An error is a clue for reviewing what you typed. You will practise reading errors later.

Next, [Setting up Mana](./getting-started-installation.md) prepares the tool you use to run programs.
