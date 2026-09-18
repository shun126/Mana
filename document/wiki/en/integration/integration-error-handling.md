# Error Handling

When embedding Mana in a game or tool, errors are easier to handle if you don't treat them as one kind, but think of them separately as **compile time / loading the Program Image / script execution / internal inconsistencies in Mana**.

## The overall picture

```text
Mana source
   |
   | Compile diagnostics
   v
mana::Compile()
   |
   | Program Image load error
   v
mana::VM::LoadProgram()
   |
   | ScriptError / runtime trace
   v
mana::VM::Run()
   |
   | FatalError / FaultHandler
   v
An inconsistency in Mana or in the embedding
```

Each has a different unit of recovery and a different notification route.

## Compile-time errors

`mana::Compile()` returns failures as a `CompileResult`.

```cpp
const mana::CompileResult result = mana::Compile(options);

if (!result.mSucceeded)
{
    for (const auto& diagnostic : result.mDiagnostics)
        ShowError(diagnostic);
}
```

Exceptions that occur inside the Compiler are also caught at the boundary of `Compile()` and stored in `mDiagnostics` as fatal diagnostics.

So ordinary embedding code does not need to control the whole of `Compile()` with exceptions because of compile errors.

For details, see [Diagnostics](./integration-diagnostics.md).

## Errors loading a Program Image

Unlike the Compiler API, `VM::LoadProgram()` may throw an exception when loading fails.

For example, the current implementation checks for problems such as:

- The file cannot be opened
- It does not have the signature of a Mana Program Image
- The Program Image's version does not match
- The 32-bit / 64-bit format does not match the runtime

On the host side, catch exceptions at the loading boundary.

```cpp
try
{
    vm->LoadProgram("event.mx");
}
catch (const std::exception& e)
{
    LogError(e.what());
    return false;
}
```

When loading from memory too, design the host to handle exceptions for an invalid Program Image.

## Checking in advance with ProgramImage

To examine a Program Image without running it, you can use `mana::ProgramImage`.

```cpp
mana::ProgramImage image;

if (!image.LoadProgram(program, size))
{
    LogError(image.GetLastError());
    return false;
}
```

`ProgramImage::LoadProgram()` returns success as a `bool`, and the reason for a failure is available from `GetLastError()`.

In an editor or asset importer, you can use it as a check before handing the image to the VM.

## Script execution errors

Problems during execution of a Mana script that it cannot continue from, such as division by zero, an array index out of range, or an invalid wait on itself, are handled as `mana::ScriptError`.

`VM::RunActor()` catches `ScriptError` per Actor.

```text
Actor A
  ScriptError
     |
     v
  Actor A is stopped

Actor B / C
  Keep running
```

The Actor that caused the error stops, but the design is not to stop the whole VM or other Actors for that reason alone.

The error details are written to the Trace as `TraceLevel::Error`.

## Connecting the Trace to the host

Output from `print()`, warnings, runtime errors and so on can be received through the Trace.

```cpp
void OnTrace(
    void* userData,
    const mana::TraceLevel level,
    const char* message,
    const std::size_t length)
{
    // Send it to the game engine's log
}

mana::SetTraceHandler(&OnTrace, hostContext);
```

There are three severities.

```cpp
mana::TraceLevel::Info
mana::TraceLevel::Warning
mana::TraceLevel::Error
```

When embedding in a game engine where standard output is hard to use, we recommend setting a TraceHandler at startup and connecting it to the engine's log.

### Notes on the TraceHandler

Set the TraceHandler once before you start using it, and don't swap it frequently while running.

Also, don't throw exceptions from the Handler itself.

The Trace is not always called back one line at a time, so if you need line-by-line logs, buffer on the host side until a line break.

## Internal inconsistencies in Mana

When an internal assumption breaks, not because of a script mistake but because of an inconsistency in Mana itself or in the embedding, `mana::FatalError` is used.

This family is reported from `RaiseFault()`, which writes an Error Trace and then throws `FatalError`.

A `std::exception` that occurs while an Actor runs is caught at the boundary of `VM::RunActor()`, and that Actor stops. Other Actors can continue.

## FaultHandler

To connect internal inconsistencies to a debugger or crash reporting, you can use `SetFaultHandler()`.

```cpp
void OnFault(
    void* userData,
    const char* file,
    int line,
    const char* message)
{
    // debugger break / crash reporter / telemetry and so on
}

mana::SetFaultHandler(&OnFault, hostContext);
```

The FaultHandler is called before unwinding starts, so you can also use it to stop in the debugger on the spot while developing Mana.

After the Handler returns, `FatalError` is thrown.

The FaultHandler itself must not throw exceptions.

## ScriptError and FatalError

| Kind | Meaning | FaultHandler | Actor |
| --- | --- | --- | --- |
| `ScriptError` | A runtime error on the script side | Not called | The Actor in question is stopped |
| `FatalError` | An internal inconsistency in Mana or the embedding | Called | At an Actor execution boundary, the Actor in question is stopped |

It is important to keep errors shown to script authors apart from internal inconsistencies that Mana / engine developers should investigate.

## Recommended approach on the host side

On the embedding side, it is easier to handle if you divide the responsibilities like this:

1. Show Compiler problems in the editor as `Diagnostic`s
2. Treat Program Image load failures as exceptions of the loading code
3. Send runtime `ScriptError`s to the Trace, and stop only the Actor in question
4. Notify developers of `FatalError` through the Trace + FaultHandler
5. Set the `TraceHandler` and `FaultHandler` when the application initialises

This lets you choose a unit of recovery that fits the kind of problem, rather than ending the whole game process immediately because of one invalid script.

## Related topics

- [Diagnostics](./integration-diagnostics.md)
- [VM](./integration-vm.md)
- [Program Image](./integration-program-image.md)
- [Native Functions](./integration-native-functions.md)
