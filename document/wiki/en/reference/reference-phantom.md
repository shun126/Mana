# Phantom

A `phantom` is a template that creates no instance when the VM starts, from which the C++ side creates Actors when it needs them.

## Syntax

```mana
phantom EnemyTemplate
{
    int mHp;

    action main
    {
    }

    action damage
    {
    }
}
```

Grammatically its body has the same form as an `actor`, and can define Actions and members.

## How it differs from an Actor

For an ordinary `actor`, the VM creates an instance when the Program Image is loaded and registers it in the list of Actors.

A `phantom` is not created in the list of Actors at load time. The VM keeps the Phantom's definition information and creates an Actor instance when the C++ side explicitly asks for one.

| | `actor` | `phantom` |
| --- | --- | --- |
| Created when the VM loads | Yes | No |
| Defines Actions | Can | Can |
| Has Actor variables | Can | Can |
| Main use | A resident unit of execution | A template for dynamic creation |

## Creating it from C++

The current VM API uses `CreateActorFromPhantom`.

```cpp
std::shared_ptr<mana::Actor> enemy =
    vm->CreateActorFromPhantom("EnemyTemplate", "Enemy01");
```

The first argument is the Phantom's definition name, and the second is the name of the Actor to create.

The created Actor is registered in the VM's list of Actors, and has the Actions and Actor variable area defined in the Phantom.

## How it differs from automatic startup

When the VM loads a Program Image, it sends `init` and `main` Requests to the ordinary Actors.

A Phantom has not been created as an Actor at that point, so it is not part of these Requests made all at once at load time.

How a dynamically created Actor is initialised and started should be designed together with the C++ code that creates it and the way it uses the VM API.

## Creating one from a script

The current language has no syntax for instantiating a Phantom directly from a Mana script.

Creating Phantoms is the responsibility of the host's C++ API.

## Errors

If a Phantom name that doesn't exist is passed to `CreateActorFromPhantom`, the VM raises a `Phantom not found` runtime error.

## Related topics

- [Actor](./reference-actor.md)
- [Module](./reference-module.md)
- [Compiler and VM](../concepts/concept-compiler-and-vm.md)
