# Mana
Actor-oriented scripting language Mana 

[![stars](https://img.shields.io/github/stars/shun126/Mana?style=social)](https://github.com/shun126/Mana/stargazers)
[![license](https://img.shields.io/github/license/shun126/Mana)](https://github.com/shun126/Mana/blob/main/LICENSE)
[![release](https://img.shields.io/github/v/release/shun126/Mana)](https://github.com/shun126/Mana/releases)
[![downloads](https://img.shields.io/github/downloads/shun126/Mana/total)](https://github.com/shun126/Mana/releases)
[![doxygen](https://img.shields.io/badge/document-doxygen-brightgreen)](https://mnu.sakura.ne.jp/_doxygen/mana)

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
        request(1, Child::talk);
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

# Installing
## Requirements
* [bison 3.8](https://www.gnu.org/software/bison/)
* [flex 2.6.4](https://github.com/westes/flex)
* [C++ Compiler](https://en.wikipedia.org/wiki/C%2B%2B)
  * [Visual Studio](https://visualstudio.microsoft.com/)
  * [Clang](https://clang.llvm.org/)

## Any Linux Distribution
- cd to <download_path>\compiler
- make

## Building with Cygwin
- Install Cygwin from: http://www.cygwin.com/
- cd to <download_path>\compiler
- make

## Building with MSVC
- Install Microsoft Visual C++ 2022 Community (should work with other versions).
- Run "Vistual Studio 2022 Command Prompt" from the "Visual Studio 2022" start menu.
- cd to <download_path>\compiler
- Open mana.sln

## Building with Xcode
- Install Xcode 8.3.2 (should work with other versions).
- cd to <download_path>\compiler
- Open mana.xcodeproj

# Running Tests
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

---
Shun Moriya

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/M4M413XDXB)
