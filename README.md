<div align="center">
<h1>Actor-oriented scripting language Mana</h1>
<p>
<a href="https://github.com/shun126/Mana/issues">Issues</a>,
<a href="https://github.com/shun126/Mana/discussions">Discussions</a>,
<a href="https://github.com/shun126/Mana/wiki">Wiki</a>,
<a href="https://deepwiki.com/shun126/Mana">DeepWiki</a>,
<a href="https://mnu.sakura.ne.jp/_doxygen/mana">Doxygen</a>
</p>
<p>
<a href="https://github.com/shun126/Mana/stargazers"><img src="https://img.shields.io/github/stars/shun126/Mana?style=social"></a>
<a href="https://github.com/shun126/Mana/blob/main/LICENSE"><img src="https://img.shields.io/github/license/shun126/Mana"></a>
<a href="https://github.com/shun126/Mana/releases"><img src="https://img.shields.io/github/v/release/shun126/Mana"></a>
<a href="https://github.com/shun126/Mana/releases"><img src="https://img.shields.io/github/downloads/shun126/Mana/total"></a>
</p>
</div>

This scripting language was developed with the aim of developing a character's action using actor model.
You can implement the execution engine easily into your computer game program.

# What Mana code looks like
````mana:talk.mn
actor Mother
{
    int mFlag;
    action initialize
    {
        mFlag = 0;
    }
    action main
    {
        print("Hi");
        request(1, Child->talk);
    }
}

actor Child
{
    action update
    {
        int i;
    }
    action talk
    {
        print("Hi");
    }
}
````

## Namespaces and action references

Use `namespace` blocks to group actors, and `using` to import namespace paths or actor symbols. Action references use `->`, while `::` is reserved for namespace qualification.

````mana
namespace Game::AI
{
    actor Enemy
    {
        action think
        {
        }
    }
}

using Game::AI;

actor Controller
{
    action main
    {
        request(1, Enemy->think);
    }
}
````

# Repository layout

| Directory | Builds | Description |
| --- | --- | --- |
| `compiler` | `manac.lib` / `libmana.a` | The compiler itself. Free of command line and file output concerns, so it can be embedded in another application. |
| `driver` | `mana` | The command line tool that drives the compiler. |
| `runner` | header only | The virtual machine that executes a compiled program. |
| `sample` | | Example scripts. |
| `test` | | Test scripts and the test runner. |

