# Setting up Mana

The goal of this page is to run `mana --version` in a terminal and see the version information.

Here you **build** the tool from the repository. Building means turning the C++ sources that implement Mana into an executable you can use. It is not something you do every time you write a Mana script.

## 1. Get the repository

If you have Git, run the following in a terminal. Git is a tool for fetching and managing source code.

```text
git clone https://github.com/shun126/Mana.git
cd Mana
```

`cd` is the command that changes the folder you are working in. In these lessons, the folder you have just entered, the one containing `CMakeLists.txt`, is called the **Mana folder**.

If you do not use Git, download the repository's source as a ZIP, extract it, and open a terminal in that folder.

## 2. Build it for your operating system

### Windows / Visual Studio

You need Visual Studio 2022 or newer with C++ desktop development (MSVC v143 or newer and the Windows SDK), CMake 3.20 or newer, Python 3, Bison 3.8 or newer, and Flex 2.6.4 or newer. Bison and Flex generate the C++ code that processes Mana's grammar.

1. In the Visual Studio Installer, set up Desktop development with C++.
2. Get Bison and Flex executables that run on Windows.
3. In PowerShell in the Mana folder, run these commands. Set the required environment variables to the absolute paths of the Bison and Flex executables at their actual locations.

```powershell
$env:BISON_EXECUTABLE = "C:\path\to\bison.exe"
$env:FLEX_EXECUTABLE = "C:\path\to\flex.exe"
cmake -S . -B build -A x64
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

Both environment variables are required even if Bison and Flex are on `PATH`. To configure in the Visual Studio IDE, set them as Windows user environment variables and restart Visual Studio, or supply them in a local `CMakeSettings.json` `environments` entry. For a 32-bit build, use `-A Win32` and a separate build folder. CMake uses the newest Visual Studio it finds; to use a specific version, add `-G` with its generator name, such as `-G "Visual Studio 17 2022"`.

Once the build succeeds, check it in PowerShell in the Mana folder.

```powershell
.\build\Release\mana.exe --version
```

When the version information appears, enter the following in the same PowerShell.

```powershell
Set-Alias mana (Resolve-Path .\build\Release\mana.exe).Path
mana --version
```

Now, as long as this PowerShell stays open, you can run the tool by the short name `mana`. You do not need to change PATH. When you open a new PowerShell, run `Set-Alias` again in the Mana folder.

### Linux / CMake

Install a C++17 compiler, CMake 3.20 or newer, Make, Python 3, Bison 3.8 or newer, and Flex 2.6.4 or newer. Follow your Linux distribution's instructions for installing the packages.

Run the following in a terminal to check that each tool is available.

```bash
cmake --version
bison --version
flex --version
```

Build in the Mana folder.

```bash
export BISON_EXECUTABLE="$(command -v bison)"
export FLEX_EXECUTABLE="$(command -v flex)"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/mana --version
```

When the version information appears, run the following in Bash.

```bash
mana_executable="$(pwd)/build/mana"
mana() { "$mana_executable" "$@"; }
mana --version
```

From now on you can type `mana` in the same terminal. The function remembers where the executable is and passes the arguments on to it. It only lasts for the current shell, so set it up again in the Mana folder whenever you open a new terminal.

## 3. On to your first program

The rest of the lessons assume that **you work in the Mana folder and use the same terminal you set up here**. If `mana --version` works, you are ready.

If something goes wrong, check in this order.

| Situation | What to check |
| --- | --- |
| The build fails because Bison or Flex cannot be found | Whether `BISON_EXECUTABLE` and `FLEX_EXECUTABLE` point to existing executables |
| The executable cannot be found | Whether the build succeeded. On Windows, whether you built `Release / x64` |
| It works with the full path but not as `mana` | Whether you set up the short name in this terminal |
| The source file cannot be found | Whether the terminal's working folder matches where you saved the file |

Next is [Your first Mana program](./getting-started-first-program.md).
