# CMake Files: A Practical, Modern Guide (Reference + Learning Notes)

This is a learn-and-reference guide for writing `CMakeLists.txt` and related CMake files.
It focuses on "modern CMake": targets, usage requirements, and dependency-aware builds.

If you only remember one thing:

- Prefer *targets* over global flags.
- Put include paths/defines/options on the target that needs them.
- Make dependencies explicit with `target_link_libraries()` and let properties propagate.

---

## Table of Contents

- Basics
- How CMake Configures and Builds
- Minimum Version, Policies, and Project
- Targets (Executables, Libraries)
- Include Paths, Compile Options, Definitions
- PUBLIC / PRIVATE / INTERFACE (Usage Requirements)
- Source Organization and File Globs
- Build Types, Multi-Config vs Single-Config
- Options and Cache Variables
- Functions, Macros, and Scope
- Generator Expressions
- Output Layout and Runtime Paths
- Dependencies
- Testing with CTest
- Install Rules
- Packaging with CPack
- Presets (`CMakePresets.json`)
- Toolchains and Cross-Compiling
- Common C++ Workflows (sanitizers, LTO, clang-tidy)
- Common Pitfalls and Debugging
- Starter Templates
- Glossary

---

## Basics

### What is CMake?

CMake is a *build system generator*. You write CMake scripts (usually `CMakeLists.txt`) that:

- Describe build targets and their dependencies
- Configure compiler/linker settings
- Find and integrate libraries
- Generate native build files (Ninja, Makefiles, Visual Studio solutions, Xcode projects, etc.)

### Typical files

- `CMakeLists.txt`: main entry file in a directory
- `cmake/*.cmake`: helper modules you write
- `CMakePresets.json`: reproducible configure/build/test presets
- Toolchain file: `toolchains/<name>.cmake` for cross-compiling or compiler selection

---

## How CMake Configures and Builds

CMake is a two-phase process:

1. Configure: read CMake scripts, detect tools, generate build system
2. Build: run the generated build tool

Common CLI usage:

```sh
# Configure (out-of-source build recommended)
cmake -S . -B build -G Ninja

# Build
cmake --build build

# Run tests
ctest --test-dir build

# Install into a staging dir
cmake --install build --prefix "$(pwd)/stage"
```

Out-of-source builds keep build artifacts out of your source tree:

- Source: your repo (contains `CMakeLists.txt`)
- Build dir: `build/` (contains cache, generated files, object files)

---

## Minimum Version, Policies, and Project

### `cmake_minimum_required()`

Always set a minimum.

```cmake
cmake_minimum_required(VERSION 3.24)
```

This also selects a policy set compatible with that version.

### `project()`

Defines your project name, version, and enabled languages.

```cmake
project(MyApp VERSION 1.2.0 LANGUAGES CXX)
```

If you do both C and C++:

```cmake
project(MyProj VERSION 0.1 LANGUAGES C CXX)
```

### Language standards

Prefer target properties:

```cmake
add_executable(app main.cpp)
target_compile_features(app PRIVATE cxx_std_20)
```

Alternative:

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

The target approach scales better and plays nicer with dependencies.

---

## Targets (Executables, Libraries)

Targets are the core abstraction in modern CMake.

### Executables

```cmake
add_executable(my_tool
  src/main.cpp
  src/args.cpp
)
```

### Libraries

Static:

```cmake
add_library(mylib STATIC
  src/lib.cpp
)
```

Shared:

```cmake
add_library(mylib SHARED
  src/lib.cpp
)
```

Header-only / interface library:

```cmake
add_library(mylib INTERFACE)
target_include_directories(mylib INTERFACE include)
```

### Aliases

Aliases provide stable names (especially for exported targets):

```cmake
add_library(myproj::mylib ALIAS mylib)
```

### Object libraries

Useful for sharing compiled objects across multiple targets:

```cmake
add_library(core_objs OBJECT src/core.cpp)
add_executable(app src/main.cpp $<TARGET_OBJECTS:core_objs>)
add_library(core STATIC $<TARGET_OBJECTS:core_objs>)
```

---

## Include Paths, Compile Options, Definitions

### Include directories

Prefer `target_include_directories()`:

```cmake
target_include_directories(mylib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
```

Avoid global `include_directories()` unless you have a legacy reason.

### Compiler options

```cmake
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic)
```

