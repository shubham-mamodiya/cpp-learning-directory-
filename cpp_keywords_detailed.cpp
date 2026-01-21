/*
===============================================================================
 Complete List of C++ Keywords (C++23) - DETAILED EXPLANATIONS
 Categorized by usage with examples and clear descriptions
===============================================================================
*/

/*==============================================================================
  1. FUNDAMENTAL TYPES - Building blocks for storing data
==============================================================================*/

// BOOLEAN TYPE
bool            // Stores true or false values
                // Example: bool isReady = true;
                // Size: Usually 1 byte

// CHARACTER TYPES
char            // Stores a single character (ASCII)
                // Example: char letter = 'A';
                // Size: 1 byte, range: -128 to 127 (or 0 to 255 if unsigned)

char8_t         // Stores a single UTF-8 character (C++20)
                // Example: char8_t emoji = u8'😀'; (careful: many emojis need multiple units)
                // Size: 1 byte
                // Used for Unicode text that's backwards-compatible with ASCII

char16_t        // Stores a single UTF-16 code unit
                // Example: char16_t ch = u'€';
                // Size: 2 bytes
                // Used for Unicode (may need multiple units for some characters)

char32_t        // Stores a single UTF-32 character (one full Unicode codepoint)
                // Example: char32_t ch = U'😀';
                // Size: 4 bytes
                // Guarantees one value = one character (all Unicode fits)

wchar_t         // Wide character (platform-dependent size)
                // Example: wchar_t wc = L'Ω';
                // Size: 2 or 4 bytes depending on platform
                // Windows: 2 bytes, Linux: 4 bytes

// INTEGER TYPES
short           // Short integer (smaller range than int)
                // Example: short temperature = -40;
                // Size: Usually 2 bytes, range: -32,768 to 32,767

int             // Standard integer type
                // Example: int count = 1000;
                // Size: Usually 4 bytes, range: -2,147,483,648 to 2,147,483,647

long            // Long integer (at least as large as int)
                // Example: long population = 8000000000L;
                // Size: 4 or 8 bytes depending on platform

signed          // Specifies that a type can hold negative values (default for int types)
                // Example: signed int temperature = -10;
                // Can be combined with: char, short, int, long

unsigned        // Specifies that a type can only hold non-negative values (0 and positive)
                // Example: unsigned int age = 25;
                // Doubles the positive range (e.g., 0 to 4,294,967,295 for unsigned int)

// FLOATING POINT TYPES
float           // Single-precision floating point number
                // Example: float price = 19.99f;
                // Size: 4 bytes, ~7 decimal digits of precision
                // Range: ±3.4e38

double          // Double-precision floating point number (more precise than float)
                // Example: double pi = 3.14159265359;
                // Size: 8 bytes, ~15-16 decimal digits of precision
                // Range: ±1.7e308

// SPECIAL TYPES
void            // Represents "no type" or "no value"
                // Used for functions that don't return anything
                // Example: void printMessage() { std::cout << "Hello"; }
                // Also used for generic pointers: void* ptr

nullptr_t       // The type of nullptr (rarely used directly)
                // Example: nullptr_t null_value = nullptr;
                // Usually you just use 'nullptr' without specifying the type


/*==============================================================================
  2. TYPE MODIFIERS / QUALIFIERS - Control how types behave
==============================================================================*/

const           // Makes a variable read-only (cannot be modified after initialization)
                // Example: const int MAX_SIZE = 100;
                // Attempting to change: MAX_SIZE = 200; // ERROR!
                // Also used for function parameters to prevent modification

constexpr       // Indicates a value can be computed at compile-time
                // Example: constexpr int square(int x) { return x * x; }
                //          constexpr int result = square(5); // Calculated during compilation
                // More powerful than const - guarantees compile-time evaluation when possible

consteval       // MUST be evaluated at compile-time (C++20)
                // Example: consteval int factorial(int n) { return n <= 1 ? 1 : n * factorial(n-1); }
                //          int x = factorial(5); // Computed at compile-time: 120
                // Stricter than constexpr - always compile-time, never runtime

constinit       // Ensures static/thread_local variables are initialized at compile-time (C++20)
                // Example: constinit int config = 42;
                // Prevents "static initialization order fiasco"
                // Variable itself is NOT const, but initialization must be compile-time

