
// ============================================================
// Modern C++ Constructors & Special Member Functions Cheat Sheet
// ============================================================
//
// Use this file as a mental reference while designing classes.
//
// ------------------------------------------------------------
// 1. FIRST QUESTION TO ASK
// ------------------------------------------------------------
// Does this class OWN a resource?
//
// Resource examples:
// - new / delete
// - malloc / free
// - file handles
// - sockets
// - mutexes
// - raw pointers with ownership
//
// If NO  -> Rule of Zero
// If YES -> Rule of Five
//

// ============================================================
// 2. RULE OF ZERO (MOST COMMON, MOST IMPORTANT)
// ============================================================
// If your class does NOT manage resources directly,
// DO NOT write any of these:
//
// - destructor
// - copy constructor
// - copy assignment
// - move constructor
// - move assignment
//
// Let the compiler generate everything.
//

#include <string>
#include <utility>
#include <vector>

struct Point {
  int x{};
  int y{};
  // No constructors needed
};

struct Node {
  std::string data;
  Node *next = nullptr;
  Node *prev = nullptr;

  // One constructor using perfect forwarding
  template <typename U>
  Node(U &&value, Node *n = nullptr, Node *p = nullptr)
      : data(std::forward<U>(value)), next(n), prev(p) {}
};
// ✔ Rule of Zero
// ✔ Compiler-generated copy & move are correct

// ============================================================
// 3. PERFECT FORWARDING CONSTRUCTOR (LVALUES + RVALUES)
// ============================================================
// Use this instead of writing const T& and T&& separately
//

template <typename T> struct Box {
  T value;

  template <typename U> explicit Box(U &&v) : value(std::forward<U>(v)) {}
};

// ❌ Never write: const T&&
// ❌ Rarely write: both const T& and T&&

// ============================================================
// 4. RULE OF FIVE (ONLY WHEN YOU OWN A RESOURCE)
// ============================================================
// If you manually manage a resource, you must define:
//
// 1. Destructor
// 2. Copy constructor
// 3. Copy assignment
// 4. Move constructor
// 5. Move assignment
//

#include <algorithm>
#include <cstddef>

class Buffer {
  std::size_t size{};
  int *data{nullptr};

public:
  // Constructor
  explicit Buffer(std::size_t s) : size{s}, data{new int[s]{}} {}

  // Destructor
  ~Buffer() { delete[] data; }

  // Copy constructor (deep copy)
  Buffer(const Buffer &other) : size{other.size}, data{new int[other.size]} {
    std::copy(other.data, other.data + size, data);
  }

  // Copy assignment
  Buffer &operator=(const Buffer &other) {
    if (this != &other) {
      delete[] data;
      size = other.size;
      data = new int[size];
      std::copy(other.data, other.data + size, data);
    }
    return *this;
  }

  // Move constructor
  Buffer(Buffer &&other) noexcept : size{other.size}, data{other.data} {
    other.data = nullptr;
    other.size = 0;
  }

  // Move assignment
  Buffer &operator=(Buffer &&other) noexcept {
    if (this != &other) {
      delete[] data;
      size = other.size;
      data = other.data;
      other.data = nullptr;
      other.size = 0;
    }
    return *this;
  }
};

// ============================================================
// 5. AVOID RULE OF FIVE BY USING RAII TYPES
// ============================================================
// Prefer std::vector, std::string, std::unique_ptr
//

class BetterBuffer {
  std::vector<int> data;

public:
  explicit BetterBuffer(std::size_t s) : data(s) {}
};
// ✔ Rule of Zero again
// ✔ Compiler handles copy & move

// ============================================================
// 6. DISABLING COPYING (BUT ALLOW MOVING)
// ============================================================

class NonCopyable {
public:
  NonCopyable() = default;

  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;

  NonCopyable(NonCopyable &&) = default;
  NonCopyable &operator=(NonCopyable &&) = default;
};

// ============================================================
// 7. QUICK DECISION FLOW (REMEMBER THIS)
// ============================================================
//
// Does the class own a resource?
//   |
//   |-- NO  --> Rule of Zero (write nothing)
//   |
//   |-- YES --> Can I use std::vector / unique_ptr?
//               |
//               |-- YES --> Rule of Zero
//               |
//               |-- NO  --> Rule of Five
//
// ============================================================
//
// GOLDEN RULES:
// - Prefer Rule of Zero
// - Avoid raw owning pointers
// - Never use const T&&
// - One forwarding constructor > many overloads
// - Let the compiler help you
//
// ============================================================