For MSVC you typically add different flags:

```cmake
target_compile_options(app PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
```

### Preprocessor definitions

```cmake
target_compile_definitions(app PRIVATE APP_ENABLE_LOGGING=1)
```

If consumers need a definition too:

```cmake
target_compile_definitions(mylib PUBLIC MYLIB_USE_FAST_PATH)
```

### Link options

```cmake
target_link_options(app PRIVATE -Wl,--as-needed)
```

Use sparingly and prefer feature-based control when possible.

---

## PUBLIC / PRIVATE / INTERFACE (Usage Requirements)

These keywords control propagation of properties from one target to targets that link to it.

- `PRIVATE`: applies only to the target itself
- `PUBLIC`: applies to the target and consumers
- `INTERFACE`: applies only to consumers (typical for header-only libs)

Example:

```cmake
add_library(math STATIC src/math.cpp)

# Consumers need the headers, but not math's private src/.
target_include_directories(math
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(calc src/main.cpp)
target_link_libraries(calc PRIVATE math)
```

When `calc` links to `math`, it automatically inherits `math`'s PUBLIC/INTERFACE include paths,
compile definitions, and link dependencies.

---

## Source Organization and File Globs

### Explicit source listing (recommended)

List sources explicitly so new files are not silently missed:

```cmake
add_executable(app
  src/main.cpp
  src/foo.cpp
  include/foo.hpp
)
```

### `target_sources()`

This is useful when sources are added in subdirectories:

```cmake
add_library(mylib STATIC)
target_sources(mylib PRIVATE src/a.cpp src/b.cpp)
```

### `file(GLOB ...)` (use carefully)

Globbing can be convenient, but CMake may not automatically re-run configure when files change
unless you use `CONFIGURE_DEPENDS`.

```cmake
file(GLOB CONFIGURE_DEPENDS SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(mylib STATIC ${SRC_FILES})
```

Still, explicit lists are more predictable.

---

## Build Types, Multi-Config vs Single-Config

### Single-config generators (Ninja, Unix Makefiles)

You choose a build type at configure time:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
```

Common values: `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel`.

### Multi-config generators (Visual Studio, Xcode, Ninja Multi-Config)

Build type is selected at build time:

```sh
cmake -S . -B build -G "Ninja Multi-Config"
cmake --build build --config Debug
```

In scripts, avoid assuming `CMAKE_BUILD_TYPE` exists; it may be empty.

---

## Options and Cache Variables

### `option()`

```cmake
option(ENABLE_TESTS "Build tests" ON)
```

You can toggle it:

```sh
cmake -S . -B build -DENABLE_TESTS=OFF
```

### Cache variables

Cache entries persist across configure runs in a build directory.

```cmake
set(MY_PATH "" CACHE PATH "Path to something")
set(MY_MODE "fast" CACHE STRING "Mode: fast|safe")
set_property(CACHE MY_MODE PROPERTY STRINGS fast safe)
```

### Useful built-in cache variables

- `CMAKE_PREFIX_PATH`: extra prefixes for `find_package()`
- `CMAKE_TOOLCHAIN_FILE`: cross-compiling/toolchain selection
- `CMAKE_EXPORT_COMPILE_COMMANDS`: generates `compile_commands.json`

---

## Functions, Macros, and Scope

### Directory scope

CMake has a directory-based scope model. `add_subdirectory()` enters another directory and
creates a new scope layered on top of the parent.

### `function()` vs `macro()`

- `function()`: has its own scope; variables set inside do not leak unless returned
- `macro()`: textually expands; variables leak (often surprising)

Prefer `function()`.

Example helper:

```cmake
function(enable_warnings target)
  target_compile_options(${target} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
  )
endfunction()
```

Returning values:

```cmake
function(compute_out_var out)
  set(${out} "value" PARENT_SCOPE)
