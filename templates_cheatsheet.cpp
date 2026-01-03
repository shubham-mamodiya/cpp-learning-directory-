
// ==========================================================
// C++ TEMPLATES — PRACTICAL CHEAT SHEET
// ==========================================================
//
// Think of templates as:
// "Let the compiler generate code for me, based on types"
//
// ==========================================================
// 1. CLASS TEMPLATE (MOST COMMON)
// ==========================================================

template <typename T> class Box {
  T value;
};

// Usage:
Box<int> a;         // T = int
Box<std::string> b; // T = std::string

// Rule:
// - T is chosen ONCE when the class is instantiated
// - T is fixed everywhere inside the class

// ==========================================================
// 2. FUNCTION TEMPLATE
// ==========================================================

template <typename T> T add(T a, T b) { return a + b; }

// Usage:
add(1, 2);     // T = int
add(1.5, 2.3); // T = double

// Rule:
// - T is deduced EACH TIME the function is called

// ==========================================================
// 3. CLASS TEMPLATE + FUNCTION TEMPLATE TOGETHER
// ==========================================================

template <typename T> class Wrapper {
  T value;

public:
  // Function template inside a class template
  template <typename U> Wrapper(U &&v) : value(v) {}
};

// Meaning:
// - T = what the class stores
// - U = what the function accepts

// ==========================================================
// 4. WHEN YOU NEED ANOTHER TEMPLATE (VERY IMPORTANT)
// ==========================================================
//
// Use another template parameter ONLY when:
//
// - T is already fixed
// - but you want to accept DIFFERENT input types
//
// Example:

template <typename T> struct Node {
  T data;

  // Accept anything that can build a T
  template <typename U> Node(U &&v) : data(std::forward<U>(v)) {}
};

// If T = std::string
// U can be: std::string, const char*, etc.
// If T cannot be built from U -> COMPILE ERROR

// ==========================================================
// 5. WHEN YOU DO NOT NEED ANOTHER TEMPLATE
// ==========================================================

template <typename T> struct SimpleNode {
  T data;

  SimpleNode(const T &v) : data(v) {}       // copy
  SimpleNode(T &&v) : data(std::move(v)) {} // move
};

// Use this when:
// - input type == stored type
// - you want simpler code

// ==========================================================
// 6. U&& RULE (FORWARDING REFERENCE RULE)
// ==========================================================
//
// U&& is special ONLY when:
// - U is a template parameter
// - U is deduced
//
// Then:
//
// lvalue  -> U = T&  -> U&& becomes T&
// rvalue  -> U = T   -> U&& becomes T&&
//
// This is how forwarding works.

// ==========================================================
// 7. std::forward VS std::move
// ==========================================================
//
// std::move(x)
// - always allows moving
// - use when YOU own x
//
// std::forward<U>(x)
// - preserves how caller passed x
// - use ONLY with U&&

// ==========================================================
// 8. WHAT std::forward DOES NOT DO
// ==========================================================
//
// ❌ does not move memory
// ❌ does not copy memory
// ❌ does not allocate
//
// It is ONLY a cast that decides:
// "should the next operation copy or move?"

// ==========================================================
// 9. COMMON BEGINNER RULES (FOLLOW THESE)
// ==========================================================
//
// ✔ Use template <typename T> for containers
// ✔ Use T everywhere inside the class
// ✔ Add U only if you need flexible input
// ✔ Prefer const T& + T&& when learning
// ✔ Prefer std::vector / std::unique_ptr
// ✘ Do NOT use const T&&
// ✘ Do NOT over-template early

// ==========================================================
// 10. MENTAL MODEL (MOST IMPORTANT)
// ==========================================================
//
// T  -> what the class STORES
// U  -> what a function ACCEPTS
//
// Class templates fix types.
// Function templates deduce types.
//
// If a value cannot convert to T -> compile-time error.
//
// ==========================================================
