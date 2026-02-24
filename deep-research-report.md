# C++ Identifier Naming Rules  
C++ identifiers must follow language rules: start with a letter or underscore, contain only letters, digits, or underscores, and not be a C++ keyword【1†L175-L183】. Identifiers are case-sensitive (`value` ≠ `Value`). As a guideline, **avoid names beginning with an underscore** (those are reserved)【1†L246-L254】. Use *descriptive* names that reflect meaning – e.g. `customerCount` is clearer than `ccount`【1†L249-L254】. Keep names proportionate to scope: short for local indices, longer for global or widely-used values【1†L262-L270】.

## Variables and Function Names  
- **LearnCpp style:** Variable and function names typically start with a **lowercase** letter【1†L189-L197】. Multi-word names can use *snake_case* or *lowerCamelCase* (intercapped), as long as you stay consistent. For example:  
  ```cpp
  int totalCount;      // lowerCamelCase (common in tutorials)
  int total_count;     // snake_case (also common in C/C++)
  void processData();  // function name lowerCamel or snake
  ```  
  Both `my_variable_name` and `myVariableName` are conventional【1†L209-L217】. The LearnCpp tutorials prefer camelCase for readability, but note the C++ standard library often uses underscores.  
- **Google style:** Uses **snake_case** for variables and parameters (all lowercase with `_`), and adds a trailing underscore for private class data members【39†L3325-L3330】. For instance:  
  ```cpp
  std::string file_path;      // ok: snake_case variable
  class Example {
    int width_;               // ok: private member (trailing underscore)
  };
  ```  
- **Avoid:** Do not start variable/function names with a capital letter (that’s usually reserved for types)【1†L225-L229】, and don’t use C++ keywords as names【1†L175-L183】.

## Program-Defined Types (Classes, Structs, Enums, Aliases)  
- **LearnCpp / Common practice:** User-defined types (classes, structs, enums) **start with a capital letter** and use PascalCase (one capital per word)【4†L179-L184】. Don’t add suffixes like `_t`. For example:  
  ```cpp
  class Player { ... };
  struct DataRecord { ... };
  enum Color { /* ... */ };
  ```  
  This makes types visually distinct from variables (which start lowercase).  
- **Type aliases:** Follow type-naming rules too. Both LearnCpp and Google say alias names should start with a capital letter and have no suffix【3†L124-L128】【39†L3300-L3303】. E.g.:  
  ```cpp
  using Size = std::size_t;   // capital letter
  using Speed = float;        // capital letter
  ```  
- **Google style:** Type names (classes, structs, enums, typedefs) are **PascalCase** with no underscores【39†L3300-L3303】 (e.g. `UrlTable`, `PropertiesMap`). Enums declared `enum class Foo` follow the same rule.  

## Constants and Enumerations  
- **Const variables:** LearnCpp recommends treating `const` or `constexpr` variables like normal variables (no special ALL-CAPS)【2†L182-L185】. In practice:  
  ```cpp
  const double earthGravity = 9.8;  // LearnCpp-preferred: like a regular variable
  constexpr int maxUsers = 100;     // same style, lowercase first letter
  ```  
  (Many legacy guides use `EARTH_GRAVITY` or `kEarthGravity`, but LearnCpp prefers the ordinary naming style【2†L182-L185】.)  
- **Google style:** Uses a **leading “k” + PascalCase** for fixed constants【39†L3373-L3380】. For example:  
  ```cpp
  const int kDaysInAWeek = 7;  // Google style constant
  constexpr float kPi = 3.14159;
  ```  
- **Enumerations:** Enum *type* names follow type rules (PascalCase)【29†L220-L228】. For example: `enum Color { … };`. LearnCpp suggests enumerator *values* start lowercase (like `red, green, blue`)【29†L241-L242】. Google’s style uses the same convention as constants for enum values (e.g. `kOk, kOutOfMemory`)【39†L3439-L3442】. In summary:  
  ```cpp
  enum Color { red, green, blue };             // LearnCpp example (type capital, values lower-case)
  enum class Status { kOk, kError, kMissing }; // Google example (values use k-prefix)
  ```

## Global vs Member Variables  
- **Global variables:** Often prefixed with `g_` or `g` to indicate global scope【5†L159-L168】. For example:  
  ```cpp
  int g_totalScore;      // global variable (LearnCpp suggests "g_" prefix)
  ```  
  This avoids name collisions and signals “static” duration【5†L159-L168】【5†L175-L179】. If globals live in a namespace, the prefix can be optional (the namespace already provides context)【5†L169-L173】.  
- **Class/struct members:**  
  - *Private data members* are commonly prefixed with `m_`【8†L229-L237】. This makes it clear inside methods that `m_value` is a member, not a local variable or parameter【8†L241-L249】. For instance:  
    ```cpp
    class Rectangle {
      int m_width;          // private member
      int m_height;         // private member
    public:
      void setWidth(int width) { m_width = width; }  // prefix avoids shadowing
    };
    ```  
    LearnCpp notes this prefix prevents naming collisions with parameters or functions【8†L229-L237】【8†L263-L267】. (Public members in structs often omit `m_` by convention.)  
  - *Static members:* Follow the same rules as regular members. Constants inside classes still use the constant naming rule (e.g. `static constexpr int kMaxX = 5;` with `k` prefix in Google style)【39†L3349-L3354】.  
