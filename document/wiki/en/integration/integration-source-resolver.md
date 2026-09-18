# SourceResolver

`mana::SourceResolver` is the interface that supplies source code to the Mana Compiler.

Normally `.mn` files are read from the file system, but you can also supply source from an editor's unsaved buffer, an asset database, data inside a package, virtual files over a network, and so on.

## How it relates to the Compiler

Set your own Resolver in `CompileOptions::mSourceResolver`.

```cpp
mana::CompileOptions options;
options.mSourceFilename = "main.mn";
options.mSourceResolver = resolver;

mana::CompileResult result = mana::Compile(options);
```

If `mSourceResolver` is left out, the default `mana::FileSourceResolver` is used.

## The interface

A custom Resolver implements these two functions.

```cpp
class SourceResolver
{
public:
    virtual std::string Resolve(
        std::string_view from,
        std::string_view filename) const = 0;

    virtual bool Read(
        std::string_view path,
        std::string& outText) const = 0;
};
```

Their roles are clearly separate.

- `Resolve()` : decides, from the referring source and the given name, the location that identifies the source
- `Read()` : gets the actual source text from the resolved location

## FileSourceResolver

The standard `FileSourceResolver` reads from the file system.

```text
project/
├─ main.mn
└─ actor/
   └─ npc.mn
```

If `main.mn` contains

```mana
import "actor/npc.mn";
```

the relative path is resolved from the directory that contains `main.mn`.

If `actor/npc.mn` then reads another file, the location of `npc.mn` becomes the base.

Only the first source is resolved from the current working directory.

## Supplying source from memory

For example, to compile an editor's unsaved contents directly, you can write a Resolver like this.

```cpp
class MemorySourceResolver final : public mana::SourceResolver
{
public:
    std::map<std::string, std::string, std::less<>> files;

    std::string Resolve(
        std::string_view,
        std::string_view filename) const override
    {
        return std::string(filename);
    }

    bool Read(
        std::string_view path,
        std::string& outText) const override
    {
        const auto it = files.find(path);
        if (it == files.end())
            return false;

        outText = it->second;
        return true;
    }
};
```

The calling side looks like this.

```cpp
auto resolver = std::make_shared<MemorySourceResolver>();
resolver->files["main.mn"] = R"(
actor Main
{
    action main
    {
        print("Hello\n");
    }
}
)";

mana::CompileOptions options;
options.mSourceFilename = "main.mn";
options.mSourceResolver = resolver;

const mana::CompileResult result = mana::Compile(options);
```

## import and unique paths

`import` does not read a source with the same resolved path twice.

So in a custom `SourceResolver`, it is important to return, as far as possible, the same resolved string for names that refer to the same source.

For example, if these two refer to the same data

```text
scripts/npc.mn
scripts/./npc.mn
```

but you return them as different resolved results, the Compiler may see them as different sources.

We recommend normalising to a stable identifier, such as an asset ID or a normalised virtual path.

## Read failures

If `Resolve()` returns an empty string, the Compiler diagnoses that the location given could not be resolved.

If `Resolve()` returns a location but `Read()` returns `false`, it diagnoses that the resolved location could not be opened.

In editor integrations, returning from `Resolve()` a logical path that reads well in diagnostics makes it easy to show users which virtual file had the problem.

## Line endings

The Resolver does not need to unify the line endings of the string `Read()` returns.

The Compiler's Lexer normalises them to LF after reading.

## Forced reading, equivalent to `-I`

Sources given in `CompileOptions::mForcedIncludeFiles` are also read through the same `SourceResolver`.

```cpp
options.mForcedIncludeFiles.push_back("common.mn");
```

This is equivalent to `-I common.mn` in the CLI.

## Example uses

Replacing the SourceResolver enables integrations such as:

- Compiling an unsaved Mana script in a game editor as it is
- Supplying source from assets in Unreal Engine and similar engines
- Reading from inside packages such as zip / pak
- Giving source to unit tests without file I/O
- Separating logical paths from where files actually are

## Related topics

- [Compiler](./integration-compiler.md)
- [Diagnostics](./integration-diagnostics.md)
- [Source files and import / include](../reference/reference-source-files.md)