volatile        // Tells compiler this variable can change unexpectedly (don't optimize it)
                // Example: volatile int hardwareRegister;
                // Used for: hardware registers, signal handlers, memory-mapped I/O
                // Compiler won't cache its value in a register

mutable         // Allows modification of a member variable in a const object
                // Example: class Counter {
                //              mutable int accessCount = 0;
                //          public:
                //              void getValue() const { ++accessCount; } // OK!
                //          };
                // Used for caching, debugging counters, mutex locks in const methods


/*==============================================================================
  3. STORAGE DURATION & LINKAGE - Control where and how long variables live
==============================================================================*/

auto            // MODERN: Automatic type deduction (compiler figures out the type)
                // Example: auto x = 5;        // x is int
                //          auto y = 3.14;     // y is double
                //          auto msg = "Hi";   // msg is const char*
                // OLD (deprecated): Automatic storage duration

register        // DEPRECATED: Hint to store variable in CPU register for speed
                // Example: register int counter; // Compiler ignores this now
                // Modern compilers optimize better without hints
                // As of C++17, this has no effect

static          // THREE MEANINGS:
                // 1. In functions: Variable persists between function calls
                //    Example: void count() { static int num = 0; ++num; }
                // 2. In classes: Member shared by all instances (not per-object)
                //    Example: class Counter { static int total; };
                // 3. At file scope: Limits visibility to current file (internal linkage)

extern          // Declares a variable/function defined in another file
                // Example: extern int globalConfig; // Defined in another .cpp file
                // Tells compiler: "This exists somewhere else, trust me"
                // Used to share global variables across multiple files

thread_local    // Each thread gets its own copy of this variable (C++11)
                // Example: thread_local int threadID = 0;
                // Useful for multi-threaded programs where threads need independent data
                // Each thread's copy is initialized separately


/*==============================================================================
  4. CONTROL FLOW STATEMENTS - Direct the execution path of your program
==============================================================================*/

if              // Executes code block if condition is true
                // Example: if (age >= 18) { /* can vote */ }

else            // Executes when 'if' condition is false
                // Example: if (score >= 60) { pass(); } else { fail(); }

switch          // Multi-way branch based on value
                // Example: switch(grade) {
                //              case 'A': /* excellent */; break;
                //              case 'B': /* good */; break;
                //              default: /* other */;
                //          }

case            // Labels a branch in a switch statement
                // Must be followed by a constant expression
                // Falls through to next case unless you use 'break'

default         // Catch-all case in switch (when no other case matches)
                // Optional but recommended

for             // Loop with initialization, condition, and increment
                // Example: for (int i = 0; i < 10; ++i) { /* runs 10 times */ }
                // Also range-based: for (auto& item : collection) { }

while           // Loop that continues while condition is true (checks condition first)
                // Example: while (count < 100) { ++count; }
                // May not execute at all if condition is initially false

do              // Loop that executes at least once (checks condition at end)
                // Example: do { ++count; } while (count < 100);
                // Always runs the body at least once

break           // Exits the innermost loop or switch statement immediately
                // Example: for (int i = 0; i < 100; ++i) {
                //              if (i == 50) break; // Exits loop at 50
                //          }

continue        // Skips the rest of current loop iteration, goes to next iteration
                // Example: for (int i = 0; i < 10; ++i) {
                //              if (i % 2 == 0) continue; // Skip even numbers
                //              std::cout << i; // Only prints odd numbers
                //          }

goto            // Jumps to a labeled statement (AVOID - makes code hard to follow)
                // Example: goto error_handler;
                //          // ... code ...
                //          error_handler:
                //              // cleanup code
                // Only acceptable use: breaking out of nested loops

return          // Exits function and optionally returns a value
                // Example: int add(int a, int b) { return a + b; }
                // In void functions: return; (no value)


/*==============================================================================
  5. EXCEPTION HANDLING - Gracefully handle errors
==============================================================================*/

try             // Marks a block of code that might throw exceptions
                // Example: try {
                //              riskyOperation();
                //          } catch (const std::exception& e) {
                //              // Handle error
                //          }

