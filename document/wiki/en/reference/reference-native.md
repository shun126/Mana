# Native Function

`native` is a declaration for calling, from a Mana script, an external function registered on the C++ side.

## Global native functions

```mana
native int nativeAdd(int a, int b);
```

A `native` function has only a declaration; you don't write a body on the Mana side.

```mana
actor Main
{
    action main
    {
        int value = nativeAdd(10, 20);
        print("%d\n", value);
    }
}
```

At run time, the VM looks up the registered C++ function by the function's name and calls it.

On the C++ side it can be registered with, for example, `RegisterFunction`.

```cpp
vm->RegisterFunction("nativeAdd", &OnNativeAdd);
```

## Native methods on Structs

`native` can also be declared as a Struct member.

```mana
struct Vec
{
    float x;
    float y;

    native void normalize();
}

void update(Vec value)
{
    value.normalize();
}
```

A Struct's native method is resolved as an external function named in the form `StructName::methodName`.

```text
Vec::normalize
```

The VM's external function callback receives a pointer to the Struct instance, in addition to the running Actor.

## Declaration syntax

```mana
native return-type functionName(arguments...);
```

Inside a Struct, the form is:

```mana
struct TypeName
{
    native return-type methodName(arguments...);
}
```

## How it differs from an ordinary Function

An ordinary Mana Function runs by branching to the instructions the Mana Compiler generated.

A `native` Function has no body on the Mana side; at run time a registered external function is looked up by name.

## The registration type on the C++ side

The current VM's basic callback type has this form:

```cpp
std::function<void(const std::shared_ptr<mana::Actor>& actor,
                   void* structPointer)>
```

Arguments and return values are passed through the Actor's API for external functions and the VM stack. The details are covered in Native Functions in Integration.

## When the external function cannot be found

If no external function with that name is registered with the VM, the VM reports that the external function cannot be found as an error.

Make the declaration on the script side match the name registered on the C++ side.

## Points to note

`native` is the boundary between C++ and Mana. The declaration on the Mana side and the way the C++ side handles arguments and return values must match.

In particular, a Struct native method receives a pointer to the Struct instance, unlike an ordinary global native function.

## Related topics

- [Function](./reference-functions.md)
- [Struct](./reference-struct.md)
- [Predefined symbols](./reference-predefined-symbols.md)
