cmake_minimum_required(VERSION 3.20)

project(Mana LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

foreach(mana_tool IN ITEMS BISON FLEX)
  set(mana_tool_var "${mana_tool}_EXECUTABLE")
  if(NOT DEFINED ENV{${mana_tool_var}} OR "$ENV{${mana_tool_var}}" STREQUAL "")
    message(FATAL_ERROR "${mana_tool_var} environment variable must be set to the executable's absolute path")
  endif()
  if(NOT IS_ABSOLUTE "$ENV{${mana_tool_var}}" OR
     NOT EXISTS "$ENV{${mana_tool_var}}" OR
     IS_DIRECTORY "$ENV{${mana_tool_var}}")
    message(FATAL_ERROR "${mana_tool_var} must point to an existing executable: $ENV{${mana_tool_var}}")
  endif()
  set(${mana_tool_var} "$ENV{${mana_tool_var}}" CACHE FILEPATH "${mana_tool} executable" FORCE)
endforeach()

find_package(BISON 3.8 REQUIRED)
find_package(FLEX 2.6.4 REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(mana_generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${mana_generated_dir}")

BISON_TARGET(ManaParser compiler/Parser.yy
  "${mana_generated_dir}/Parser.cpp"
  DEFINES_FILE "${mana_generated_dir}/Parser.hpp")

if(WIN32)
  set(mana_flex_flags "-8")
else()
  set(mana_flex_flags "-8 -F")
endif()
FLEX_TARGET(ManaLexer compiler/Lexer.l
  "${mana_generated_dir}/Lexer.cpp"
  COMPILE_FLAGS "${mana_flex_flags}")
ADD_FLEX_BISON_DEPENDENCY(ManaLexer ManaParser)

add_custom_target(mana_generated_parser
  DEPENDS ${BISON_ManaParser_OUTPUTS} ${FLEX_ManaLexer_OUTPUTS})

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/runner/common/Version.h"
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/runner/common/Version.py"
  DEPENDS runner/common/Version.py runner/common/Version.json
  VERBATIM)
add_custom_target(mana_generated_version
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/runner/common/Version.h")

add_library(manac STATIC
  compiler/CodeBuffer.cpp
  compiler/CodeGenerator.cpp
  compiler/Compiler.cpp
  compiler/DataBuffer.cpp
  compiler/ErrorHandler.cpp
  compiler/GlobalAddressResolver.cpp
  compiler/GlobalSemanticAnalyzer.cpp
  compiler/IntermediateLanguage.cpp
  compiler/LocalAddressResolver.cpp
  compiler/LocalSemanticAnalyzer.cpp
  compiler/NamespaceRegistry.cpp
  compiler/ParsingDriver.cpp
  compiler/Path.cpp
  compiler/SemanticAnalyzer.cpp
  compiler/SourceResolver.cpp
  compiler/StringPool.cpp
  compiler/Symbol.cpp
  compiler/SymbolFactory.cpp
  compiler/SyntaxNode.cpp
  compiler/TypeDescriptor.cpp
  compiler/TypeDescriptorFactory.cpp
  ${BISON_ManaParser_OUTPUTS}
  ${FLEX_ManaLexer_OUTPUTS})
if(NOT MSVC)
  set_target_properties(manac PROPERTIES OUTPUT_NAME mana)
endif()
add_dependencies(manac mana_generated_parser)
target_include_directories(manac PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/compiler"
  "${CMAKE_CURRENT_SOURCE_DIR}/runner")
target_include_directories(manac BEFORE PUBLIC "${mana_generated_dir}")

add_executable(mana driver/Main.cpp)
target_link_libraries(mana PRIVATE manac)

add_executable(EmbeddingTest tests/cpp/EmbeddingTest.cpp)
target_link_libraries(EmbeddingTest PRIVATE manac)

add_executable(ProgramImageTest tests/cpp/ProgramImageTest.cpp)

foreach(mana_target IN ITEMS manac mana EmbeddingTest ProgramImageTest)
  add_dependencies(${mana_target} mana_generated_version)
  target_compile_definitions(${mana_target} PRIVATE "$<$<CONFIG:Debug>:DEBUG>")
  if(MSVC)
    target_compile_options(${mana_target} PRIVATE /W4 /source-charset:utf-8)
  else()
    target_compile_options(${mana_target} PRIVATE -Wall)
  endif()
endforeach()

if(UNIX AND NOT APPLE)
  target_link_libraries(mana PRIVATE ${CMAKE_DL_LIBS} m)
  target_link_libraries(EmbeddingTest PRIVATE ${CMAKE_DL_LIBS} m)
endif()

enable_testing()

add_test(NAME EmbeddingTest COMMAND EmbeddingTest)

set(mana_program_image "${CMAKE_CURRENT_BINARY_DIR}/ProgramImageTest.mx")
add_test(NAME GenerateProgramImage
  COMMAND mana "${CMAKE_CURRENT_SOURCE_DIR}/tests/cpp/ProgramImageTest.mn"
          -o "${mana_program_image}")
set_tests_properties(GenerateProgramImage PROPERTIES FIXTURES_SETUP ProgramImage)
add_test(NAME ProgramImageTest COMMAND ProgramImageTest "${mana_program_image}")
set_tests_properties(ProgramImageTest PROPERTIES FIXTURES_REQUIRED ProgramImage)

add_test(NAME LanguageTest
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/mana/test.py"
          "$<TARGET_FILE:mana>")
set_tests_properties(LanguageTest PROPERTIES
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/mana")

add_test(NAME TutorialExamples
  COMMAND "${Python3_EXECUTABLE}"
          "${CMAKE_CURRENT_SOURCE_DIR}/documents/tools/verify-examples.py"
          "$<TARGET_FILE:mana>")
set_tests_properties(TutorialExamples PROPERTIES
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_test(NAME LanguageTour
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/check_output.py"
          "${CMAKE_CURRENT_SOURCE_DIR}/examples/language-tour/expected-output.txt"
          "$<TARGET_FILE:mana>" --debug
          "${CMAKE_CURRENT_SOURCE_DIR}/examples/language-tour/main.mn"
          -I natives.mh)
set_tests_properties(LanguageTour PROPERTIES
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