catch           // Handles exceptions thrown in try block
                // Example: catch (const std::runtime_error& e) { /* handle */ }
                //          catch (...) { /* catch everything */ }
                // Can have multiple catch blocks for different exception types

throw           // Throws an exception (signals an error)
                // Example: if (x < 0) throw std::invalid_argument("x must be positive");
                // Also re-throws current exception: throw; (inside catch block)

noexcept        // Specifies that a function won't throw exceptions
                // Example: void safeFunction() noexcept { /* guaranteed no throws */ }
                // If it does throw, program terminates
                // Allows optimizations; also can query: noexcept(expression)


/*==============================================================================
  6. FUNCTIONS & CLASSES - Building blocks for organizing code
==============================================================================*/

// ACCESS SPECIFIERS - Control who can access class members

public          // Makes members accessible from anywhere
                // Example: class MyClass {
                //          public:
                //              int value;        // Anyone can access
                //              void doSomething(); // Anyone can call
                //          };
                // Used for: interfaces, public APIs
                // Also used for inheritance: class Derived : public Base { };

protected       // Makes members accessible from the class itself and derived classes
                // Example: class Base {
                //          protected:
                //              int secret;       // Derived classes can access
                //              void helper();    // Derived classes can call
                //          };
                //          class Derived : public Base {
                //              void useSecret() { secret = 10; } // OK!
                //          };
                // Not accessible from outside the class hierarchy
                // Used for: helper functions/data for subclasses

private         // Makes members accessible only within the class (default for classes)
                // Example: class BankAccount {
                //          private:              // Can omit 'private:' - it's default
                //              double balance;   // Only BankAccount methods can access
                //              void validate();  // Internal helper
                //          public:
                //              void deposit(double amount); // Public interface
                //          };
                // Strongest encapsulation
                // Used for: implementation details, internal state

// USER-DEFINED TYPES

class           // Defines a class (members are private by default)
                // Example: class Person {
                //              std::string name; // private by default
                //          public:
                //              void setName(const std::string& n) { name = n; }
                //          };

struct          // Defines a structure (members are public by default)
                // Example: struct Point { int x, y; }; // x and y are public
                // Otherwise identical to class

union           // Type where all members share the same memory location
                // Example: union Data {
                //              int i;
                //              float f;
                //              char c;
                //          }; // Only one member has a meaningful value at a time
                // Size = size of largest member

enum            // Defines an enumeration (set of named integer constants)
                // Example: enum Color { RED, GREEN, BLUE }; // RED=0, GREEN=1, BLUE=2
                // Values can be used as integers (weak typing)

// Note: "enum class" is two keywords used together
                // Scoped enumeration (strongly-typed, values don't leak into surrounding scope)
                // Example: enum class Status { OK, ERROR };
                //          Status s = Status::OK; // Must use Status:: prefix
                // Can't implicitly convert to int (safer)

inline          // MODERN: Suggests function can be defined in header (multiple definitions OK)
                // Example: inline int max(int a, int b) { return a > b ? a : b; }
                // Compiler may or may not actually inline it
                // Mainly prevents "multiple definition" errors

virtual         // Marks a function as overridable in derived classes (enables polymorphism)
                // Example: class Base {
                //          public:
                //              virtual void speak() { /* base implementation */ }
                //          };
                //          class Derived : public Base {
                //          public:
                //              void speak() override { /* override */ }
                //          };
                // Enables dynamic dispatch (runtime polymorphism)

explicit        // Prevents implicit type conversions for constructors/conversion operators
                // Example: class MyInt {
                //          public:
                //              explicit MyInt(int x) { }
                //          };
                //          MyInt a = 5;    // ERROR! Can't implicitly convert
                //          MyInt b(5);     // OK - explicit construction
                //          MyInt c = MyInt(5); // OK

friend          // Grants a function or class access to private/protected members
                // Example: class Box {
                //              int width;
                //              friend void printWidth(const Box& b); // Can access width
                //          };
                // Breaks encapsulation - use sparingly

this            // Pointer to the current object (only available in member functions)
                // Example: class Person {
                //              std::string name;
                //          public:
                //              void setName(const std::string& name) {
                //                  this->name = name; // Disambiguate
                //              }
                //          };

