
/*
===============================================================================
 C++ VALUE CATEGORIES & FUNCTION PARAMETER CHEAT SHEET
 Modern C++ (C++17 / C++20 friendly)

 This file is meant as a long-term reference.
 Read the comments carefully — they matter more than the code.

===============================================================================

VALUE CATEGORIES (VERY IMPORTANT)

1) lvalue:
   - Has a name
   - Has a stable memory address
   - Can appear on the LEFT side of '='

   Example:
       int x = 10;
       x = 20;   // x is an lvalue

2) rvalue:
   - Temporary object
   - Usually has no name
   - Often appears on the RIGHT side of '='

   Example:
       int y = x + 5;    // (x + 5) is an rvalue
       std::string{"hi"}; // temporary => rvalue

===============================================================================
REFERENCE TYPES

T&        -> lvalue reference
const T&  -> read-only reference (binds to lvalues AND rvalues)
T&&       -> rvalue reference (binds to rvalues only, unless template case)

===============================================================================
CHEAT SHEET: HOW TO ACCEPT PARAMETERS
===============================================================================
*/

#include <string>
#include <utility>

/*
-----------------------------------------------------------------------------
1) PASS BY VALUE (BEST DEFAULT IN MODERN C++)
-----------------------------------------------------------------------------
*/

void by_value(std::string s) {
    /*
    What happens:
    - lvalue argument  -> COPY
    - rvalue argument  -> MOVE

    Why this is great:
    - One function
    - Clear ownership
    - Compiler optimizes moves well

    Use this when:
    - You need your own copy anyway
    - The type is cheap to move
    */
}

/*
-----------------------------------------------------------------------------
2) PASS BY CONST LVALUE REFERENCE (READ-ONLY)
-----------------------------------------------------------------------------
*/

void by_const_ref(const std::string& s) {
    /*
    Properties:
    - Accepts both lvalues and rvalues
    - No copy
    - No move
    - Cannot modify or steal from s

    Use this when:
    - You only need to READ
    - You do not store or modify the object
    */
}

/*
-----------------------------------------------------------------------------
3) PASS BY NON-CONST LVALUE REFERENCE (MODIFY CALLER)
-----------------------------------------------------------------------------
*/

void by_lvalue_ref(std::string& s) {
    /*
    Properties:
    - Accepts ONLY lvalues
    - Allows modification of caller’s object

    Use this when:
    - You intentionally want to modify the argument
    - Very explicit API contract
    */

    s += " modified";
}

/*
-----------------------------------------------------------------------------
4) PASS BY RVALUE REFERENCE (MOVE / CONSUME)
-----------------------------------------------------------------------------
*/

void by_rvalue_ref(std::string&& s) {
    /*
    Properties:
    - Accepts ONLY rvalues
    - Usually moved-from
    - Caller should not expect s to remain usable

    Use this when:
    - You want to "consume" temporaries
    - Performance-critical library code
    */
}

/*
-----------------------------------------------------------------------------
5) OVERLOADING FOR LVALUE AND RVALUE (RARE)
-----------------------------------------------------------------------------
*/

void overloaded(const std::string& s) {
    // Called for lvalues
}

void overloaded(std::string&& s) {
    // Called for rvalues (can std::move(s))
}

/*
RULE:
❌ Do NOT do this unless behavior or performance is meaningfully different.
✅ Application code rarely needs this.
*/

/*
-----------------------------------------------------------------------------
6) PERFECT FORWARDING (ADVANCED / GENERIC CODE)
-----------------------------------------------------------------------------
*/

template <typename T> void forwarding(T&& arg) {
    /*
    IMPORTANT:
    - T&& here is a *forwarding reference*
    - Preserves value category

    If arg was:
      - lvalue -> forwarded as lvalue
      - rvalue -> forwarded as rvalue
    */

    consume(std::forward<T>(arg));
}

void consume(const std::string&) {}
void consume(std::string&&) {}

/*
Use this when:
- Writing templates
- Wrappers
- Factories
- emplace-style APIs
*/

/*
-----------------------------------------------------------------------------
MODERN API DESIGN PATTERN (RECOMMENDED)
-----------------------------------------------------------------------------
*/

class Widget {
  public:
    void set_name(std::string name) {
        /*
        Modern pattern:
        - Accept by value
        - Move into member

        Benefits:
        - One function
        - Efficient for rvalues
        - Clean interface
        */

        m_name = std::move(name);
    }

  private:
    std::string m_name;
};

/*
-----------------------------------------------------------------------------
QUICK DECISION TABLE
-----------------------------------------------------------------------------
Need to READ only?              -> const T&
Need ownership / store value?   -> T
Need to modify caller object?   -> T&
Need to consume temporaries?    -> T&&
Generic forwarding?             -> template <typename T> T&&

-----------------------------------------------------------------------------
GOLDEN RULE (MEMORIZE THIS)
-----------------------------------------------------------------------------
"If you are unsure, pass by value or const reference.
 Only use & / && overloads when you have a clear reason."

===============================================================================
END OF CHEAT SHEET
===============================================================================
*/

int main() {
    std::string a = "hello";

    by_value(a);                  // copy
    by_value(std::string{"tmp"}); // move

    by_const_ref(a);
    by_const_ref("temporary");

    by_lvalue_ref(a);
    // by_lvalue_ref("tmp");        // ❌ ERROR (not an lvalue)

    by_rvalue_ref(std::string{"x"});
    // by_rvalue_ref(a);            // ❌ ERROR

    return 0;
}
