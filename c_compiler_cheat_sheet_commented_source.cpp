
/*
=====================================================================
 C++ COMPILER CHEAT SHEET
---------------------------------------------------------------------
 This file is NOT meant to be compiled.
 It is a living reference written as a C++ source file with comments.

 Open this in Neovim / VS Code and treat it like documentation.
=====================================================================
*/

/*
=====================================================================
 1. MAJOR C++ COMPILERS
=====================================================================
*/

/*
--------------------------------------------------
 GCC (GNU Compiler Collection)
--------------------------------------------------
 Command:
     g++

 Platforms:
     - Linux (default)
     - macOS (via Homebrew)
     - Windows (MinGW / WSL)

 Example:
     g++ main.cpp -o main

 Notes:
     - Most common on Linux
     - Excellent warnings
     - Works perfectly with GDB and Valgrind
*/

/*
--------------------------------------------------
 Clang / LLVM
--------------------------------------------------
 Command:
     clang++

 Platforms:
     - Linux
     - macOS (default compiler)
     - Windows (LLVM toolchain)

 Example:
     clang++ main.cpp -o main

 Notes:
     - Best error messages
     - Faster compile times in large projects
     - Excellent tooling (clangd, clang-tidy)
*/

/*
--------------------------------------------------
 MSVC (Microsoft Visual C++)
--------------------------------------------------
 Command:
     cl

 Platform:
     - Windows only

 Example:
     cl /std:c++20 main.cpp

 Notes:
     - Different flags (NOT GCC-style)
     - Required for some Windows libraries
*/

/*
--------------------------------------------------
 Intel oneAPI (icpx / icc)
--------------------------------------------------
 Commands:
     icpx   // new
     icc    // legacy

 Notes:
     - Strong CPU optimizations
     - Used in HPC and numerical workloads
*/

/*
=====================================================================
 2. SELECTING C++ STANDARD
=====================================================================
*/

/*
 GCC / Clang:
     -std=c++11
     -std=c++14
     -std=c++17
     -std=c++20
     -std=c++23
     -std=gnu++20   // GNU extensions

 Example:
     g++ -std=c++20 main.cpp
*/

/*
 MSVC:
     /std:c++14
     /std:c++17
     /std:c++20
     /std:c++latest
*/

/*
=====================================================================
 3. WARNING FLAGS (IMPORTANT)
=====================================================================
*/

/*
 Basic warnings (ALWAYS use):
     -Wall -Wextra

 Strong warnings:
     -Wpedantic
     -Wshadow
     -Wconversion
     -Wsign-conversion
     -Weffc++        // Effective C++ rules (educational, noisy)

 Treat warnings as errors:
     -Werror

 Example:
     g++ main.cpp -Wall -Wextra -Wpedantic -Wshadow
*/

/*
 MSVC warnings:
     /W4        // high warning level
     /WX        // warnings as errors
*/

/*
=====================================================================
 4. OPTIMIZATION LEVELS
=====================================================================
*/

/*
     -O0    // no optimization (debug)
     -O1    // basic optimization
     -O2    // recommended default
     -O3    // aggressive optimization
     -Os    // optimize for size
     -Ofast // breaks strict standard guarantees

 Debug build:
     -O0 -g

 Release build:
     -O2 -DNDEBUG
*/

/*
=====================================================================
 5. DEBUGGING & SYMBOLS
=====================================================================
*/

/*
 Generate debug info:
     -g
     -g3   // maximum debug info
     -ggdb // GDB-specific extensions

 Usage:
     gdb ./program
*/

/*
=====================================================================
 6. SANITIZERS (HIGHLY RECOMMENDED)
=====================================================================
*/

/*
 Address Sanitizer (memory errors):
     -fsanitize=address

 Undefined Behavior Sanitizer:
     -fsanitize=undefined

 Leak Sanitizer:
     -fsanitize=leak

 Thread Sanitizer:
     -fsanitize=thread

 Combined example:
     g++ main.cpp -fsanitize=address,undefined -g
*/

/*
 MSVC (limited):
     /fsanitize=address
*/

/*
=====================================================================
 7. LINKING LIBRARIES (IMPORTANT)
=====================================================================
*/

/*
 System libraries:
     -lm            // math
     -pthread      // threading (preferred)
     -ldl          // dynamic loader
     -lrt          // realtime extensions

 C++ standard library control:
     -static-libstdc++
     -static-libgcc

 Custom libraries:
     -L/path/to/lib     // library search path
     -lmylib            // links libmylib.so / libmylib.a

 Order matters:
     g++ main.o -lmylib -lm

 Static linking (careful):
     -static
*/

/*
=====================================================================
 8. INCLUDE PATHS
=====================================================================
*/

/*
 User include paths:
     -I./include
     -I/usr/local/include

 System include paths (suppresses warnings):
     -isystem /usr/include/somelib
*/

/*
=====================================================================
 9. OUTPUT & COMPILATION MODES
=====================================================================
*/

/*
 Output file name:
     -o program

 Compile only (no linking):
     -c

 Generate assembly:
     -S

 Preprocess only:
     -E
*/

/*
=====================================================================
 10. PREPROCESSOR DEFINES
=====================================================================
*/

/*
 Define macros:
     -DDEBUG
     -DVERSION=2

 Undefine macros:
     -UDEBUG

 Usage in code:
     #ifdef DEBUG
     #endif
*/

/*
=====================================================================
 11. COMMON BUILD PRESETS
=====================================================================
*/

/*
 DEBUG PRESET:
     -std=c++20 -O0 -g -Wall -Wextra -Wpedantic \
     -fsanitize=address,undefined

 RELEASE PRESET:
     -std=c++20 -O2 -DNDEBUG
*/

/*
=====================================================================
 12. ABI / FEATURE CONTROL
=====================================================================
*/

/*
 Disable exceptions:
     -fno-exceptions

 Disable RTTI:
     -fno-rtti

 GCC libstdc++ ABI toggle:
     -D_GLIBCXX_USE_CXX11_ABI=0 | 1
*/

/*
=====================================================================
 13. CROSS COMPILATION
=====================================================================
*/

/*
 Windows target from Linux:
     --target=x86_64-w64-mingw32

 ARM Linux:
     --target=aarch64-linux-gnu
*/

/*
=====================================================================
 14. TOOLING INTEGRATION
=====================================================================
*/

/*
 CMake compilation database:
     -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

 Used by:
     - clangd
     - clang-tidy
     - Neovim / VS Code LSP
*/

/*
=====================================================================
 15. PRACTICAL RECOMMENDATIONS
=====================================================================
*/

/*
 Linux development:
     clang++ -std=c++20 -Wall -Wextra -O0 -g

 Competitive programming:
     g++ -std=gnu++20 -O2

 Learning & debugging:
     g++ -std=c++20 -fsanitize=address,undefined -g

 Windows (MSVC):
     cl /std:c++20 /W4 main.cpp
*/

/*
=====================================================================
 16. YOUR PERSONAL PRESET (CLEANED)
=====================================================================
*/

/*
 Warnings:
     -Wall -Wextra -Werror
     -Weffc++
     -Wconversion -Wsign-conversion

 Strict standard compliance:
     -pedantic-errors

 Debug build (recommended now):
     -O0 -ggdb

g++ -std=c++20 -O0 -g -Wall -Wextra -fsanitize=address,undefined
 Release build:
     -O2 -DNDEBUG
*/

/*
=====================================================================
 END OF CHEAT SHEET
=====================================================================
*/
