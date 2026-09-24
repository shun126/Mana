# CLI

`mana` is the command-line tool that compiles Mana source, writes out Program Images, and runs Program Images.

## Basic form

```text
mana [options] input
```

## Compile and run

```bash
mana main.mn
```

If no output file is given, it compiles the source and then runs that Program Image on the Mana VM straight away.

## Save the Program Image

```bash
mana main.mn -o game.mx
```

With `-o filename`, the compiled Program Image is saved to a file and is not run automatically.

The file extension can be anything.

In the current CLI, if `-o` is given without a value, it generates a `.mx` file name with the same base name as the input source.

```bash
mana main.mn -o
```

In this case the output is `main.mx`.

## Run a Program Image

```bash
mana --execute game.mx
```

With `--execute`, the input is read as a compiled Program Image rather than Mana source, and run on the VM.

The current `driver/Main.cpp` does not implement a short form `-e`.

## Generate a C++ type declaration header

```bash
mana main.mn -t public_types.h
```

`-t filename` writes the public type declarations the compiler generates to a file.

If `-t` is given without a value, the output is a `.h` with the same base name as the input source.

```bash
mana main.mn -t
```

In this case `main.h` is generated.

## Forced includes

```bash
mana -I common.mn main.mn
```

`-I filename` forces the given file into the compilation.

It can be given several times.

```bash
mana -I common.mn -I platform.mn main.mn
```

## Showing information

```bash
mana --help
mana --version
mana --copyright
```

| Option | What it does |
| --- | --- |
| `--help` | Shows how to use it |
| `--version` | Shows Mana's version |
| `--copyright` | Shows the copyright notice |

## List of public options

| Option | What it does |
| --- | --- |
| `-o filename` | Where to write the Program Image |
| `-t filename` | Where to write the C++ type declaration header |
| `-I filename` | Forced include. Can be given several times |
| `--execute` | Runs the input as a Program Image |
| `--help` | Shows help |
| `--version` | Shows the version |
| `--copyright` | Shows the copyright notice |

## Exit codes

If compilation fails or an output file cannot be saved, the CLI returns an exit code that signals failure.

In build scripts and CI, check the exit code and the compiler's diagnostics.

## Related topics

- [Source files and import / include](./reference-source-files.md)
- [Compiler and VM](../concepts/concept-compiler-and-vm.md)
