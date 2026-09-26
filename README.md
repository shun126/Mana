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
| `examples` | | Example scripts: `tutorial` holds the finished code of the Japanese tutorial, `language-tour` exercises most language features in one program. |
| `tests` | | Test scripts and the test runner. |

Runtime-only applications can include `runner/common/Version.h` and read
`mana::version::Number` without linking the compiler. The program image format
version in `runner/common/FileFormat.h` is a separate compatibility number.
`Version.h` is generated from `runner/common/Version.json` during the build
and is ignored by Git. Runtime-only source users can run
`python3 runner/common/Version.py` to generate it from a repository checkout.
The release source packages already include it. To use Mana from a build system
other than CMake, see [Embedding without CMake](#embedding-without-cmake).

# Installing
## Release source packages

Each `vMAJOR.MINOR.PATCH` tag matching `runner/common/Version.json` produces a
tested GitHub Release with two source archives:

* `mana-runtime-MAJOR.MINOR.PATCH-source.zip` contains the header-only VM in
  `runner/`, its generated `runner/common/Version.h`, the license, and this
  README. Copy `runner/` into a C++17 project to embed the VM.
* `mana-compiler-MAJOR.MINOR.PATCH-source.zip` adds the compiler to the VM, for
  hosts such as game engine editors that compile scripts themselves. It
  contains the compiler, command-line driver, runtime, the license, this README,
  and generated `Parser.cpp`, `Parser.hpp`, `Lexer.cpp`, and `Version.h`, so it
  builds without Bison, Flex, or Python. It has no CMake files, documents,
  examples, or tests; add the sources to your own build as described in
  [Embedding without CMake](#embedding-without-cmake). To build Mana itself,
  use a repository checkout or GitHub's source code archive.

The archives have a single top-level directory. The compiler archive also keeps
`compiler/Parser.yy` and `compiler/Lexer.l` for developers who want to change
the grammar; regenerate the parser and lexer from a repository checkout.

To publish a release, update `runner/common/Version.json` in the release commit,
then push the matching tag (for example, `git tag v1.0.0` followed by
`git push origin v1.0.0`). The release workflow builds and tests that tag
before publishing the two archives with generated release notes.

## Requirements
* CMake 3.20 or newer
* Python 3
* [bison 3.8](https://www.gnu.org/software/bison/)
* [flex 2.6.4](https://github.com/westes/flex)
* [C++ Compiler](https://en.wikipedia.org/wiki/C%2B%2B)
  * [Visual Studio](https://visualstudio.microsoft.com/)
  * [Clang](https://clang.llvm.org/)

## Building on Linux

Install CMake 3.20 or newer, Make, Bison 3.8 or newer, Flex 2.6.4 or newer,
Python 3, and a C++17 compiler. From the repository root:

```sh
export BISON_EXECUTABLE="$(command -v bison)"
export FLEX_EXECUTABLE="$(command -v flex)"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/mana --version
```

Use a separate build directory for Debug, for example `build-debug` with
`-DCMAKE_BUILD_TYPE=Debug`.

## Building with MSVC

Install Visual Studio 2022 or newer with C++ desktop development, CMake 3.20 or newer,
Python 3, Bison 3.8 or newer, and Flex 2.6.4 or newer. In PowerShell, from the
repository root, set both required environment variables to the absolute paths
of the executables:

```powershell
$env:BISON_EXECUTABLE = "C:\path\to\bison.exe"
$env:FLEX_EXECUTABLE = "C:\path\to\flex.exe"
cmake -S . -B build -A x64
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
.\build\Release\mana.exe --version
```

CMake uses the newest Visual Studio it finds; pass `-G` to pick another one.
Replace `x64` with `Win32` for a 32-bit build. Use a separate build directory
for each platform. Both variables are required even when Bison and Flex are on
`PATH`. To configure in the Visual Studio IDE, set the variables in the Windows
user environment and restart Visual Studio, or supply them in a local
`CMakeSettings.json` `environments` entry.

# Testing

Three suites, and all of them matter:

| Suite | What it covers |
| --- | --- |
| `tests/mana/test.py` | The language, through the `mana` executable. Checks the exit status, the diagnostics of scripts that must not compile, and the complete output of scripts that run. |
| `tests/cpp/EmbeddingTest` | The library interface: compiling from memory, diagnostics as data, redirected output, faults, script errors and native bindings. None of this is reachable from the command line. |
| `tests/cpp/ProgramImageTest` | Reading a compiled program image back. |

The scripts the language suite feeds to `mana` live in `tests/mana/`; the C++
suites and the program image they read live in `tests/cpp/`.
Every `.mn` file in `tests/mana/` must be listed in `test.py`; an unlisted
script fails the suite instead of being skipped silently. The language tour's
output is compared with `examples/language-tour/expected-output.txt`, so update
that file when a change to the tour is meant to change what it prints.

`ctest --test-dir build --output-on-failure` runs the three suites, the
language tour, and the tutorial example checks. On Visual Studio generators,
also pass `-C Debug` or `-C Release`.

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

# Running the examples
````bash
mana examples/tutorial/01-hello.mn
mana examples/language-tour/main.mn -I natives.mh
````

See [examples/README.md](examples/README.md) for the full list.

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
Linux) that the `mana` command line tool links against. Applications that need to
compile scripts themselves — an editor, an asset pipeline, a test harness — can
link the same library instead of shelling out to the executable.

1. Build the `manac` CMake target with `cmake --build build --target manac`
   (add `--config Release` for Visual Studio generators).
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

# Embedding without CMake

The repository build regenerates sources with Bison, Flex and Python. To add
Mana to a project with its own build system, such as a game engine, ship the
generated files with the sources and reproduce the settings below by hand. The
compiler release archive already includes the generated files: the parser and
lexer are in `compiler/` next to the other compiler sources, and `Version.h` is
in `runner/common/`.

Otherwise, generate the files once on a machine that has the tools, with a
normal CMake build as described in [Installing](#installing):

| File | Generated from | Needed by |
| --- | --- | --- |
| `runner/common/Version.h` | `runner/common/Version.json` | The virtual machine and the compiler |
| `build/generated/Parser.cpp`, `Parser.hpp`, `Lexer.cpp` | `compiler/Parser.yy`, `compiler/Lexer.l` | The compiler only |

Then configure your build as follows. Every setting here is one that the
`manac` target otherwise passes on to the projects that link it.

* Compile as C++17.
* Add these include directories: the repository root, `compiler/`, `runner/`,
  and the directory holding the generated parser and lexer. Only the
  repository root is needed when embedding just the virtual machine.
* To embed the compiler, compile every `compiler/*.cpp` together with the
  generated `Parser.cpp` and `Lexer.cpp`.
* In debug builds, define `MANA_DEBUG` (or `DEBUG`) for **every** translation
  unit that includes Mana headers, not only the compiler sources. The headers
  add members in debug builds, so mixing the two settings breaks struct
  layouts. MSVC's debug runtime defines `_DEBUG`, which has the same effect.
* With MSVC, pass `/source-charset:utf-8` (or `/utf-8`). The headers are UTF-8
  without a byte order mark and contain Japanese comments, which MSVC
  misreads in other code pages.
* On Linux, link `dl` and `m`. The virtual machine loads plugins with `dlopen`.

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
