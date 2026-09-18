# Program Image

`mana::ProgramImage` is a class for loading a compiled Program Image and examining the Actors / Actions / Phantoms it contains before running it.

Whereas `mana::VM` is the API for "running", `ProgramImage` is the API for "examining the contents".

## Loading

```cpp
auto bytes = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);

mana::ProgramImage image;
const bool loaded = image.LoadProgram(
    std::shared_ptr<const void>(bytes, bytes->data()),
    bytes->size());
```

The result of loading is returned as a `bool`.

```cpp
if (!loaded)
{
    std::cerr << image.GetLastError() << '\n';
}
```

You can also check the state with `IsLoaded()`.

## Getting the list of Actors

```cpp
for (std::string_view name : image.GetActorNames())
{
    std::cout << name << '\n';
}
```

You can also check whether a particular Actor exists.

```cpp
if (image.HasActor("Game::NPC::Guide"))
{
}
```

## Examining an Actor's Actions

```cpp
for (std::string_view action :
     image.GetActorActionNames("Game::NPC::Guide"))
{
    std::cout << action << '\n';
}
```

```cpp
if (image.HasActorAction("Game::NPC::Guide", "talk"))
{
}
```

Use it to validate names before the game sends a Request by name, or to build lists of candidates in an editor UI.

## Examining Phantoms

For Phantoms too, you can check existence and get the list of Actions.

```cpp
if (image.HasPhantom("EnemyTemplate"))
{
    const auto actions =
        image.GetPhantomActionNames("EnemyTemplate");
}
```

```cpp
image.HasPhantomAction("EnemyTemplate", "damage");
```

## Lifetime of the Program Image

`ProgramImage` holds the loaded bytes as a `shared_ptr<const void>`.

Actor and Action names are returned as `std::string_view`, so these views depend on the lifetime of the data the Program Image holds. Don't keep a `string_view` you got for longer than the `ProgramImage` it came from.

## Uses

`ProgramImage` is especially suited to:

- Showing the list of Actors in an editor
- Choosing an Action name from a combo box
- Validating in advance the Actor / Action names given in C++-side settings
- Listing candidates for creating Phantoms
- Inspecting a Program Image without running it

## How it differs from the VM

| | `ProgramImage` | `VM` |
| --- | --- | --- |
| Reads a Program Image | Yes | Yes |
| Examines lists of Actors / Actions | Its main use | Has some lookup APIs |
| Runs Actions | No | Yes |
| Sends Requests | No | Yes |
| Registers Native Functions | No | Yes |

## Related topics

- [Compiler](./integration-compiler.md)
- [VM](./integration-vm.md)
- [Actor](../reference/reference-actor.md)
- [Action](../reference/reference-action.md)
