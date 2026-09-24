# Source files and import / include

Mana source files normally use the `.mn` extension.

In a large program, you can bring several source files into one compilation with `import` or `include`.

## import

```mana
import "npc.mn";
```

`import` reads the given source. If a source with the same resolved path has already been read, the current Lexer skips reading it a second time or more.

So `import` is recommended for ordinary splitting into files.

```text
main.mn
 ├─ import "npc.mn"
 └─ import "event.mn"
```

The definitions brought in are combined into the same compilation result, and one Program Image is produced.

## include

```mana
include "common.mn";
```

`include` also reads the given source, but unlike `import` it does not prevent reading the same file more than once.

If you `include` the same file several times, the same declarations are analysed several times, which can cause duplicate definition errors.

Normally use `import`, and consider `include` only when you deliberately need to read the same source again.

## Resolving paths

The path of the file to read is resolved by the SourceResolver.

With the standard file-based use, relative paths can be resolved from the location of the source file currently being read.

```text
project/
├─ main.mn
└─ actors/
   └─ npc.mn
```

```mana
import "actors/npc.mn";
```

When a custom SourceResolver is used in an embedding, the actual path resolution rules depend on that implementation.

## Forward references

Mana builds the syntax tree including the sources brought in, and then performs semantic analysis of symbols and namespaces.

So even if Actors, Modules, namespaces and so on are split across files, the design does not stop you referring to a resolvable name just because of the order of definition.

```mana
// main.mn
import "enemy.mn";

actor Controller
{
    action main
    {
        request(1, Enemy->think);
    }
}
```

```mana
// enemy.mn
actor Enemy
{
    action think
    {
    }
}
```

## Independent of namespaces

Namespaces are never generated automatically from file names or directory structure.

Creating a file named

```text
actors/enemy.mn
```

does not automatically make it `actors::Enemy`.

To organise names logically, use `namespace` explicitly.

## Forced includes in the CLI

The `mana` command has `-I filename`, which forces a file into the compilation without changing the source code.

```bash
mana -I common.mn main.mn
```

`-I` can be given several times.

## Related topics

- [Namespace and using](./reference-namespace.md)
- [CLI](./reference-cli.md)
- [Source code structure](./reference-source-code.md)