endfunction()
```

---

## Generator Expressions

Generator expressions are evaluated at *generate time* (not configure time) and depend on
configuration, platform, target, etc.

Syntax: `$<...>`

Common patterns:

### Conditional compile options

```cmake
target_compile_options(app PRIVATE
  $<$<CONFIG:Debug>:-O0 -g>
)
```

### Conditional sources

```cmake
target_sources(app PRIVATE
  $<$<PLATFORM_ID:Windows>:src/win.cpp>
  $<$<NOT:$<PLATFORM_ID:Windows>>:src/posix.cpp>
)
```

### Include dirs for build vs install

```cmake
target_include_directories(mylib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
```

---

## Output Layout and Runtime Paths

### Output directories

For multi-target projects, you often want binaries in consistent folders:

```cmake
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
```

Be aware: multi-config generators may need config-specific output dirs.

### RPATH (Linux/macOS)

RPATH controls where executables look for shared libraries.

When installing, consider:

- `CMAKE_INSTALL_RPATH`
- `CMAKE_BUILD_RPATH`

RPATH settings are platform-specific and easy to get wrong; test your install tree.

---

## Dependencies

There are several dependency workflows. Prefer target-based linking.

### 1) Using `find_package()` (system or preinstalled dependencies)

Example (config package):

```cmake
find_package(fmt CONFIG REQUIRED)
target_link_libraries(app PRIVATE fmt::fmt)
```

Example (module mode):

```cmake
find_package(OpenSSL REQUIRED)
target_link_libraries(app PRIVATE OpenSSL::SSL OpenSSL::Crypto)
```

Hints:

- If CMake cannot find a package, set `-DCMAKE_PREFIX_PATH=/path/to/prefix`
- Or set `<Package>_DIR` to the directory containing `<Package>Config.cmake`

### 2) `FetchContent` (download at configure time)

Good for reproducible builds without system dependencies.

```cmake
include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)

target_link_libraries(app PRIVATE fmt::fmt)
```

Notes:

- Pin versions/tags for reproducibility
- Consider `FETCHCONTENT_UPDATES_DISCONNECTED` for offline builds

### 3) Add a subproject (`add_subdirectory`)

If you vendor a dependency into your tree:

```cmake
add_subdirectory(external/somelib)
target_link_libraries(app PRIVATE somelib)
```

### 4) `ExternalProject` (build dependency as separate project)

Use when the dependency has a non-CMake build, or must be built in isolation.
This is more complex than `FetchContent`.

---

## Testing with CTest

### Enable testing

```cmake
include(CTest)  # defines BUILD_TESTING option

if(BUILD_TESTING)
  add_executable(unit_tests tests/test_main.cpp)
  add_test(NAME unit COMMAND unit_tests)