operator        // Defines or overloads an operator for a class
                // Example: class Complex {
                //          public:
                //              Complex operator+(const Complex& other) const {
                //                  // Define addition
                //              }
                //          };
                // Can overload: +, -, *, /, ==, <<, [], (), etc.

sizeof          // Returns size of a type or object in bytes (compile-time)
                // Example: sizeof(int)        // Usually 4
                //          sizeof(double)     // Usually 8
                //          int arr[10];
                //          sizeof(arr)        // 40 (10 * 4)

alignof         // Returns alignment requirement of a type (C++11)
                // Example: alignof(double)    // Usually 8
                //          alignof(char)      // Usually 1
                // Alignment = memory address must be multiple of this value

alignas         // Specifies alignment for a variable or type (C++11)
                // Example: alignas(16) int x;  // x must be aligned to 16-byte boundary
                //          alignas(32) struct Data { /* ... */ };
                // Used for SIMD, cache line optimization

decltype        // Deduces the type of an expression at compile-time
                // Example: int x = 5;
                //          decltype(x) y = 10; // y is also int
                //          decltype(x + 3.14) z; // z is double
                // Preserves references and const

typeid          // Returns type information at runtime (RTTI)
                // Example: #include <typeinfo>
                //          int x = 5;
                //          std::cout << typeid(x).name(); // Prints type name
                //          if (typeid(x) == typeid(int)) { /* true */ }
                // Requires RTTI to be enabled


/*==============================================================================
  7. OBJECT LIFETIME - Managing dynamic memory
==============================================================================*/

new             // Allocates memory on the heap
                // Example: int* ptr = new int(5);        // Single int
                //          int* arr = new int[10];       // Array of 10 ints
                // Must be paired with delete to avoid memory leaks
                // Returns a pointer to the allocated memory

delete          // Deallocates memory allocated with new
                // Example: delete ptr;      // For single object
                //          delete[] arr;    // For arrays (note the [])
                // Using delete on non-new memory = undefined behavior
                // After delete, pointer is "dangling" - don't use it!

using           // MODERN: Type alias (cleaner than typedef)
                // Example: using Integer = int;
                //          using StringMap = std::map<std::string, std::string>;
                // Also: namespace alias: using std::cout;
                // Template aliases: using = template<typename T> using Vec = std::vector<T>;

typedef         // OLD: Creates a type alias
                // Example: typedef int Integer;
                //          typedef std::vector<int> IntVector;
                // 'using' is preferred in modern C++ (more readable with templates)

static_assert   // Compile-time assertion (checks condition during compilation)
                // Example: static_assert(sizeof(int) == 4, "int must be 4 bytes");
                // If false, compilation fails with error message
                // No runtime cost - checked at compile-time


/*==============================================================================
  8. TEMPLATES & GENERICS - Write code that works with any type
==============================================================================*/

template        // Defines a template (generic code)
                // Example: template<typename T>
                //          T max(T a, T b) { return a > b ? a : b; }
                //          // Works with: max(5, 10), max(3.14, 2.71), etc.
                // Also class templates: template<typename T> class Container { };

typename        // TWO USES:
                // 1. Template parameter: template<typename T> ...
                // 2. Disambiguate dependent types: typename Container<T>::value_type
                // Interchangeable with 'class' for template parameters

concept         // Defines constraints on template parameters (C++20)
                // Example: template<typename T>
                //          concept Numeric = std::is_arithmetic_v<T>;
                //          
                //          template<Numeric T>
                //          T add(T a, T b) { return a + b; }
                // Makes template errors clearer, documents requirements

requires        // Specifies constraints on templates (C++20)
                // Example: template<typename T>
                //          requires std::is_integral_v<T>
                //          T multiply(T a, T b) { return a * b; }
                // Also used in concepts and for function overloading

export          // NEW MEANING (C++20): Marks module interface
                // Example: export module MyModule;
                //          export int publicFunction();
                // OLD MEANING (removed): Export template definitions (never widely implemented)


/*==============================================================================
  9. NAMESPACES - Organize code and prevent name conflicts
==============================================================================*/

