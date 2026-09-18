# Diagnostics

The Mana Compiler does not just send warnings and errors to standard output as plain strings; it returns them in structured form as `mana::Diagnostic`.

When embedding it in an editor, IDE, CI or in-game tool, `CompileResult::mDiagnostics` lets you show them your own way while keeping the file name and line number.

## Getting them from CompileResult

The most basic way is to check `mDiagnostics` after compiling.

```cpp
mana::CompileOptions options;
options.mSourceFilename = "main.mn";

const mana::CompileResult result = mana::Compile(options);

for (const mana::Diagnostic& diagnostic : result.mDiagnostics)
{
    std::cout << diagnostic.ToString() << '\n';
}
```

Having diagnostics does not always mean compilation failed.

- Warning: a warning. Compilation continues
- Error: an error. Analysis continues as far as it can, but nothing is produced
- Fatal: a fatal error that stops that compilation from continuing

Use `CompileResult::mSucceeded` as the final judgement of success.

## What a Diagnostic contains

`mana::Diagnostic` has the following information.

```cpp
struct Diagnostic
{
    DiagnosticSeverity mSeverity;
    DiagnosticPhase mPhase;
    std::string mFilename;
    int32_t mLineNo;
    std::string mMessage;
};
```

### Severity

```cpp
mana::DiagnosticSeverity::Warning
mana::DiagnosticSeverity::Error
mana::DiagnosticSeverity::Fatal
```

### Phase

```cpp
mana::DiagnosticPhase::Compile
mana::DiagnosticPhase::Link
```

`Compile` covers problems in lexical analysis, parsing, semantic analysis, code generation and so on.

`Link` covers problems in symbol resolution and in producing the Program Image.

## File name and line number

With `mFilename` and `mLineNo`, an editor can jump to the source in question.

```cpp
for (const auto& diagnostic : result.mDiagnostics)
{
    editor.ShowDiagnostic(
        diagnostic.mFilename,
        diagnostic.mLineNo,
        diagnostic.mMessage);
}
```

`mLineNo == 0` means the diagnostic has no line information.

A diagnostic that occurs in a source brought in with `include` / `import` keeps that source's file name and line number.

When you use a custom `SourceResolver`, the logical path `Resolve()` returns becomes important as-is, as the location shown in diagnostics.

## Converting to the standard format

`Diagnostic::ToString()` formats it in Mana's standard format.

```cpp
const std::string text = diagnostic.ToString();
```

With a line number, depending on the platform, it looks like this:

```text
main.mn(12): error: message
```

or like this:

```text
main.mn:12 error: message
```

You can use the structured fields in your own UI, and `ToString()` in the CLI and logs.

## Receiving them as they occur

To receive diagnostics when they occur, rather than after compilation completes, use `CompileOptions::mDiagnosticHandler`.

```cpp
mana::CompileOptions options;
options.mSourceFilename = "main.mn";

options.mDiagnosticHandler = [](const mana::Diagnostic& diagnostic)
{
    LogDiagnostic(diagnostic);
};

const mana::CompileResult result = mana::Compile(options);
```

Even with a Handler set, diagnostics also remain in `CompileResult::mDiagnostics`.

So you can use them like this:

- Handler: real-time display and logging
- `mDiagnostics`: listing after compilation, and tests

## Don't throw from the Handler

We recommend designing the embedding's Diagnostic Handler so that it does not throw exceptions.

The current `Compile()` is implemented so that exceptions do not leave the compile boundary, and a regression test checks that an exception thrown inside the Handler does not cross over to the host either.

But if the diagnostic handling itself fails, you can lose the error display you actually wanted, so keep the Handler as simple as possible.

## The Compiler's threading constraint

The current Mana Compiler uses global state, including for collecting diagnostics.

So `mana::Compile()` cannot be called from several threads at the same time.

Even when an editor compiles in the background, serialise the calls to the Mana Compiler into a single line.

## Using them in CI

In CI it is convenient to handle `mSucceeded` and `mDiagnostics` together.

```cpp
const mana::CompileResult result = mana::Compile(options);

if (!result.mSucceeded)
{
    for (const auto& diagnostic : result.mDiagnostics)
        std::cerr << diagnostic.ToString() << '\n';

    return 1;
}
```

If your own policy treats warnings as errors, the host can decide that by looking at `DiagnosticSeverity`.

## Compile errors and runtime errors are separate

The `Diagnostic` this page covers is mainly the Compiler's diagnostics.

Script execution errors and internal VM errors that occur after a Program Image is loaded are handled through other routes, such as `Trace`, `ScriptError` and `FatalError`.

For those, see [Error Handling](./integration-error-handling.md).

## Related topics

- [Compiler](./integration-compiler.md)
- [SourceResolver](./integration-source-resolver.md)
- [Error Handling](./integration-error-handling.md)
