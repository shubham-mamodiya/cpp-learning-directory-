
/*
===============================================================================
                            C++ TRAITS & CORE DOCUMENTATION
===============================================================================

This file is NOT meant to be compiled.
It is a reference / documentation file written as C++ comments.

Target audience:
- Modern C++ learners (C++17 / C++20)
- People learning from https://www.learncpp.com/
- Programmers implementing templates, iterators, and generic libraries

===============================================================================
TABLE OF CONTENTS
===============================================================================

1. What are Traits in C++?
2. Why Traits Exist
3. Type Traits (std::type_traits)
4. Custom Traits
5. Iterator Traits (std::iterator_traits)
6. Traits vs Concepts (Modern C++)
7. SFINAE and Traits
8. constexpr, type aliases, and modern style
9. General Modern C++ Notes
10. Summary

===============================================================================
1. WHAT ARE TRAITS IN C++?
===============================================================================

A *trait* is a compile-time mechanism used to query or describe properties
of a type.

Traits answer questions like:
- Is this type an integer?
- Is this type const?
- Does this iterator support random access?
- What is the value_type of this iterator?

Traits do NOT store data.
Traits are evaluated at compile time.
Traits are usually implemented as templates.

Example idea:
    "Tell me something about a type, without creating an object."

===============================================================================
2. WHY TRAITS EXIST
===============================================================================

C++ templates work with *unknown types*.

Example:
    template<typename T>
    void foo(T x);

Inside foo(), you do NOT know:
- Is T an int?
- Is T a pointer?
- Is T movable?
- Is T an iterator?

Traits allow templates to adapt behavior depending on type properties.

Without traits:
- You would need many overloads
- Or runtime checks (slow and ugly)

With traits:
- Compile-time decisions
- Zero runtime cost
- Safer generic code

===============================================================================
3. TYPE TRAITS (std::type_traits)
===============================================================================

Header:
    #include <type_traits>

Type traits live in the std namespace.

-------------------------------------------------------------------------------
3.1 Boolean Type Traits
-------------------------------------------------------------------------------

These evaluate to true or false at compile time.

Examples:

    std::is_integral<T>      // int, short, long, char, etc.
    std::is_floating_point<T>
    std::is_pointer<T>
    std::is_reference<T>
    std::is_const<T>
    std::is_same<T, U>

Each trait exposes:
    ::value          (pre-C++17)
    _v version       (C++17+)

Modern usage (preferred):

    std::is_integral_v<T>
    std::is_same_v<T, int>

-------------------------------------------------------------------------------
3.2 Conditional Compilation with Traits
-------------------------------------------------------------------------------

Example:

    template<typename T>
    void print(T value) {
        if constexpr (std::is_integral_v<T>) {
            // integer-specific code
        } else {
            // generic code
        }
    }

if constexpr is evaluated at compile time.
Unused branches are discarded.

-------------------------------------------------------------------------------
3.3 Transforming Type Traits
-------------------------------------------------------------------------------

These modify types:

    std::remove_const<T>
    std::remove_reference<T>
    std::add_pointer<T>
    std::decay<T>

Example:

    using Raw = std::remove_reference_t<T>;

The _t suffix is modern C++ (C++14+).

===============================================================================
4. CUSTOM TRAITS
===============================================================================

You can write your own traits.

-------------------------------------------------------------------------------
4.1 Simple Custom Trait
-------------------------------------------------------------------------------

    template<typename T>
    struct is_string {
        static constexpr bool value = false;
    };

    template<>
    struct is_string<std::string> {
        static constexpr bool value = true;
    };

Modern alias:

    template<typename T>
    inline constexpr bool is_string_v = is_string<T>::value;

-------------------------------------------------------------------------------
4.2 Tag Dispatching (Classic Technique)
-------------------------------------------------------------------------------

Traits can expose *types*, not just booleans.

    struct fast_tag {};
    struct slow_tag {};

    template<typename T>
    struct speed_trait {
        using type = slow_tag;
    };

    template<>
    struct speed_trait<int> {
        using type = fast_tag;
    };

===============================================================================
5. ITERATOR TRAITS (std::iterator_traits)
===============================================================================

Header:
    #include <iterator>

Iterator traits describe properties of iterators.

Standard iterator traits include:

    iterator_category
    value_type
    difference_type
    pointer
    reference

-------------------------------------------------------------------------------
5.1 Why iterator_traits exist
-------------------------------------------------------------------------------

Algorithms (like std::sort, std::advance) do NOT know:
- What container is used
- What iterator type is passed

They rely on iterator_traits to extract this information.

-------------------------------------------------------------------------------
5.2 iterator_category
-------------------------------------------------------------------------------

Tells what operations an iterator supports.

Common categories:

    std::input_iterator_tag
    std::output_iterator_tag
    std::forward_iterator_tag
    std::bidirectional_iterator_tag
    std::random_access_iterator_tag
    std::contiguous_iterator_tag (C++20)

Example:
    If iterator_category is random_access,
    std::advance can do pointer arithmetic.

-------------------------------------------------------------------------------
5.3 Implementing iterator traits in custom iterators
-------------------------------------------------------------------------------

Example skeleton:

    struct MyIterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    };

IMPORTANT:
- The names MUST match exactly
- The types do NOT have to be identical to other iterators
- They must describe YOUR iterator correctly

-------------------------------------------------------------------------------
5.4 std::iterator_traits
-------------------------------------------------------------------------------

Used like this:

    std::iterator_traits<It>::value_type
    std::iterator_traits<It>::difference_type

Works for:
- Custom iterators
- Raw pointers (specialized by the standard)

===============================================================================
6. TRAITS VS CONCEPTS (MODERN C++)
===============================================================================

Concepts (C++20) are a cleaner replacement for many trait usages.

Example:

    template<typename T>
    requires std::integral<T>
    void foo(T);

Instead of:

    template<typename T>
    std::enable_if_t<std::is_integral_v<T>> foo(T);

Traits still matter:
- Concepts are built USING traits
- Legacy code uses traits
- Traits are more flexible

===============================================================================
7. SFINAE AND TRAITS
===============================================================================

SFINAE = Substitution Failure Is Not An Error

Traits are often used with SFINAE.

Example:

    template<typename T>
    std::enable_if_t<std::is_integral_v<T>>
    func(T);

Modern replacement:
- if constexpr
- concepts

Avoid enable_if unless necessary.

===============================================================================
8. constexpr, TYPE ALIASES, MODERN STYLE
===============================================================================

Prefer:

    using value_type = T;

Over:

    typedef T value_type;

Prefer:

    std::is_same_v<T, U>

Over:

    std::is_same<T, U>::value

Use:
- constexpr
- inline constexpr variables
- type aliases (_t)

===============================================================================
9. GENERAL MODERN C++ NOTES
===============================================================================

- Prefer RAII
- Avoid raw new/delete
- Use std::unique_ptr / std::shared_ptr
- Prefer range-based for
- Use std::span (C++20) for views
- Prefer algorithms over loops
- Prefer value semantics

Traits are foundational for:
- STL
- Iterators
- Allocators
- Ranges
- Concepts

===============================================================================
10. SUMMARY
===============================================================================

Traits:
- Describe types at compile time
- Enable generic programming
- Have zero runtime cost

Key trait families:
- std::type_traits
- std::iterator_traits
- Custom traits

Modern C++:
- Uses traits internally
- Exposes concepts as a cleaner interface
- Still requires understanding traits deeply

If you understand traits, templates stop feeling like magic.

===============================================================================
END OF FILE
===============================================================================
*/