namespace       // Creates a namespace (named scope for identifiers)
                // Example: namespace MyLib {
                //              int value = 42;
                //              void doSomething() { }
                //          }
                //          // Use: MyLib::value, MyLib::doSomething()
                // Prevents name collisions in large projects
                // Can be nested: namespace Outer::Inner { }


/*==============================================================================
 10. CASTING OPERATORS - Type conversions (use cautiously!)
==============================================================================*/

static_cast     // Safe, compile-time checked conversions
                // Example: double d = 3.14;
                //          int i = static_cast<int>(d); // i = 3
                //          // Upcast: Derived* d = ...; Base* b = static_cast<Base*>(d);
                // Use for: numeric conversions, upcasting, explicit conversions
                // Won't compile if conversion is impossible

dynamic_cast    // Runtime-checked cast for polymorphic types (has virtual functions)
                // Example: Base* b = new Derived();
                //          Derived* d = dynamic_cast<Derived*>(b);
                //          if (d != nullptr) { /* successful */ }
                // Returns nullptr (pointers) or throws exception (references) if cast fails
                // Requires RTTI; has runtime cost

const_cast      // Adds or removes const/volatile qualifiers
                // Example: const int x = 10;
                //          int* ptr = const_cast<int*>(&x); // Remove const
                // DANGEROUS: Modifying originally const object = undefined behavior
                // Only use when you know object isn't really const (e.g., bad API design)

reinterpret_cast// Unsafe, low-level bit reinterpretation
                // Example: int x = 65;
                //          char* c = reinterpret_cast<char*>(&x);
                // Treats memory as a different type without conversion
                // Use for: pointer <-> integer, unrelated pointer types
                // VERY DANGEROUS - easy to invoke undefined behavior


/*==============================================================================
 11. BOOLEAN & NULL LITERALS - Special values
==============================================================================*/

true            // Boolean literal for "true" state
                // Example: bool isReady = true;
                // Converts to 1 in integer contexts

false           // Boolean literal for "false" state
                // Example: bool hasError = false;
                // Converts to 0 in integer contexts

nullptr         // Null pointer literal (C++11)
                // Example: int* ptr = nullptr;
                //          if (ptr == nullptr) { /* pointer is null */ }
                // Replaces NULL (which was just 0)
                // Type-safe: won't accidentally convert to integer


/*==============================================================================
 12. ALTERNATIVE OPERATOR KEYWORDS - Text versions of operators
==============================================================================*/
// These exist for keyboards/languages that lack special symbols
// Rarely used in modern code

and             // Alternative for &&  (logical AND)
                // Example: if (x > 0 and x < 10) { }
                // Same as: if (x > 0 && x < 10) { }

or              // Alternative for ||  (logical OR)
                // Example: if (x < 0 or x > 100) { }

not             // Alternative for !   (logical NOT)
                // Example: if (not isValid) { }

xor             // Alternative for ^   (bitwise XOR)
                // Example: int result = a xor b;

bitand          // Alternative for &   (bitwise AND)
                // Example: int masked = value bitand 0xFF;

bitor           // Alternative for |   (bitwise OR)
                // Example: int flags = FLAG_A bitor FLAG_B;

compl           // Alternative for ~   (bitwise complement)
                // Example: int inverted = compl value;

and_eq          // Alternative for &=  (AND assignment)
                // Example: value and_eq mask;

or_eq           // Alternative for |=  (OR assignment)
                // Example: flags or_eq NEW_FLAG;

xor_eq          // Alternative for ^=  (XOR assignment)
                // Example: value xor_eq mask;

not_eq          // Alternative for !=  (not equal)
                // Example: if (a not_eq b) { }


/*==============================================================================
 13. COROUTINES (C++20) - Pausable/resumable functions
==============================================================================*/

co_await        // Suspends coroutine until operation completes
                // Example: auto result = co_await asyncOperation();
                // Used for asynchronous programming (like async/await in other languages)
                // Coroutine pauses, other code can run, resumes when ready

co_yield        // Yields a value and suspends (used for generators)
                // Example: Generator<int> count() {
                //              for (int i = 0; i < 10; ++i)
                //                  co_yield i;
                //          }
                // Like Python's yield - produces value, remembers state

co_return       // Returns from a coroutine
                // Example: co_return 42;
                // Like normal return, but for coroutines
                // Can't use regular 'return' in a coroutine


