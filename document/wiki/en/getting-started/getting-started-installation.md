# Setting up Mana

The goal of this page is to run `mana --version` in a terminal and see the version information.

Here you **build** the tool from the repository. Building means turning the C++ sources that implement Mana into an executable you can use. It is not something you do every time you write a Mana script.

## 1. Get the repository

If you have Git, run the following in a terminal. Git is a tool for fetching and managing source code.

```text
git clone https://github.com/shun126/Mana.git
cd Mana
```

`cd` is the command that changes the folder you are working in. In these lessons, the folder you have just entered, the one containing `mana.sln` and `Makefile`, is called the **Mana folder**.

If you do not use Git, download the repository's source as a ZIP, extract it, and open a terminal in that folder.

## 2. Build it for your operating system

### Windows / Visual Studio

You need Visual Studio 2022 with C++ desktop development (MSVC v143 and the Windows SDK), Bison 3.8 and Flex 2.6.4. Bison and Flex generate the C++ code that processes Mana's grammar.

1. In the Visual Studio Installer, set up Desktop development with C++.
2. Get Bison and Flex executables that run on Windows.
3. In Windows "Environment Variables", set the following user environment variables.
4. After setting them, reopen Visual Studio and open `mana.sln`.
5. Set the configuration to `Release` and the platform to `x64`, then build the `mana` project in Solution Explorer. The `manac` project it depends on is built too.

| Environment variable | Example value |
| --- | --- |
| `GNU_BISON_BIN` | `C:\tools\bison\bin\bison.exe` |
| `GNU_FLEX_BIN` | `C:\tools\flex\bin\flex.exe` |

Match the values to where the tools are on your machine, and give an **absolute path that includes the executable name**. The build currently calls Bison without quoting the path, so place Bison in a path with no spaces.

If you also want to update the version information header, install Python 3 and set `PYTHON_BIN` to the path of the Python executable. This call also needs a path with no spaces. If it is not set, the header included in the repository is used.

Once the build succeeds, check it in PowerShell in the Mana folder.

```powershell
.\x64\Release\mana.exe --version
```

When the version information appears, enter the following in the same PowerShell.

```powershell
Set-Alias mana (Resolve-Path .\x64\Release\mana.exe).Path
mana --version
```

Now, as long as this PowerShell stays open, you can run the tool by the short name `mana`. You do not need to change PATH. When you open a new PowerShell, run `Set-Alias` again in the Mana folder.

### Linux / make

Install Clang with C++17 support, make, Bison 3.8 and Flex 2.6.4. Follow your Linux distribution's instructions for installing the packages.

Run the following in a terminal to check that each tool is available.

```bash
clang++ --version
make --version
bison --version
flex --version
```

Build in the Mana folder.

```bash
make
./driver/mana --version
```

When the version information appears, run the following in Bash.

```bash
mana_executable="$(pwd)/driver/mana"
mana() { "$mana_executable" "$@"; }
mana --version
```

From now on you can type `mana` in the same terminal. The function remembers where the executable is and passes the arguments on to it. It only lasts for the current shell, so set it up again in the Mana folder whenever you open a new terminal.

## 3. On to your first program

The rest of the lessons assume that **you work in the Mana folder and use the same terminal you set up here**. If `mana --version` works, you are ready.

If something goes wrong, check in this order.

| Situation | What to check |
| --- | --- |
| The build fails because Bison or Flex cannot be found | Where they are installed and the environment variables. On Windows, whether the value goes all the way to the executable |
| The executable cannot be found | Whether the build succeeded. On Windows, whether you chose `Release / x64` |
| It works with the full path but not as `mana` | Whether you set up the short name in this terminal |
| The source file cannot be found | Whether the terminal's working folder matches where you saved the file |

Next is [Your first Mana program](./getting-started-first-program.md).
