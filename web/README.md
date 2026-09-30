# Mana Web Playground

The Playground at <https://shun126.github.io/Mana/playground/> lets anyone
edit and run Mana in a browser. It is not a separate implementation: the Mana
compiler and VM in `compiler/` and `runner/` are built with Emscripten into
`mana.wasm`, and everything runs in the reader's browser. There is no server
side; GitHub Pages serves static files only.

```text
Page (playground.js)  --postMessage-->  Web Worker (worker.js)
                                            |
                                     mana-runtime.js
                                            |
                                  mana.wasm (web/cpp/ManaWeb.cpp)
                                   mana::Compile()  ->  program image  ->  mana::VM
                                            |
Output panel  <--postMessage--  print() / Trace  (mana::SetTraceHandler)
```

## Files

| Path | What it is |
| --- | --- |
| `web/cpp/ManaWeb.cpp` | The WebAssembly bridge: a few `extern "C"` functions around `mana::Compile()` and `mana::VM`. All web-specific C++ lives here. |
| `web/cpp/MemorySourceResolver.h` | A `mana::SourceResolver` that serves sources from a `filename -> text` map, so the compiler never touches a file system. `include` works between in-memory files. |
| `web/CMakeLists.txt` | The `mana_web` target. The root `CMakeLists.txt` adds it only under Emscripten. |
| `web/emscripten-version.txt` | The pinned Emscripten version. CI and local builds should use this one. |
| `web/tests/` | Node.js tests for the module and the worker, and the expected output of the examples. |
| `documents/pages/playground/` | The static page: `index.html`, `playground.js` (UI), `worker.js`, `mana-runtime.js`, `playground.css`, and `examples/*.mn`. |

The only changes outside these directories are small and apply to the web
build alone: `runner/common/Platform.h` defines `MANA_TARGET_WEB` under
Emscripten, and `runner/Plugin.inl` then never loads native plugins.

## How a run works

1. The page posts `{ type: "run", files: { "main.mn": source } }` to the worker.
2. The worker hands the files to `MemorySourceResolver` and calls
   `mana::Compile()`. The program image stays in memory; nothing is written
   to disk.
3. Diagnostics come back as data (`severity`, `phase`, `filename`, `line`,
   `message`) and the page lists them apart from the output. Clicking one
   selects its line in the editor.
4. On success the worker loads the image into a new `mana::VM` and calls
   `VM::Run()` in 20 ms slices, sending buffered `print()` output between
   slices.
5. The page stops a run after 5 seconds, or when **Stop** is pressed, by
   terminating the worker. A new worker starts at once, so the next **Run**
   works as usual. The VM needs no cancellation support for this.

Output beyond 200,000 characters is dropped so a script that prints in a loop
cannot flood the page.

## Building

You need CMake 3.20 or newer, Ninja, Python 3, Bison 3.8 or newer, Flex 2.6.4
or newer, and Emscripten at the version in `web/emscripten-version.txt`.
Bison, Flex, and Python run at build time only, exactly as in the desktop
build; the browser never needs them.

Install Emscripten with [emsdk](https://emscripten.org/docs/getting_started/downloads.html):

```sh
git clone --branch 6.0.10 https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install 6.0.10
./emsdk activate 6.0.10
source ./emsdk_env.sh        # Windows PowerShell: .\emsdk_env.ps1
```

Then, from the repository root on Linux or macOS:

```sh
export BISON_EXECUTABLE="$(command -v bison)"
export FLEX_EXECUTABLE="$(command -v flex)"
emcmake cmake -S . -B build-web -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build-web --target mana_web
```

On Windows, in a shell where `emsdk_env` has run (Visual Studio ships Ninja,
and Cygwin or MSYS2 can provide Bison and Flex):

```powershell
$env:BISON_EXECUTABLE = "C:\path\to\bison.exe"
$env:FLEX_EXECUTABLE = "C:\path\to\flex.exe"
emcmake cmake -S . -B build-web -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build-web --target mana_web
```

The build writes `build-web/web/mana.js` and `build-web/web/mana.wasm`.

## Trying it locally

Build the site with the module, then serve it over HTTP. Browsers do not run
Web Workers or WebAssembly from `file://` pages.

```sh
python documents/tools/build-pages.py --output build/pages --playground-wasm build-web/web
python -m http.server 8000 --directory build/pages
```

Open <http://localhost:8000/playground/>. To pick up changes to the page
alone, rerun `build-pages.py`; the C++ side needs `cmake --build` first.

## Testing

```sh
node web/tests/run-tests.js build-web/web
```

The Node.js that emsdk installs is enough. The tests:

* compile and run every example and compare its output with
  `web/tests/expected/`;
* check that syntax and semantic errors come back as diagnostics with line
  numbers, and that the next compile still works;
* compile several in-memory files that include each other;
* run the real `worker.js` in a worker thread, including stopping an endless
  script with `terminate()` and running again in a fresh worker.

The desktop CTest suite runs the same examples through the native `mana`
executable against the same expected output (`PlaygroundExample.*`), so the
browser and the desktop must agree.

## Changing the examples

The examples in `documents/pages/playground/examples/` are also listed in
`playground.js`, `web/tests/run-tests.js`, and the root `CMakeLists.txt`.
After editing one, regenerate its expected output with the desktop build and
check that it reads well:

```sh
./build/mana documents/pages/playground/examples/request.mn > web/tests/expected/request.txt
```

## Publishing

`.github/workflows/publish-pages.yml` installs the pinned Emscripten, builds
`mana_web`, runs the tests above, and only then builds the site with
`--playground-wasm` and deploys it to GitHub Pages. It runs whenever the
compiler, the VM, `web/`, or the site changes, so the published Playground
always matches `master`.

## Not in this version

Accounts, saving, sharing, a debugger, native plugins, and Unreal Engine
features are out of scope. The bridge already compiles a set of files, so
multiple files, sharing code through a compressed URL fragment, and a richer
editor can be added to the page later without changing the C++ side.
