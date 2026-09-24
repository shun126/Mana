# Compiler and VM

Mana does not run source code directly as it is.

The Mana Compiler analyses the source code and produces a **Program Image**, and the Mana VM runs that Program Image.

```text
Mana source (.mn)
        |
        v
Mana Compiler
        |
        v
Program Image
        |
        v
Mana VM
        |
        v
Running Actors / Actions
```

## A scripting language can be compiled too

Even a scripting language can turn its source code into another form before running it.

Compiling, in Mana's sense, does not mean producing native machine code that the CPU runs directly.

It means **producing a Program Image for the Mana VM**.

## What the Compiler does

The Compiler reads the source, analyses its syntax and meaning, and turns it into a form the VM can run.

Conceptually, the flow is:

```text
Read the source
    ↓
Analyse the syntax
    ↓
Analyse names and types
    ↓
Detect errors
    ↓
Produce the Program Image
```

Mana can handle namespaces and forward references because it analyses the whole source at the compile stage instead of simply running it one line at a time from the top.

## What a Program Image is

A Program Image is the compiled data that connects the Compiler and the VM.

It holds what the VM needs to run the program: Actors, Actions, instructions, constants and so on.

```text
Source Code
    ↓ Compiler
Program Image
    ↓ VM
Runtime State
```

Source code and the state of running Actors are different things.

## What the VM does

The Mana VM loads the Program Image and runs Actors and Actions.

Its main jobs are:

- Managing Actors
- Running Actions
- Processing Requests
- Interrupting and resuming by Priority
- Executing VM instructions
- Working with the C++ side

The `request` and `awaitCompletion` you used in the Tutorial also work through this VM's execution model.

## When a Program Image is loaded

Ordinary `actor`s are created on the VM when the Program Image is loaded.

After that, initialisation runs, and then each Actor's `init` and then `main` are started.

A `phantom`, on the other hand, is kept as definition information; no Actor instance is created at load time.

## The CLI can do it all at once

```bash
mana source.mn
```

compiles the source and runs it on the VM straight away.

You can also write the Program Image out to a file.

```bash
mana source.mn -o program.bin
```

A compiled file can be run like this.

```bash
mana --execute program.bin
```

In terms of the internal roles, the differences are:

```text
mana source.mn
    = Compile + Execute

mana source.mn -o program.bin
    = Compile

mana --execute program.bin
    = Execute
```

## The Compiler and VM can be embedded

Mana's Compiler and VM are not confined to the command-line tool.

The Compiler can be embedded in a game or editor as a library, and the VM as a runtime.

```text
Editor / Game Engine
        |
        +--> Mana Compiler
        |        |
        |        v
        |   Program Image
        |        |
        +--> Mana VM
```

The C++ API is covered in detail in Integration.

## Compile time and run time

Because the Compiler and VM are separate, problems are also detected at separate stages.

The Compiler detects problems that can be decided before running, such as syntax, name resolution and types.

The VM handles problems that can only be decided while Actors and Actions are actually running.

This separation lets you catch what mistakes you can before running, while leaving control during the game to the VM.

## Program Image compatibility

A Program Image is the executable format the Mana VM reads.

The current VM checks the signature, version, bit width and so on when loading it.

So treat a Program Image as a compiled format to be used with a matching Mana VM.

## Summary

- Mana is split into a Compiler and a VM
- The Compiler turns `.mn` source into a Program Image
- A Program Image is an executable format for the Mana VM
- The VM runs Actors / Actions / Requests
- The Compiler is responsible for analysis and error detection before running
- The CLI can compile and execute together or separately
- The Compiler and VM can be embedded in a C++ application

That completes the basic topics of Concepts.

Next, the **Language Reference** organises Mana's syntax and each language feature so you can look them up precisely.
