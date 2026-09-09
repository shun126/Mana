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
reported as a fatal `mana::Diagnostic`. It also restores the working directory,
which the scanner changes while resolving includes.

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