- **Example:**  
  ```cpp
  int g_counter;        // global (recommended prefix g_)
  class MyClass {
    int m_age;          // private member (prefix m_)
    std::string name;   // public (no m_ prefix in struct-style classes)
  };
  ```

## Function Names  
LearnCpp generally uses the same lowercase-start style for functions (often using camelCase)【1†L189-L197】. Google’s style guide, however, prefers **PascalCase** (CapitalizedWords) for normal functions【39†L3403-L3406】. For example:  
```cpp
void computeScore();     // LearnCpp style (lowercase start)
void ComputeScore();     // Google style (capital start)
```
Accessor and mutator names in Google style *may* use snake_case (e.g. `int count() const; void set_count(int);`)【39†L3417-L3420】, but either convention is often fine as long as it’s consistent.

## Macros and Preprocessor  
Macros are discouraged, but if you must use them, name them **ALL_UPPERCASE_WITH_UNDERSCORES** with a project-specific prefix【43†L3473-L3475】. For example:  
```cpp
#define MYPROJECT_MAX_BUFFER_SIZE 1024  // macro name, all caps
```
This follows the traditional convention (similar to C) and avoids clashing with normal identifiers【43†L3473-L3475】.

## Namespace Names  
Namespaces help avoid name clashes, so they should be **lowercase with underscores** (snake_case)【39†L3421-L3424】. For example:  
```cpp
namespace my_app {
    // ...
}
namespace math_utils {
    // ...
}
```

## Best Practices and Summary  
- **Consistency:** Always match the naming style of the codebase you’re working in【1†L236-L238】. If starting a new project, pick a coherent style (e.g. all snake_case or all lowerCamel) and stick with it.  
- **Descriptiveness:** Choose names that clearly indicate purpose. Avoid inscrutable abbreviations.  
- **Avoid underscores at start:** Names beginning with an underscore (especially `_X` or `__X`) are reserved by the implementation and should be avoided【1†L246-L254】.  
- **Use common conventions:** Follow established style guides (e.g. LearnCpp, Google C++ Style) as much as possible. These ensure readability and reduce confusion (for example, not mixing up type and variable names by case).  

### Convention Cheat Sheet

| Entity                | LearnCpp/Modern C++ Style                    | Google Style (common)               | Example                      |
|-----------------------|----------------------------------------------|-------------------------------------|------------------------------|
| Variables (local)     | lowerCamelCase or snake_case (start lowercase)【1†L189-L197】 | snake_case, start lowercase【39†L3325-L3330】 | `int totalCount;`<br>`int total_count;` |
| Class Data Members    | Prefix `m_` + lowerCamelCase【8†L229-L237】   | snake_case + trailing `_`【39†L3325-L3330】| `int m_score;`<br>`std::string table_name_;` |
| Global Variables      | Prefix `g_` (in global scope)【5†L159-L168】  | similar (often `g_` or `g`)        | `int g_totalScore;`         |
| Functions             | lowerCamelCase or snake (start lowercase)【1†L189-L197】 | PascalCase (start uppercase)【39†L3403-L3406】 | `void processData();`<br>`void ComputeData();` |
| Types (class/struct/enum) | PascalCase (start uppercase)【4†L179-L184】      | PascalCase (start uppercase)【39†L3300-L3303】 | `class Player { ... };`   |
| Type Aliases          | PascalCase (start uppercase)【3†L124-L128】    | PascalCase                          | `using Distance = double;`   |
| Constants (`const`/`constexpr`) | like normal variables (lowerCamel/snake)【2†L182-L185】 | `k` + PascalCase【39†L3373-L3380】  | `const int maxValue = 5;`<br>`const int kMaxValue = 5;` |
| Enumerations          | Type name PascalCase; enumerators lowercase【29†L241-L242】 | Type PascalCase; enumerators `k`-prefix【39†L3439-L3442】 | `enum Color { red, green };`<br>`enum class Error { kOk, kFail };` |
| Macros                | ALL_UPPER_SNAKE (if used)                     | ALL_UPPER_SNAKE, project prefix【43†L3473-L3475】 | `#define MYPROJ_MAX 100` |
| Namespaces            | snake_case (lowercase)                        | snake_case (lowercase)【39†L3421-L3424】 | `namespace audio_effects { ... }` |

**Sources:** LearnCpp.com tutorials on naming (variables【1†L189-L197】, constants【2†L182-L185】, types【4†L179-L184】, type aliases【3†L124-L128】, global/`g_` prefix【5†L159-L168】, member `m_` prefix【8†L229-L237】, enums【29†L228-L237】【29†L241-L242】) and the Google C++ Style Guide【39†L3300-L3303】【39†L3325-L3330】【39†L3373-L3380】【39†L3403-L3406】【39†L3439-L3442】【43†L3473-L3475】. These reflect current common practices in modern C++ code.