# Compiler

The entry point for using the Mana Compiler from C++ is `mana::Compile()`.

## Basic form

```cpp
mana::CompileOptions options;
options.mSourceFilename = "main.mn";

mana::CompileResult result = mana::Compile(options);
```

`Compile()` writes no files; it stores what it produces in a `CompileResult` and returns it.

## CompileOptions

The main settings are:

| Member | What it is |
| --- | --- |
| `mSourceFilename` | The entry source file |
| `mForcedIncludeFiles` | Files read before the source. Equivalent to `-I` in the CLI |
| `mGenerateDump` | Generates a dump of the symbol table, syntax tree and intermediate code |
| `mGeneratePublicTypeDecl` | Generates a C++ type declaration header |
| `mSourceResolver` | Replaces how source is supplied |
| `mDiagnosticHandler` | Callback called when a diagnostic occurs |

If `mSourceResolver` is left out, source is read from the standard file system.

## CompileResult

| Member | What it is |
| --- | --- |
| `mSucceeded` | `true` if there were no errors |
| `mProgramImage` | The generated Program Image |
| `mPublicTypeDecl` | The public C++ type declarations |
| `mDump` | A Markdown dump for debugging |
| `mDiagnostics` | All diagnostics that occurred |

If compilation fails, `mProgramImage` is empty.

## Showing diagnostics as they occur

```cpp
options.mDiagnosticHandler = [](const mana::Diagnostic& diagnostic)
{
    std::cerr << diagnostic.ToString() << '\n';
};
```

Even with a handler, diagnostics are also kept in `CompileResult::mDiagnostics`.

In the current implementation, even if the diagnostic handler itself throws an exception, it is handled so that the exception does not cross out of `Compile()`.

## Forced includes

```cpp
options.mForcedIncludeFiles.push_back("common.mn");
options.mForcedIncludeFiles.push_back("platform.mn");
```

Files added earlier are read earlier.

## Generating public type declarations

```cpp
options.mGeneratePublicTypeDecl = true;
const mana::CompileResult result = mana::Compile(options);

if (result.mSucceeded)
{
    const std::string& header = result.mPublicTypeDecl;
}
```

You get the same generation as `-t` in the CLI, as a string rather than a file.

## Generating a dump

```cpp
options.mGenerateDump = true;
```

For analysing successes and failures, or for compiler development, a Markdown dump is available from `mDump`.

## Exception boundary

`Compile()` is designed to catch exceptions that occur inside it and turn them into fatal diagnostics. Normally the host only needs to check `mSucceeded` and `mDiagnostics`, and does not need to build control flow around Mana's internal exceptions.

## Thread safety

The current compiler has global state, so `Compile()` cannot be called from several threads at the same time.

## Related topics

- [Integration overview](./integration-overview.md)
- [Program Image](./integration-program-image.md)
- [Source files](../reference/reference-source-files.md)
