# Implementation-Based Reference (Undocumented Features)

This document summarizes behavior that can be read directly from the current Mana implementation. It does **not** introduce new specifications or change existing behavior. Anything that cannot be confirmed from the implementation is explicitly marked as **undefined / implementation-dependent**.

## native functions

### Overview (purpose and role)
- Functions declared with `native` are entry points that allow Mana scripts to call C++ external functions through the VM.
- There is no Mana-side function body; the call is resolved by name to an external function registered in the VM.

### How to declare in Mana scripts
- Only a prototype declaration with the `native` keyword is allowed; **no function body** is provided.
- Declarations are allowed both at global scope and as members inside `struct` definitions.

```mana
native void waitFrame(int frame);

struct Foo
{
    native int bar(int x);
}
```

### Runtime flow (Mana → VM → C++)
1. At compile time, a `native` declaration is registered as a native function, and calls are emitted as the `Call` instruction.
2. At runtime, the VM looks up the external function by its name string and, if found, invokes the registered C++ function.
3. Arguments are passed via the VM stack; the external function reads parameters via the `Actor` API. Return values are set via `Actor::SetReturn*`, then pushed back onto the VM stack.

### Differences from normal Mana functions
- Mana functions branch to compiled bytecode, while native functions **lookup and dispatch by name** at runtime.
- Native functions are declarations only (no body in scripts).
- Native functions declared as `struct` members are resolved at runtime by the external name `"StructName::FunctionName"` and receive a `this`-equivalent pointer.

### Constraints and cautions
- Argument **count** is checked, but type safety is up to the C++ implementation (behavior on type mismatch is implementation-dependent).
- If an external function is not found, the VM logs an error. If the call expects a return value, execution terminates; if the return type is `void`, execution continues.
- Declaring `native` inside deep blocks (e.g., inside a function) raises an error (prototype declarations are ignored in that context).
- Incomplete types (`incomplete`) cannot be used as native function return types.

### Undefined / implementation-dependent
- Argument type conversions, type safety, and propagation of C++ exceptions or errors are **not specified**.
- Operational rules for external function registration (plugins, loading order, etc.) are **implementation-dependent** in this document.

## namespace / using

### Role of namespace
- `namespace` provides **hierarchical scoping** to separate symbol names.
- Namespaces are registered during semantic analysis and resolved as qualified names such as `A::B::C`.

### Basic meaning of using
- `using` adds **another namespace or symbol** to the current resolution scope.
- `using` is resolved during semantic analysis and is effective within the current namespace scope (inner scopes take precedence).

### Difference between `using N;` and `using A::B;`
- `using N;`
  - Adds `N` as a namespace search path.
  - Unqualified names can be resolved as `N::X`.
- `using A::B;`
  - If `A::B` is a namespace, it is added as a namespace search path.
  - If `A::B` is an **actor / module symbol**, the last segment (`B`) is registered as an **alias**, allowing `B` to be used as an unqualified name.
  - If `A::B` is neither a namespace nor an actor/module symbol, an `unresolved using` error occurs.

### When name resolution happens (semantic analysis)
- Name resolution occurs **after parsing**, during semantic analysis.
- This is why `using` and `namespace` can be referenced before their definitions.

### Why forward references are possible
- Mana treats multiple source files as a single compile unit and resolves names after parsing.
- Namespaces are registered during analysis, so `using` can refer to namespaces defined later.

### Behavior on name resolution failure
- Ambiguous `using` declarations produce an **`ambiguous using` error**.
- Unresolvable `using` declarations produce an **`unresolved using` error**.
- If multiple candidates remain for a symbol/type/actor, an **`ambiguous symbol/type/actor reference` error** is raised.

### Undefined / constrained points
- `using` only resolves **namespaces** and **actor/module symbols**. Behavior for other kinds of symbols is undefined.
- Conflict resolution rules (beyond “inner scope wins”) are **implementation-dependent**.

## Differences between actor and phantom

### What each concept represents
- `actor` is an **instance created at VM startup** and becomes an executable entity.
- `phantom` is an **actor template** and is *not* instantiated at VM startup.

### Roles and typical use
- `actor` participates in normal execution (`init` / `main`, etc.).
- `phantom` is intended to be instantiated explicitly from C++ code when needed.

### Definition and usage differences
- Both are defined with the same syntax but different keywords (`actor` vs `phantom`).
- `phantom` does not create a runtime instance by itself, so the script alone does not execute it.

### Runtime (VM) handling differences
- On program load, the VM instantiates `actor` entries and registers them in the actor list.
- `phantom` entries keep only name and action metadata; they are instantiated only when C++ calls `CreateActorFromPhantom`.

### Why the concepts are separated (design intent)
- Implementation-wise, `phantom` is treated as a template for **duplicating actor definitions** at runtime.
- This allows actors to be created on demand.

### Guidance for choosing between them
- Use `actor` for entities that should exist and initialize at startup.
- Use `phantom` for entities that should be created dynamically from C++.

### Undefined / future-facing points
- Creation, destruction, and lifecycle rules for phantoms are **implementation-dependent** on the C++ side.
- There is **no specified script-side API** to instantiate a phantom.