# Installing
## Requirements
* [bison 3.8](https://www.gnu.org/software/bison/)
* [flex 2.6.4](https://github.com/westes/flex)
* [C++ Compiler](https://en.wikipedia.org/wiki/C%2B%2B)
  * [Visual Studio](https://visualstudio.microsoft.com/)
  * [Clang](https://clang.llvm.org/)

## Any Linux Distribution
- cd to <download_path>
- make

## Building with Cygwin
- Install Cygwin from: http://www.cygwin.com/
- cd to <download_path>
- make

## Building with MSVC
- Set the path to bison in the environment variable GNU_BISON_BIN, and the path to flex in GNU_FLEX_BIN.
- Install Microsoft Visual C++ 2022 Community (should work with other versions).
- Run "Vistual Studio 2022 Command Prompt" from the "Visual Studio 2022" start menu.
- Open mana.sln

# Testing

Three suites, and all of them matter:

| Suite | What it covers |
| --- | --- |
| `test/mana/test.py` | The language, through the `mana` executable. Checks exit status, diagnostics and, for the cases that run, what the script printed. |
| `test/cpp/EmbeddingTest` | The library interface: compiling from memory, diagnostics as data, redirected output, faults, script errors and native bindings. None of this is reachable from the command line. |
| `test/cpp/ProgramImageTest` | Reading a compiled program image back. |

The scripts the language suite feeds to `mana` live in `test/mana/`; the C++
suites and the program image they read live in `test/cpp/`.

`make test` from the top of the tree runs all three.

On MSVC, open `mana.sln` and build - it holds `manac`, `mana` and both test
programs - then run `EmbeddingTest.exe` and `ProgramImageTest.exe` from the
configuration you built.

### Run every configuration

Bugs in this codebase have hidden in a single configuration more than once, so
CI builds and tests all four combinations of 32 bit and 64 bit against debug
and release, and nothing is allowed to skip one:

* A pointer that survives a 32-bit build can be truncated in a 64-bit one, and
  the other way round for anything sized against a pointer.
* Assertions are compiled out of release builds, so a release-only run will
  walk past a broken invariant without a word.

`EmbeddingTest` prints the pointer and `int_t` widths it was built with, so a
log makes plain which one ran.

# Running Sample
````bash
mana sample/sample.mn
````

# How to Use

When you specify the source file at runtime, it will compile and execute it.

```bash
mana source_file
```

If you specify the `-o` option, it will output the compiled binary file.

```bash
mana source_file -o binary_file
```

If you specify the `--execute` option, it will execute the binary file.

```bash
mana --execute binary_file
mana -e binary_file
```

# How to Embed the Virtual Machine

1. Copy the `runner` directory to your preferred location in your project.
1. Add `#include "runner/Mana.h"` to your code.
1. Create the VM class and load the program.
1. Tick the VM.

```cpp
auto vm = std::make_shared<mana::VM>();
vm->LoadPlugins(".");
vm->LoadProgram(path);
// Tick
while (vm->Run())
    ;
```

### Binding native code

Register C-style callbacks or bind methods on existing C++ objects to the same `native` entry points in Mana scripts.

```cpp
// Plain function binding
vm->RegisterFunction("nativeFunction", &NativeFunction);

// Bind a member function that matches mana::VM::ExternalFunctionType
auto instance = std::make_shared<MyPlugin>();
vm->RegisterMemberFunction("pluginCallback", instance, &MyPlugin::OnCall);
```

### Redirecting output

Everything the virtual machine prints - the `print()` builtin, execution
traces, errors and assertions - goes through `mana::Trace`, which writes to
standard output by default. A host where standard output goes nowhere, such as
a packaged game, can redirect it.

```cpp
void OnTrace(void* userData, const mana::TraceLevel level, const char* message, const std::size_t length)
{
    // message is NUL terminated UTF-8; length excludes the terminator
    switch (level)
    {
    case mana::TraceLevel::Error:   /* log as an error */   break;
    case mana::TraceLevel::Warning: /* log as a warning */  break;
    default:                        /* log as info */       break;
    }
}

mana::SetTraceHandler(&OnTrace, myHost);
```

Pass `nullptr` to go back to standard output. Set the handler once before the
virtual machine runs: replacing it while output is in flight is not safe.

The handler is not called once per line. A few execution traces build one line
from several calls, so buffer until a newline if the destination is line
oriented. `MANA_BUG` and the assertion macros throw as soon as they have
written, so a handler must not throw.

### Faults never end the process

A broken invariant inside mana - the sort of thing an assertion catches - is
reported and then thrown as `mana::FatalError`. It is never `std::terminate`.
A guest scripting engine has no business taking its host down with it, and in
an editor that would cost someone their unsaved work.

Two places catch it, because those are the two places where recovering means
something:

| Where | What survives |
| --- | --- |
| `mana::Compile` | The compile stops and reports a fatal `mana::Diagnostic`. |
| `mana::VM::Run` | That one actor halts. Every other actor, the VM and the host carry on. |

To stop at the fault instead of unwinding - which is what you want while
working on mana itself - install a handler. It runs before the stack unwinds,
so a breakpoint there still has the frames that caused it.

```cpp
void OnFault(void* userData, const char* file, const int line, const char* message)
{
    // break into the debugger, or log through the host
}

mana::SetFaultHandler(&OnFault, myHost);
```

The handler must not throw: `mana::FatalError` is thrown as soon as it returns.

### Faults in a script

A mistake in a script is not mana's own invariant breaking, so it is reported
as a `mana::ScriptError` rather than a `mana::FatalError`, and the fault
handler is left alone. The same boundary catches it, so the actor halts and
everything else keeps running:

```
mana: actor Root halted: script error: division by zero
mana: actor Root halted: script error: subscript out of range: byte offset 400 is outside the array of 16 byte(s)
```

Checked in every configuration, release included, because each of these would
otherwise take the process with it or quietly overwrite another variable:

| What | Why it cannot continue |
| --- | --- |
| Integer division or remainder by zero | Traps in the CPU, so no C++ handler ever sees it |
| Integer division of the smallest value by -1 | Traps the same way |
| A subscript outside its array | Reads or writes whatever happens to be next to it |
| Awaiting an action of the actor doing the awaiting | Waits for something that cannot arrive |

Floating point division by zero is left alone; it yields infinity, which is
a value like any other.

# How to Embed the Compiler

The compiler is built as a static library (`manac.lib` on MSVC, `libmana.a` on
make) that the `mana` command line tool links against. Applications that need to
compile scripts themselves — an editor, an asset pipeline, a test harness — can
link the same library instead of shelling out to the executable.

1. Build the `manac` project (MSVC) or run `make` in the `compiler` directory.
1. Add `#include "compiler/Compiler.h"` to your code.
1. Fill in `mana::CompileOptions` and call `mana::Compile()`.

`Compile()` writes no files. The program image, the generated C++ header and the
debug dump are all returned by value, so the caller decides what to do with them.

```cpp
mana::CompileOptions options;
options.mSourceFilename = "npc.mn";
options.mForcedIncludeFiles.emplace_back("Function.mh");

const mana::CompileResult result = mana::Compile(options);

for (const mana::Diagnostic& diagnostic : result.mDiagnostics)
{
    // Structured: mSeverity, mFilename, mLineNo and mMessage are all available,
    // so the host can render diagnostics in its own UI.
    std::cout << diagnostic.ToString() << '\n';
}

if (result.mSucceeded)
{
    // Hand the program image straight to the virtual machine
    auto image = std::make_shared<std::vector<uint8_t>>(std::move(result.mProgramImage));
    vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));
}
```

`Compile()` never throws; exceptions raised inside the compiler are caught and
reported as a fatal `mana::Diagnostic`. It touches no global process state such
as the working directory either.

### Supplying sources from somewhere other than disk

Sources reach the compiler through `mana::SourceResolver`. The default,
`mana::FileSourceResolver`, reads from the file system, resolving a relative
`include` against the directory of the file that includes it. Implement the
interface to compile from an editor buffer that has not been saved, from an
archive, or from a game engine asset system.

```cpp
class MemorySourceResolver final : public mana::SourceResolver
{
public:
    std::map<std::string, std::string, std::less<>> mFiles;

    std::string Resolve(const std::string_view from, const std::string_view filename) const override
    {
        return std::string(filename);
    }

    bool Read(const std::string_view path, std::string& outText) const override
    {
        const auto it = mFiles.find(path);
        if (it == mFiles.end())
            return false;
        outText = it->second;
        return true;
    }
};

auto resolver = std::make_shared<MemorySourceResolver>();
resolver->mFiles["npc.mn"] = editorBufferText;

mana::CompileOptions options;
options.mSourceFilename = "npc.mn";
options.mSourceResolver = resolver;
```

Line endings are normalised by the compiler, so a resolver may return text as
it found it.

> **Note**
> The compiler still keeps global state, so `Compile()` must not be called from
> more than one thread at a time.

# License

MIT License

# 👀 See also
* [Issues](https://github.com/shun126/Mana/issues)
* [Discussions](https://github.com/shun126/Mana/discussions)
* [Wiki](https://github.com/shun126/Mana/wiki)
* [DeepWiki](https://deepwiki.com/shun126/Mana)
* [Doxygen](https://mnu.sakura.ne.jp/_doxygen/mana/index.html)

# Author
* Shun Moriya ([X.com](https://x.com/monjiro1972))

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/M4M413XDXB)
