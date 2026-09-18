# Mana Integration

This section explains how to embed the Mana Compiler and the Mana VM in a C++ application or a game engine.

## The overall picture

```text
Mana source
   |
   v
mana::Compile()
   |
   v
CompileResult::mProgramImage
   |
   +--> Inspect its contents with mana::ProgramImage
   |
   v
mana::VM::LoadProgram()
   |
   +--> Connect to host features with Native Functions
   |
   v
VM::Run()
```

The way source is supplied to the Compiler can be replaced through `SourceResolver`. Compile-time problems are reported to the host through `Diagnostic`, and run-time problems through Trace / ScriptError / FatalError and so on.

## Reading order

1. [Integration overview](./integration-overview.md)
2. [Compiler](./integration-compiler.md)
3. [VM](./integration-vm.md)
4. [Program Image](./integration-program-image.md)
5. [Native Functions](./integration-native-functions.md)
6. [SourceResolver](./integration-source-resolver.md)
7. [Diagnostics](./integration-diagnostics.md)
8. [Error Handling](./integration-error-handling.md)

## Find by purpose

| What you want to do | Page |
| --- | --- |
| See the overall picture of embedding Mana in a game | [Integration overview](./integration-overview.md) |
| Compile Mana source from C++ | [Compiler](./integration-compiler.md) |
| Run a Program Image | [VM](./integration-vm.md) |
| Get the list of Actors / Actions without running | [Program Image](./integration-program-image.md) |
| Call game-side C++ from Mana | [Native Functions](./integration-native-functions.md) |
| Supply source from an editor or assets | [SourceResolver](./integration-source-resolver.md) |
| Show compile errors in an IDE or CI | [Diagnostics](./integration-diagnostics.md) |
| Handle runtime errors and internal Faults | [Error Handling](./integration-error-handling.md) |

## Who this is for

Whereas the Language Reference is for people writing Mana scripts, Integration is for C++ developers embedding Mana in games and tools.

To look up the Mana language itself, see the [Language Reference](../reference/README.md).