/*==============================================================================
 14. MODULES (C++20) - Modern alternative to header files
==============================================================================*/

module          // Declares or implements a module
                // Example: export module Math; // Module interface
                //          module Math;         // Module implementation
                // Replaces #include system (faster compilation, better encapsulation)
                // No more header guards needed!

import          // Imports a module
                // Example: import std;      // Import standard library
                //          import Math;     // Import custom module
                // Replaces #include directives
                // Only imports what's explicitly exported


/*==============================================================================
 15. ATTRIBUTES & DIAGNOSTICS - Low-level system interaction
==============================================================================*/

asm             // Inline assembly code (platform-specific)
                // Example: asm("mov eax, 1"); // x86 assembly
                // Lets you write CPU instructions directly
                // VERY platform-specific, not portable

// Note: "asm volatile" uses two keywords
                // volatile prevents compiler from optimizing away the assembly
                // Example: asm volatile("mfence"); // Memory fence - must execute


/*==============================================================================
 16. LAMBDA & MODERN UTILITIES - Advanced type deduction
==============================================================================*/

// Note: "decltype(auto)" is a combination of two keywords
                // Perfect return type deduction (preserves references and const)
                // Example: decltype(auto) func() {
                //              int x = 5;
                //              return (x);  // Returns int& (reference)
                //          }
                // vs: auto func() would return just int (copy)


/*==============================================================================
 17. CONDITIONAL / PATTERN MATCHING - Compile-time branching
==============================================================================*/

// Note: "if constexpr" is two keywords used together
                // Compile-time if statement (C++17)
                // Example: template<typename T>
                //          void process(T value) {
                //              if constexpr (std::is_integral_v<T>) {
                //                  // Code for integers (only compiled if T is integral)
                //              } else {
                //                  // Code for non-integers
                //              }
                //          }
                // Discarded branch is not compiled (unlike regular if)


/*==============================================================================
 18. CONTEXTUAL KEYWORDS - Special meaning only in certain contexts
==============================================================================*/

final           // TWO USES:
                // 1. Prevents class inheritance
                //    Example: class Sealed final { }; // Can't derive from Sealed
                // 2. Prevents virtual function override
                //    Example: virtual void func() final; // Derived classes can't override
                // Not a reserved word - can be used as identifier elsewhere

override        // Explicitly marks function as overriding a base class virtual function
                // Example: class Derived : public Base {
                //              void func() override; // Must override Base::func()
                //          };
                // Compiler error if no matching virtual function in base
                // Catches typos and signature mismatches

transaction_safe // For transactional memory (experimental, rarely used)
                // Marks function as safe for transactional memory execution
                // Part of Transactional Memory TS (technical specification)

transaction_safe_dynamic
// For transactional memory (experimental, rarely used)
// Dynamic transactional memory safety

/*==============================================================================
 19. HISTORICAL NOTES
==============================================================================*/

// 'register' - Deprecated in C++11, removed meaning in C++17
// Still a keyword but does nothing. Kept for backwards compatibility.

// 'auto' - Meaning changed from "automatic storage" to "type deduction" in
// C++11 Old code using auto for storage is obsolete.

// 'export' - Original meaning (export templates) was removed in C++11
// Repurposed for modules in C++20.

/*==============================================================================
  SUMMARY OF COMMON CONFUSIONS
==============================================================================*/

// const vs constexpr:
//   const = runtime or compile-time constant (value can't change)
//   constexpr = guarantees compile-time evaluation

// class vs struct:
//   Same thing! Only difference: default access (private vs public)

// virtual vs override vs final:
//   virtual = can be overridden (in base class)
//   override = explicitly overriding (in derived class) - compiler checks
//   final = can't be overridden anymore (in derived class)

// auto vs decltype vs decltype(auto):
//   auto = deduce type, removes references/const
//   decltype = deduce exact type including references/const
//   decltype(auto) = perfect forwarding of return type

// new/delete vs malloc/free:
//   new/delete = C++, calls constructors/destructors
//   malloc/free = C, just allocates/deallocates memory
//   Don't mix them!

/*==============================================================================
  END OF FILE
==============================================================================*/
