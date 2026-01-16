
/*
===============================================================================
                                ITERATORS IN C++
===============================================================================

This file is a *documentation-style reference* for C++ iterators.
Everything is explained using comments so it can be read like a guide.

This follows MODERN C++ practices (C++17 / C++20) and is compatible
with how the standard library expects iterators to behave.

===============================================================================
1. WHAT IS AN ITERATOR?
===============================================================================

An iterator is an OBJECT that behaves like a generalized pointer.

A raw pointer:
    int* p;

An iterator:
    std::vector<int>::iterator it;

Both support:
    - Dereferencing   (*it)
    - Incrementing    (++it)
    - Comparison      (it == other)

Iterators allow algorithms (std::sort, std::find, std::copy, etc.)
to work with ANY container in a uniform way.

Key idea:
    Algorithms do NOT know about containers.
    Containers provide iterators.
    Algorithms operate ONLY on iterators.

This is the heart of the C++ STL design.

===============================================================================
2. WHY ITERATORS EXIST
===============================================================================

Without iterators:
    - Every algorithm would need to know container internals
    - Algorithms would be duplicated for vector, list, deque, etc.

With iterators:
    - Algorithms are container-agnostic
    - Containers expose traversal logic safely
    - Separation of concerns

Example:
    std::sort(v.begin(), v.end());
    std::find(l.begin(), l.end(), 42);

The algorithm does not care whether:
    - v is a vector
    - l is a list
    - data is contiguous or not

===============================================================================
3. BASIC ITERATOR OPERATIONS
===============================================================================

For a type to behave like an iterator, it must support:

    *it         -> access element
    ++it        -> move forward
    it == other -> comparison
    it != other -> comparison

Additional operations depend on iterator category.

===============================================================================
4. ITERATOR CATEGORIES
===============================================================================

The C++ standard defines iterator *capabilities* via categories.

From weakest to strongest:

1) Input Iterator
   - Read-only, single pass
   - Examples: std::istream_iterator

2) Output Iterator
   - Write-only, single pass
   - Examples: std::ostream_iterator

3) Forward Iterator
   - Read/write
   - Multi-pass
   - Examples: std::forward_list

4) Bidirectional Iterator
   - Can move forward and backward
   - Examples: std::list, std::map

5) Random Access Iterator
   - Supports +, -, indexing
   - Examples: std::vector, std::deque

6) Contiguous Iterator (C++20)
   - Memory is contiguous
   - Examples: std::vector, std::array

Why this matters:
    Algorithms REQUIRE certain iterator capabilities.

Example:
    std::sort → requires Random Access Iterator
    std::reverse → requires Bidirectional Iterator

===============================================================================
5. ITERATOR TRAITS
===============================================================================

Iterator traits describe properties of an iterator type.

They allow algorithms to ask questions like:
    - What type does this iterator point to?
    - How far apart are two iterators?

Standard traits:

    value_type
    difference_type
    reference
    pointer
    iterator_category

Provided via:
    std::iterator_traits<Iterator>

Example:
    using value = std::iterator_traits<It>::value_type;

Modern note:
    std::iterator (base class) is DEPRECATED.
    You must define traits manually in your iterator.

===============================================================================
6. MINIMAL CUSTOM ITERATOR (BIDIRECTIONAL)
===============================================================================

Below is a SIMPLE custom iterator implementation for a node-based container.

This shows the MECHANICS, not a full container.

*/

#include <cstddef>
#include <iterator>

template <typename T>
class NodeIterator {
public:
    // =====================
    // Iterator traits
    // =====================
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

private:
    pointer ptr = nullptr;

public:
    // =====================
    // Constructors
    // =====================
    NodeIterator() = default;
    explicit NodeIterator(pointer p) : ptr(p) {}

    // =====================
    // Dereference
    // =====================
    reference operator*() const {
        return *ptr;
    }

    pointer operator->() const {
        return ptr;
    }

    // =====================
    // Increment
    // =====================
    NodeIterator& operator++() {
        ++ptr;               // advance
        return *this;
    }

    NodeIterator operator++(int) {
        NodeIterator temp = *this;
        ++(*this);
        return temp;
    }

    // =====================
    // Decrement
    // =====================
    NodeIterator& operator--() {
        --ptr;
        return *this;
    }

    NodeIterator operator--(int) {
        NodeIterator temp = *this;
        --(*this);
        return temp;
    }

    // =====================
    // Comparison
    // =====================
    friend bool operator==(const NodeIterator& a,
                           const NodeIterator& b) {
        return a.ptr == b.ptr;
    }

    friend bool operator!=(const NodeIterator& a,
                           const NodeIterator& b) {
        return !(a == b);
    }
};

/*
===============================================================================
7. HOW CONTAINERS USE ITERATORS
===============================================================================

A container exposes iterators like this:

    iterator begin();
    iterator end();

Example:

    NodeIterator<int> begin() { return NodeIterator<int>(data_start); }
    NodeIterator<int> end()   { return NodeIterator<int>(data_end); }

"end()" points ONE PAST the last element.

This allows:
    for (auto it = begin(); it != end(); ++it)

Range-based for works automatically:

    for (auto& x : container)

Because the compiler translates it to:
    begin(container), end(container)

===============================================================================
8. CONST ITERATORS
===============================================================================

Containers usually define TWO iterator types:

    iterator       → allows modification
    const_iterator → read-only access

Rule:
    const_iterator::reference must be const T&

Why:
    Prevent modification through const containers.

===============================================================================
9. MODERN C++20 NOTES
===============================================================================

- Prefer concepts when possible:
      std::input_iterator
      std::forward_iterator
      std::random_access_iterator

- Prefer ranges:
      std::ranges::sort(container);

- Avoid inheriting from std::iterator (deprecated)

===============================================================================
10. COMMON MISTAKES
===============================================================================

❌ Forgetting iterator traits
❌ Returning by value instead of reference in operator*
❌ Making end() point to last element
❌ Breaking ++it semantics
❌ Not supporting const-correctness

===============================================================================
11. MENTAL MODEL (IMPORTANT)
===============================================================================

Think of iterators as:

    "Objects that know how to move and access elements"

NOT:
    "Just pointers"

Pointers are ONE implementation of iterators.
Iterators are an ABSTRACTION.

===============================================================================
END OF ITERATOR DOCUMENTATION
===============================================================================
*/
