
/*
===============================================================================
                               VALGRIND GUIDE
===============================================================================

This file is NOT meant to be executed.
It is a documentation-style reference for learning and using Valgrind with C++.

You can:
- Keep this file as notes
- Open it inside your editor
- Search inside it when debugging memory issues

===============================================================================
1. WHAT IS VALGRIND?
===============================================================================

Valgrind is a dynamic binary instrumentation framework.
In simple terms:

→ It runs your program inside a virtual machine
→ Tracks every memory access at runtime
→ Reports memory-related bugs

Valgrind can detect:
- Memory leaks
- Invalid reads / writes
- Use of uninitialized memory
- Double delete / invalid free
- Heap misuse

Valgrind works BEST for:
- C and C++
- Debug builds
- Single-threaded or lightly threaded programs

===============================================================================
2. INSTALLATION
===============================================================================

On Linux (Arch / Omarchy):
    sudo pacman -S valgrind

On Ubuntu / Debian:
    sudo apt install valgrind

Check installation:
    valgrind --version

===============================================================================
3. COMPILING YOUR PROGRAM (CRITICAL STEP)
===============================================================================

ALWAYS compile with debug symbols and NO optimization.

Correct:
    g++ -std=c++20 -g -O0 main.cpp -o app

Wrong:
    g++ -O2 main.cpp          // Optimizations hide bugs
    g++ main.cpp              // No debug info

Explanation:
- -g   → line numbers + variable info
- -O0  → disables optimizations so Valgrind can track memory correctly

===============================================================================
4. BASIC USAGE
===============================================================================

Run your program under Valgrind:

    valgrind ./app

This already checks:
- Invalid memory access
- Uninitialized reads
- Some leaks

===============================================================================
5. MOST IMPORTANT COMMAND (USE THIS 90% OF THE TIME)
===============================================================================

    valgrind \
      --leak-check=full \
      --show-leak-kinds=all \
      --track-origins=yes \
      ./app

Explanation:
- --leak-check=full
    Shows detailed info about each leak

- --show-leak-kinds=all
    Shows:
      * definitely lost
      * indirectly lost
      * possibly lost
      * still reachable

- --track-origins=yes
    Tracks where uninitialized values come from
    (Slower, but VERY useful)

===============================================================================
6. HOW TO READ VALGRIND ERRORS
===============================================================================

Example error:

    Invalid read of size 4
       at 0x40123A: main (main.cpp:15)

Meaning:
- Your program read memory it does not own
- Size 4 → usually an int
- main.cpp:15 → exact source location

Common causes:
- Out-of-bounds array access
- Use-after-free
- Dereferencing null or dangling pointer

===============================================================================
7. MEMORY LEAK REPORT TYPES
===============================================================================

Example:

    40 bytes in 1 blocks are definitely lost

Leak categories:

1) definitely lost ❌
   - Real memory leak
   - You lost the pointer
   - MUST be fixed

2) indirectly lost ❌
   - Memory pointed to by leaked memory
   - Fix root leak to fix this

3) possibly lost ⚠️
   - Pointer manipulation confused Valgrind
   - Often related to custom allocators or pointer arithmetic

4) still reachable ✅
   - Memory still accessible at program exit
   - Usually globals or static objects
   - Generally OK

===============================================================================
8. COMMON BUG EXAMPLES VALGRIND CATCHES
===============================================================================

--- Uninitialized read ---
    int* p = new int;
    std::cout << *p;   // ERROR
    delete p;

--- Out-of-bounds access ---
    int arr[5];
    arr[10] = 3;       // ERROR

--- Wrong delete ---
    int* arr = new int[10];
    delete arr;        // ERROR (should be delete[])

--- Double delete ---
    int* p = new int(5);
    delete p;
    delete p;          // ERROR

--- Use-after-free ---
    int* p = new int(7);
    delete p;
    std::cout << *p;   // ERROR

===============================================================================
9. USING VALGRIND WITH INPUT / ARGUMENTS
===============================================================================

With command-line arguments:
    valgrind ./app arg1 arg2

With redirected input:
    valgrind ./app < input.txt

===============================================================================
10. STACK VS HEAP (IMPORTANT FOR INTERPRETING ERRORS)
===============================================================================

Stack memory:
- Local variables
- Automatically freed
- Valgrind mainly reports UNINITIALIZED reads here

Heap memory:
- new / delete
- malloc / free
- Most Valgrind errors happen here

===============================================================================
11. VALGRIND AND MODERN C++
===============================================================================

Modern C++ advice:
- Prefer std::vector over new[]
- Prefer std::string over char*
- Prefer std::unique_ptr / std::shared_ptr

Valgrind is still useful for:
- Learning how memory works
- Debugging low-level data structures
- Understanding ownership mistakes

Even with RAII, Valgrind can catch:
- Logic bugs
- Misuse of raw pointers
- Library misuse

===============================================================================
12. PERFORMANCE CONSIDERATIONS
===============================================================================

Valgrind is SLOW:
- 10x to 50x slower execution
- Uses heavy instrumentation

Do NOT use for:
- Performance benchmarking
- Large production workloads

Use it only for debugging.

===============================================================================
13. VALGRIND VS SANITIZERS (VERY IMPORTANT)
===============================================================================

Modern alternative (recommended daily):

    g++ -std=c++20 -fsanitize=address,undefined -g main.cpp
    ./a.out

Comparison:

Valgrind:
✔ Deep leak detection
✔ No recompilation required
✘ Very slow

Sanitizers:
✔ Fast
✔ Better diagnostics
✔ Compiler-integrated
✘ Requires recompilation

Best practice:
- Use sanitizers daily
- Use Valgrind for final deep checks

===============================================================================
14. COMMON VALGRIND OPTIONS (REFERENCE)
===============================================================================

--leak-check=full
--track-origins=yes
--show-leak-kinds=all
--log-file=valgrind.log
--error-exitcode=1

Example with log:
    valgrind --log-file=vg.log ./app

===============================================================================
15. WHAT VALGRIND CANNOT DO WELL
===============================================================================

- Data races → use ThreadSanitizer
- GPU memory
- Highly optimized / inlined code
- Windows native (use WSL)

===============================================================================
16. RECOMMENDED WORKFLOW (FINAL)
===============================================================================

1) Compile:
    g++ -std=c++20 -g -O0 main.cpp -o app

2) Run with sanitizers:
    ./app

3) Run with Valgrind:
    valgrind --leak-check=full --track-origins=yes ./app

4) Fix errors
5) Repeat

===============================================================================
END OF VALGRIND DOCUMENTATION
===============================================================================
*/

int main()
{
    // This file exists purely as documentation.
    // There is no executable logic here.
    return 0;
}
