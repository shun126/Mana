# WebAssembly build of the Web Playground.
#
# The root CMakeLists.txt adds this directory only when Emscripten builds it.
# The compiler (manac) and the VM headers are the same ones the desktop build
# uses; everything specific to the web stays in cpp/ManaWeb.cpp.
#
#   emcmake cmake -S . -B build-web -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
#   cmake --build build-web --target mana_web
#
# It produces build-web/web/mana.js and build-web/web/mana.wasm.

add_executable(mana_web cpp/ManaWeb.cpp)
target_link_libraries(mana_web PRIVATE manac)
target_compile_options(mana_web PRIVATE -Wall)
if(NOT MANA_USE_PREGENERATED_SOURCES)
  add_dependencies(mana_web mana_generated_version)
endif()

set_target_properties(mana_web PROPERTIES
  OUTPUT_NAME mana
  SUFFIX ".js")

# A Web Worker loads the module with importScripts() and CI loads it with
# require() in Node.js. MemorySourceResolver supplies the sources, so the
# module has no file system at all.
target_link_options(mana_web PRIVATE
  "-sMODULARIZE=1"
  "-sEXPORT_NAME=createManaModule"
  "-sENVIRONMENT=web,worker,node"
  "-sFILESYSTEM=0"
  "-sALLOW_MEMORY_GROWTH=1"
  "-sSTACK_SIZE=1048576"
  "-sEXPORTED_FUNCTIONS=_main,_malloc,_free,_mana_web_reset,_mana_web_set_file,_mana_web_compile,_mana_web_start,_mana_web_step,_mana_web_stop,_mana_web_version"
  "-sEXPORTED_RUNTIME_METHODS=UTF8ToString,stringToNewUTF8")
