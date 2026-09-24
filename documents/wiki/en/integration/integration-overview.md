# Integration overview

Mana's compiler and VM can be embedded in a C++ application as libraries.

## Minimal setup

```cpp
#include "compiler/Compiler.h"
#include "runner/Mana.h"

mana::CompileOptions options;
options.mSourceFilename = "main.mn";

const mana::CompileResult result = mana::Compile(options);
if (!result.mSucceeded)
    return;

auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
auto vm = std::make_shared<mana::VM>();
vm->LoadProgram(std::shared_ptr<const void>(image, image->data()));

while (vm->Run())
{
}
```

On the embedding side, there are three broad stages.

1. Turn the source into a Program Image with `mana::Compile()`
2. If needed, check the Actors / Actions in advance with `mana::ProgramImage`
3. Load the Program Image into `mana::VM` and run it

## Keep the Compiler and VM separate

The Mana Compiler does not assume it writes files. `Compile()` returns a `CompileResult`, and the Program Image is available as a `std::vector<uint8_t>`.

So it can be embedded in in-editor compilation, asset builds, server-side builds and so on.

The VM receives a compiled Program Image and runs it. You can also leave the Compiler out of the runtime and ship only pre-built Program Images.

## Supplying source from somewhere other than files

By replacing `CompileOptions::mSourceResolver`, you can supply source from somewhere other than the file system.

For example:

- An editor's unsaved buffer
- A game engine's asset system
- Virtual files inside a package
- In-memory sources for tests

The details come later, on the SourceResolver page.

## Handling errors

Compile errors are stored in `CompileResult::mDiagnostics`.

`Compile()` is designed to catch internal exceptions and not let exceptions cross over to the C++ caller. Problems while the VM runs, on the other hand, are handled as Trace output and execution state.

## About threads

The current `Compile()` keeps global state inside the compiler, so it cannot run from several threads at the same time.

Even when building several assets in parallel, serialise the part that calls the Mana Compiler.

## Related topics

- [Compiler](./integration-compiler.md)
- [VM](./integration-vm.md)
- [Program Image](./integration-program-image.md)
