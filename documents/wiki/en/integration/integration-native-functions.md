# Native Functions

Native Functions are the boundary for calling a game's or tool's C++ code from a Mana script.

On the Mana side you declare a function with `native`, and on the C++ side you register a function with the same name with `mana::VM`.

## Minimal setup

On the Mana side, declare it like this.

```mana
native int add(int a, int b);

actor Main
{
    action main
    {
        int result = add(10, 20);
        print("%d\n", result);
    }
}
```

On the C++ side, register the external function.

```cpp
void Add(const std::shared_ptr<mana::Actor>& actor, void*)
{
    const int32_t a = actor->GetParameterInteger(0);
    const int32_t b = actor->GetParameterInteger(1);
    actor->SetReturnInteger(a + b);
}

std::shared_ptr<mana::VM> vm = std::make_shared<mana::VM>();
vm->RegisterFunction("add", &Add);
```

Load a Program Image into the VM you registered with and `Run()` as usual, and a call to `add()` in Mana runs the C++ `Add()`.

## The external function type

The current VM's external function type has this form:

```cpp
using ExternalFunctionType =
    std::function<void(const std::shared_ptr<mana::Actor>& actor,
                       void* structPointer)>;
```

The first argument, `actor`, is the Actor running that native function.

The second argument, `structPointer`, is used to refer to the Struct instance when a Struct's native method was called.

## Getting the arguments

A Native Function's arguments are taken from the running Actor.

The typical APIs are:

```cpp
actor->GetParameterInteger(index);
actor->GetParameterFloat(index);
actor->GetParameterString(index);
actor->GetParameterActor(index);
actor->GetParameterPointer(index);
actor->GetParameterAddress(index);
```

The number of arguments is available through:

```cpp
const int32_t count = actor->GetArgumentCount();
```

Keep the types and order read on the C++ side matching the declaration on the Mana side, as a contract of the embedding.

## Returning a value

For a native function with a return value, use `SetReturn*()`.

```cpp
actor->SetReturnInteger(value);
actor->SetReturnFloat(value);
actor->SetReturnString(text);
actor->SetReturnActor(otherActor);
actor->SetReturnPointer(pointer);
actor->SetReturnData(data, size);
```

For example, if the Mana side is

```mana
native float getSpeed();
```

then the C++ side sets the value like this.

```cpp
void GetSpeed(const std::shared_ptr<mana::Actor>& actor, void*)
{
    actor->SetReturnFloat(3.5f);
}
```

## Native methods on Structs

In Mana you can declare a native function as a member of a Struct.

```mana
struct Position
{
    float x;
    float y;

    native void normalize();
}
```

In this case, the external name resolved on the C++ side is `StructName::methodName`.

```cpp
vm->RegisterFunction("Position::normalize", &NormalizePosition);
```

The callback's second argument, `structPointer`, receives the address of the target Struct.

```cpp
void NormalizePosition(const std::shared_ptr<mana::Actor>&,
                       void* structPointer)
{
    // Treat structPointer as the matching layout on the host side
}
```

If you handle a Struct's memory layout directly on the C++ side, keep it strictly in step with the type definition on the Mana side.

## Registering C++ member functions

With `VM::RegisterMemberFunction()`, you can register a C++ object's member function without a wrapper.

```cpp
class GameBridge
{
public:
    void PlaySound(const std::shared_ptr<mana::Actor>& actor, void* structPointer)
    {
        // Game-side work
    }
};

auto bridge = std::make_shared<GameBridge>();
vm->RegisterMemberFunction("playSound", bridge, &GameBridge::PlaySound);
```

The current API has overloads that take a raw pointer, a `shared_ptr` and a `weak_ptr`.

The overload that takes a `shared_ptr` keeps it internally as a weak reference. If the registered object is destroyed first, an error is written to the Trace when it is called.

## Make the registered names match

At run time the VM looks up external functions by their string names.

If the name declared on the Mana side and the name registered with `RegisterFunction()` don't match, the VM reports in the Error Trace that the external function cannot be found.

When embedding, we recommend registering every Native Function you need before loading and running the Program Image.

## Its role in the design

Native Functions are not a way of bringing rendering, physics, sound, asset management and so on into Mana itself.

Designing them as a boundary that leaves those in the host application and exposes to Mana only the operations it needs makes it easy to keep the responsibilities of scripts and the game engine apart.

## Related topics

- [VM](./integration-vm.md)
- [Program Image](./integration-program-image.md)
- [Native Function reference](../reference/reference-native.md)
- [Error Handling](./integration-error-handling.md)
