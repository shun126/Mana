# Mana
Actor-oriented scripting language Mana 

[![Build Status](https://travis-ci.org/shun126/Mana.svg?branch=master)](https://travis-ci.org/shun126/Mana)
[![Coverage Status](https://coveralls.io/repos/github/shun126/Mana/badge.svg?branch=master)](https://coveralls.io/github/shun126/Mana?branch=master)

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
* [bison](https://www.gnu.org/software/bison/)
* [flex](https://www.gnu.org/software/flex/)
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
- Install Microsoft Visual C++ 2019 Community (should work with other versions).
- Run "Vistual Studio 2019 Command Prompt" from the "Visual Studio 2019" start menu.
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

---
Shun Moriya