endif()
```

Run tests:

```sh
ctest --test-dir build --output-on-failure
```

If you use a test framework (Catch2, GoogleTest), prefer its CMake integration to auto-register tests.

---

## Install Rules

Installing means creating a usable tree under a prefix.

### Install targets

```cmake
install(TARGETS mylib app
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
```

### Install headers

```cmake
install(DIRECTORY include/ DESTINATION include)
```

### Export targets (for `find_package` by consumers)

High-level idea:

- Install your library
- Install an export set that defines imported targets for consumers
- Install a `<Package>Config.cmake` + version file

Skeleton:

```cmake
install(TARGETS mylib
  EXPORT myprojTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(EXPORT myprojTargets
  NAMESPACE myproj::
  DESTINATION lib/cmake/myproj
)
```

Producing a complete package config is a longer topic; start with `CMakePackageConfigHelpers` when you reach that stage.

---

## Packaging with CPack

CPack creates distributable packages from install rules (zip/tgz/deb/rpm/nsis/etc.).

Minimal:

```cmake
include(CPack)
```

Typical workflow:

```sh
cmake -S . -B build
cmake --build build
cmake --install build --prefix stage
cpack --config build/CPackConfig.cmake
```

For real packaging, set variables like `CPACK_PACKAGE_NAME`, `CPACK_PACKAGE_VERSION`, generator selection, etc.

---

## Presets (`CMakePresets.json`)

Presets make builds reproducible across machines.

Common preset types:

- configure presets: generator, cache vars, toolchain
- build presets: config, parallelism
- test presets: ctest args

Example structure:

```json
{
  "version": 6,
  "configurePresets": [
    {
      "name": "ninja-debug",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/ninja-debug",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    }
  ],
  "buildPresets": [
    { "name": "ninja-debug", "configurePreset": "ninja-debug" }
  ]
}
```

Use:

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
```

---

## Toolchains and Cross-Compiling

### Toolchain file

A toolchain file sets compilers and platform info before CMake detects anything.

Configure with:

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=toolchains/arm64.cmake
```

Toolchain file typical contents (conceptual):

- `CMAKE_SYSTEM_NAME` (e.g., `Linux`, `Windows`, `Darwin`)
- compiler paths (`CMAKE_C_COMPILER`, `CMAKE_CXX_COMPILER`)
- sysroot
- `CMAKE_FIND_ROOT_PATH` and related `*_MODE_*` variables

Cross-compiling is deep; expect to iterate while you learn your platform.

---

## Common C++ Workflows

### Export `compile_commands.json`

```sh
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Tools like clangd and many linters use it.

### Sanitizers (gcc/clang)

Attach flags to a target:

```cmake
target_compile_options(app PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_options(app PRIVATE -fsanitize=address)
```

Often you gate this behind an option.

### LTO / IPO

```cmake
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_ok OUTPUT ipo_err)
if(ipo_ok)
  set_property(TARGET app PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
```

### clang-tidy

For one target:

```cmake
set_target_properties(app PROPERTIES
  CXX_CLANG_TIDY "clang-tidy;-p;${CMAKE_BINARY_DIR}"
)
```

Or globally (be careful; affects dependencies):

```cmake
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
```

---

## Common Pitfalls and Debugging

### Pitfall: global flags

Avoid:

- `add_definitions()`
- `include_directories()`
- `link_directories()`
- setting `CMAKE_CXX_FLAGS` directly

These can leak into dependencies and make builds non-reproducible.

Use target-based commands instead.

### Pitfall: mixing keywords and plain signature

`target_link_libraries()` has two signatures; do not mix them.

Good:

```cmake
target_link_libraries(app PRIVATE mylib)
```

Bad:

```cmake
target_link_libraries(app mylib)
target_link_libraries(app PRIVATE otherlib)  # mixes signatures
```

### Pitfall: hardcoding compiler paths

Prefer toolchain files or presets.

### Pitfall: assuming `CMAKE_BUILD_TYPE`

In multi-config generators it may be empty; use generator expressions or `CMAKE_CONFIGURATION_TYPES`.

### Debugging tips

- Print variables:

  ```cmake
  message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
  ```

- Trace CMake execution:

  ```sh
  cmake -S . -B build --trace-source=CMakeLists.txt
  ```

- View cache:

  - use `cmake-gui`, `ccmake`, or inspect `build/CMakeCache.txt`

- Inspect target properties:

  ```cmake
  get_target_property(incs app INCLUDE_DIRECTORIES)
  message(STATUS "app incs: ${incs}")
  ```

---

## Starter Templates

These are minimal patterns you can copy and adapt.

### Minimal executable

```cmake
cmake_minimum_required(VERSION 3.24)
project(Hello VERSION 0.1 LANGUAGES CXX)

add_executable(hello src/main.cpp)
target_compile_features(hello PRIVATE cxx_std_20)
```

### Library + executable

```cmake
cmake_minimum_required(VERSION 3.24)
project(MyProj VERSION 0.1 LANGUAGES CXX)

add_library(mylib STATIC src/lib.cpp)
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(mylib PUBLIC cxx_std_20)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE mylib)
```

### Options + warnings helper

```cmake
option(ENABLE_WARNINGS "Enable common warnings" ON)

function(enable_warnings target)
  target_compile_options(${target} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
  )
endfunction()

add_executable(app src/main.cpp)
target_compile_features(app PRIVATE cxx_std_20)

if(ENABLE_WARNINGS)
  enable_warnings(app)
endif()
```

---

## Glossary (Quick)

- **Configure**: CMake step that generates the native build files.
- **Generate**: part of configure; evaluates generator expressions.
- **Build**: invoking Ninja/Make/IDE build using generated files.
- **Target**: logical build output (executable, library, custom target).
- **Imported target**: a target defined by a dependency, e.g. `fmt::fmt`.
- **Usage requirements**: properties that propagate to consumers (`PUBLIC`/`INTERFACE`).
- **Cache variable**: persistent variable stored in `CMakeCache.txt`.
- **Generator**: build system backend (Ninja, Makefiles, Visual Studio, ...).
- **Toolchain file**: early config file defining compilers/platform.

---

## Suggested Next Steps

1. Skim the templates and pick one style to practice.
2. Build a tiny project twice: once with a single target, once with a library + executable.
3. Add a dependency via `FetchContent` (e.g. `fmt`) and observe how imported targets work.
