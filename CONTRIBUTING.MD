# Contributing to HolyC++

## 🎯 Welcome!

Thank you for your interest in contributing to HolyC++! This project is a modern reimplementation of Terry A. Davis's HolyC programming language, aiming to preserve its spirit while adding modern safety features and usability improvements.

## 📊 Current Implementation Status

### ✅ **PHASE 1: COMPLETE - Core Type System & Error Framework**

#### **Integer Type System (FULLY IMPLEMENTED)**
**What's Done:**
- ✅ **All HolyC Integer Types**: U8, U16, U32, U64, I8, I16, I32, I64
- ✅ **HolyC Compatibility**: Pointer conversions, hex operations, print methods
- ✅ **Modern Safety**: Optional bounds checking, overflow detection
- ✅ **Zero-Cost Abstractions**: Compiles to native operations when safe
- ✅ **Comprehensive Testing**: Full test suite with edge cases
- ✅ **Documentation**: Complete specification in `INT.MD`

**Key Features:**
- Faithful HolyC behavior with opt-in safety
- Exception-based error handling for dangerous operations
- Template-based implementation for type safety
- Complete arithmetic, bitwise, and comparison operations
- Memory-efficient with no runtime overhead in unsafe mode

#### **Error System (FOUNDATIONAL IMPLEMENTATION)**
**What's Done:**
- ✅ **Core Architecture**: `CompilerError`, `ContextualError`, `InternalError` base classes
- ✅ **Error Management**: Singleton `ErrorManager` with statistics and reporting
- ✅ **Source Location**: File, line, column tracking with `SourceLocation`
- ✅ **Builder Pattern**: Fluent interface for complex error construction
- ✅ **Error Categories**: Lexer, Parser, Type, Semantic, CodeGen errors
- ✅ **Error Code Registry**: Predefined error codes with categories
- ✅ **Severity Levels**: NOTE, WARNING, ERROR, FATAL
- ✅ **Testing**: Comprehensive test suite in `tests/test_error.cpp`

**What's Working:**
- Basic error reporting and collection
- Source location tracking
- Error formatting and output
- Configurable error handling (warnings as errors, suppression)
- Contextual error stacks for debugging
- Error code lookup and validation

### ✅ **PHASE 2: COMPLETE - Floating Point & Union Types**

#### **Floating Point Types (FULLY IMPLEMENTED)**
**What's Done:**
- ✅ **F32**: 32-bit floating point (IEEE-754 single precision, HolyC++ extension)
- ✅ **F64**: 64-bit floating point (IEEE-754 double precision, matches HolyC's "double")
- ✅ **Math Functions**: sqrt(), pow(), sin(), cos(), tan(), floor(), ceil(), round()
- ✅ **Special Values**: NaN, Infinity detection (is_nan(), is_inf())
- ✅ **Arithmetic Operations**: +, -, *, /, % with proper floating-point semantics
- ✅ **Type Conversions**: Seamless conversion from/to integer types
- ✅ **Error Handling**: Division by zero, domain errors with proper exceptions
- ✅ **Testing**: Comprehensive test suite in `tests/test_float.cpp`

**Key Features:**
- Direct F64 compatibility with HolyC's double type
- Optional single-precision F32 for modern applications
- Full suite of mathematical functions
- Proper handling of special floating-point values
- Type-safe operations with runtime error checking

#### **Union Type System (FULLY IMPLEMENTED)**
**What's Done:**
- ✅ **U0 Type**: Void type for union compatibility, trivially constructible
- ✅ **Type-Safe Union**: `Union<Types...>` template wrapper
- ✅ **HolyC Value Class**: Exact implementation from documentation
- ✅ **Plain C Unions**: Direct HolyC compatibility with exact memory layout
- ✅ **Active Type Tracking**: Runtime type information and checking
- ✅ **Memory Management**: Proper construction/destruction of union members
- ✅ **Unsafe Access**: `as<T>()` method for HolyC-style unsafe casting
- ✅ **Move/Copy Semantics**: Full support for modern C++ semantics
- ✅ **Union Macros**: `UNION_BEGIN()`, `UNION_END()`, `UNION()` macros
- ✅ **Testing**: Comprehensive test suite in `tests/test_union.cpp`

**HolyC Examples Implemented:**
```c
// HolyC union example:
union Example {
  I32 age;
  U8 ch;
};

// HolyC Value class example:
#define FLOAT_TYPE 0
#define CHAR_TYPE  1
#define VALUE_TYPE 2

class Value {
  I32 type;
  union {
    F64 f;
    U8 ch;
    Value *val;
  };
};
```

**Key Features:**
- Type-safe `get<T>()` with runtime type checking
- Unsafe `as<T>()` for HolyC-style programming
- Memory layout identical to plain C unions
- Proper lifetime management of union members
- C-compatible interface for linking with C code

### 🔥 **URGENTLY NEEDED: Phase 3 Implementation**

## 🏛️ Project Philosophy

Before contributing, please understand our core principles:

1. **HolyC Compatibility First**: We follow the [HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables) as our reference
2. **No Bootstrapping**: This implementation will **never** be self-hosted in HolyC++ (author's philosophical choice)
3. **Modern Safety**: Add optional safety features without breaking HolyC compatibility
4. **Educational Value**: Clean, readable code for studying language and compiler design
5. **Community-Driven**: Open development welcoming all contributors
6. **Freedom to Fork**: Others may create bootstrapped versions if they wish

**Important Note from the Author (@DebadityaMalakar):**  
*"I am not bootstrapping but you may fork and make a bootstrapped version for yourself."*  
This means while the main project will never self-host, forks that implement bootstrapping are welcome and permitted under GPLv3.

## 🚧 **URGENTLY NEEDED: Phase 3 Implementation**

### **HIGH PRIORITY - String & Memory System** 🔥 **URGENT**

#### **1. String Type Implementation** 🔥 **URGENT**
- **Status**: Not started
- **Goal**: HolyC-style string handling with modern safety
- **Requirements**:
  - Null-terminated string type with bounds checking
  - String operations: concatenation, slicing, comparison
  - String literals in parser with escape sequence support
  - Memory management: stack vs heap allocation
  - HolyC compatibility: `Str` type matching HolyC's approach
  - Optional safety: Bounds checking, buffer overflow protection
- **Files to create**: `src/types/string.hpp`, `src/types/string.cpp`
- **Reference**: Study existing `Union` type implementation for patterns

#### **2. Enhanced Memory Allocation** 🔥 **URGENT**
- **Status**: Basic `MAlloc` and `Free` functions exist
- **Goal**: Complete HolyC-style memory management system
- **Requirements**:
  - `MAlloc(size_t)` function matching HolyC semantics
  - `MNew<T>()` template function for type-safe allocation
  - Memory pooling and arena allocators
  - Garbage collection hooks (optional extension)
  - Memory leak detection in debug builds
- **Files to modify**: `src/types/union_type.hpp` (contains basic allocators)
- **Reference**: Existing `MAlloc`, `Free`, `MNew` functions in union_type.hpp

### **MEDIUM PRIORITY - Control Structures**

#### **1. Basic Control Flow** 🛠️ **PRIORITY**
- **Status**: Not started
- **Goal**: HolyC control structures (if, while, for, switch)
- **Requirements**:
  - Template-based control structure implementations
  - HolyC syntax compatibility
  - Optional bounds checking for loops
  - Modern extensions: range-based for, pattern matching
- **Files to create**: `src/control/if.hpp`, `src/control/while.hpp`, etc.

#### **2. Function System** ⚡ **PRIORITY**
- **Status**: Not started
- **Goal**: HolyC function declaration and calling
- **Requirements**:
  - Function type system
  - Parameter passing (value, pointer, reference)
  - Return value handling
  - Variadic function support (like HolyC's `...`)
  - Inline function optimization
- **Files to create**: `src/functions/function.hpp`

### **LOW PRIORITY - Error System Enhancements**

#### **1. Enhanced Error Recovery** 🧪
- **Status**: Basic implementation complete
- **Needs**:
  - Better error recovery in future parser
  - Configurable error suppression policies
  - Multi-file error tracking
  - Error correlation across compilation stages
- **Files**: `src/lib/error.hpp`, `src/lib/error_types.hpp`

#### **2. Performance Optimizations** ⚡
- **Status**: Functional but unoptimized
- **Needs**:
  - Thread-safe error reporting
  - Reduced memory overhead
  - Faster error formatting
  - Compile-time error checking where possible
- **Files**: `src/lib/error.cpp`, `src/lib/error_types.cpp`

## 📋 **Phase 4 & Beyond - Future Work**

### **Parser & Interpreter** (Planning Stage)
- **Lexer**: Tokenize HolyC source code
- **Parser**: Generate AST from HolyC syntax  
- **Type Checker**: Static analysis using our type system
- **Interpreter**: Execute HolyC code directly
- **C++ Transpiler**: Convert HolyC to C++

### **Compiler Backend** (Future)
- **Code Generation**: Native machine code (x86_64)
- **Optimizations**: Basic compiler optimizations
- **Linker**: Object file generation
- **Standard Library**: HolyC stdlib reimplementation

### **Modern Extensions** (Future)
- **Enhanced Type Safety**: Optional strict mode
- **Memory Safety**: Bounds checking, use-after-free detection
- **Modern Tooling**: Debugger, profiler, IDE integration
- **Cross-Platform**: Linux, Windows, macOS support

## 🛠️ How to Contribute

### 1. Development Setup

```bash
# Clone the repository
git clone https://github.com/DebadityaMalakar/holyc-plus-plus.git
cd holyc-plus-plus

# Make build script executable
chmod +x run.sh

# Build and test everything
./run.sh

# Run specific test suites
./run.sh --test-int      # Integer type tests
./run.sh --test-float    # Floating-point tests  
./run.sh --test-union    # Union type tests
./run.sh --test-error    # Error system tests

# Clean build
./run.sh --clean
```

### 2. Contribution Workflow

1. **Choose a task** from the priority list above
2. **Fork the repository** on GitHub
3. **Create a branch** for your feature/fix:
   ```bash
   git checkout -b feature/string-implementation
   ```
4. **Study existing code** for patterns:
   ```bash
   # Look at union implementation as reference for string
   cat src/types/union_type.hpp
   cat src/types/float.hpp
   
   # Look at test patterns
   cat tests/test_union.cpp
   cat tests/test_float.cpp
   ```
5. **Implement your changes** following our coding standards
6. **Write tests** (mandatory for all contributions):
   ```bash
   # Create new test file or add to existing
   # Follow patterns in test_union.cpp and test_float.cpp
   ```
7. **Run the test suite**:
   ```bash
   ./run.sh
   ```
8. **Commit your changes** with descriptive commit messages
9. **Push to your fork** and submit a Pull Request

### 3. Coding Standards

#### **For String Type Implementation:**
- Follow patterns in `float.hpp` and `union_type.hpp`
- Provide both HolyC-compatible and safe operations
- Include comprehensive bounds checking for buffer operations
- Add `Print()` method for output
- Include type traits (`is_string_v`, etc.)
- Add to global namespace (`holycpp::`)
- Consider memory management (reference counting vs copy)

#### **For Control Structures:**
- Use templates for compile-time evaluation where possible
- Match HolyC syntax semantics exactly
- Provide optional safety checks (array bounds, null checks)
- Include debugging information in debug builds
- Test all edge cases (empty loops, null conditions, etc.)

#### **General Standards:**
- **C++17**: Use modern C++ features appropriately
- **HolyC Compatibility**: When in doubt, refer to the [HolyC spec](https://holyc-lang.com/docs/language-spec/learn-variables)
- **Documentation**: Comment complex algorithms and HolyC-specific behavior
- **Testing**: Write comprehensive tests for new features
- **No Bootstrapping Code**: Main project will not include self-hosting features

### 4. Testing Requirements

**All contributions must include tests:**

#### **For String Type:**
```cpp
// tests/test_string.cpp (to create)
void test_string_basics() {
    Str s = "Hello, HolyC!";
    assert(s.length() == 13);
    assert(s[0] == 'H');
    // ... more tests
}

void test_string_concat() {
    Str a = "Hello";
    Str b = ", World!";
    Str c = a + b;
    assert(c == "Hello, World!");
    // ... concatenation tests
}
```

#### **For Control Structures:**
```cpp
// tests/test_control.cpp (to create)
void test_if_statement() {
    I32 x = 42;
    I32 result = 0;
    
    // HolyC-style if
    if (x > 0) {
        result = 1;
    } else {
        result = -1;
    }
    
    assert(result == 1);
}
```

#### **Running Tests:**
```bash
# Run all tests
./run.sh

# Run specific test
./run.sh --test test_string
```

### 5. Documentation Requirements

**All contributions must update documentation:**

1. **Update README.md** if adding major features
2. **Create type specification** (like `STRING.MD` for strings)
3. **Add API documentation** in header files using Doxygen-style comments
4. **Update examples** in `examples/` directory
5. **Document breaking changes** clearly in commit messages

**Example header documentation:**
```cpp
/**
 * @brief HolyC-style string type with optional bounds checking
 * 
 * This class implements strings as they work in HolyC, with the addition
 * of optional bounds checking and memory safety features.
 * 
 * @note In HolyC, strings are null-terminated char arrays. This class
 * maintains that compatibility while adding safety features.
 */
class Str {
    // ...
};
```

## 🔥 **High-Priority Contribution Areas**

### **Immediate Needs (Week 1-2):**
1. **String Type Implementation** - Critical for Phase 3
2. **Enhanced Memory Allocation** - Complete MAlloc/Free system
3. **Basic Control Structures** - if, while, for statements

### **Short-Term Needs (Week 3-4):**
1. **Function System** - HolyC function declaration and calling
2. **Array Support** - HolyC array syntax and operations
3. **Pointer Type Enhancements** - Complete pointer arithmetic

### **Medium-Term Needs (Month 2-3):**
1. **Lexer Implementation** - Start Phase 4 (Parser)
2. **Basic Parser** - AST generation from HolyC syntax
3. **Type Checker** - Static analysis using our type system

## 📚 Learning Resources

### **For Understanding HolyC:**
1. **[HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables)** - Primary reference
2. **[TempleOS Source](https://github.com/cia-foundation/TempleOS)** - Original implementation
3. **Our Implementation**: Study existing code for patterns

### **For Understanding Our Codebase:**
```bash
# Key files to study for Phase 3:
src/types/union_type.hpp    # Complex type implementation
src/types/float.hpp         # Math operations and special values
tests/test_union.cpp        # Comprehensive testing patterns
tests/test_float.cpp        # Floating-point testing
```

### **For String Implementation Reference:**
- Look at `std::string` but remember HolyC uses null-terminated C strings
- Consider memory management approaches (reference counting vs small string optimization)
- Study escape sequence handling in string literals
- Look at existing `MAlloc`/`Free` functions for memory management

### **For C++ Template Programming:**
- Template specialization for different string types (stack vs heap)
- `constexpr` for compile-time string operations
- SFINAE for type-safe string operations
- Move semantics for efficient string passing

## 🤝 Collaboration Guidelines

### **Working on Large Features (like Strings):**
1. **Discuss first**: Open an issue to discuss approach and design
2. **Break it down**: 
   - Step 1: Basic string storage
   - Step 2: Basic operations (concatenation, length)
   - Step 3: Advanced operations (slicing, searching)
   - Step 4: Memory management
   - Step 5: Testing and optimization
3. **Get early feedback**: Submit draft PRs for each step
4. **Coordinate**: Check if others are working on related features

### **Code Review Process:**
1. **Self-review**: Test your code thoroughly before submitting
2. **CI checks**: Ensure all tests pass
3. **Review comments**: Address all feedback promptly
4. **Merge strategy**: Squash commits for clean history

### **Communication:**
- Use GitHub Issues for bug reports and feature requests
- Use GitHub Discussions for design discussions
- Keep PR descriptions detailed with implementation notes
- Tag relevant contributors for review using @mentions
- Update documentation as part of your PR

## 🐛 Reporting Issues

When reporting bugs or suggesting features:

1. **Check existing issues** first
2. **Include version information**:
   ```bash
   ./run.sh --version
   ```
3. **Provide reproducible examples**:
   ```cpp
   // Minimal code that reproduces the issue
   #include "src/types/float.hpp"
   
   int main() {
       F64 a = 1.0;
       F64 b = 0.0;
       F64 c = a / b;  // Should throw but doesn't
       return 0;
   }
   ```
4. **Specify expected vs actual behavior**
5. **Include platform details**: OS, compiler version, architecture

## 🔄 Forking Policy

**Explicit Permission from @DebadityaMalakar:**
> "I am not bootstrapping but you may fork and make a bootstrapped version for yourself."

This means:
- Main project stays non-bootstrapped (author's choice)
- Forks may implement bootstrapping and self-hosting
- Forks may add features not in main project
- All forks must remain GPLv3
- Forks should credit the original project

**Creating a Bootstrapped Fork:**
```bash
# Create your fork on GitHub
# Add bootstrapping features in a separate branch
# Document your changes clearly
# Keep GPLv3 license intact
# Credit original project in README
```

## 📜 License: GNU GPL v3.0

### Why This License Matters

**This project uses the GNU General Public License version 3.0 (GPLv3).** This is a philosophical statement about software freedom that aligns with Terry Davis's belief in open software.

### 🆓 What GPLv3 **ALLOWS** You To Do:
- Use, study, modify, and redistribute the software
- Create bootstrapped forks (explicitly permitted!)
- Charge for distribution or support
- Patent protection for contributors
- Create derivative works with proper attribution

### 🚫 What GPLv3 **DOES NOT ALLOW**:
- Making HolyC++ proprietary or closed-source
- Removing copyleft requirements from derivatives
- Using patented algorithms without proper licensing
- Restricting users from modifying or studying the software

**By contributing, you agree to license your work under GPLv3.**

## 🙏 Acknowledgments

By contributing to HolyC++, you help build:

1. **A faithful reimplementation** of Terry Davis's HolyC language
2. **A safer, modern language** with optional safety features  
3. **An educational resource** for compiler and language design
4. **A community project** that remains free and open forever
5. **A tribute** to Terry A. Davis's vision of open computing

**Special Thanks:**
- **Terry A. Davis (1969-2018)** - For creating HolyC and TempleOS
- **@DebadityaMalakar** - Project founder and maintainer
- **All Contributors** - Helping build modern HolyC++

---

## 📝 Contributor Agreement

By submitting a contribution to HolyC++, you:

1. **Agree to license** your contribution under GPLv3
2. **Confirm you have the right** to submit the work
3. **Acknowledge** that contributions become part of the project
4. **Understand** the main project won't bootstrap (but forks may)
5. **Commit to maintaining** code quality and documentation standards

**Thank you for helping build Phase 3 of HolyC++!**

*Current Focus: Implementing String type and control structures to advance to Phase 3.*

*"The compiler is the Temple and the language is the Word." - Terry A. Davis*  
*"No bootstrapping, but fork freely." - @DebadityaMalakar*  
*"Let's build Phase 3 together!" - HolyC++ Contributors*

---

## 🚀 Getting Started Checklist for New Contributors

1. ✅ **Fork the repository** on GitHub
2. ✅ **Clone your fork** locally
3. ✅ **Run the build script** to ensure everything works
4. ✅ **Study the existing code** (especially union_type.hpp and float.hpp)
5. ✅ **Pick a task** from the priority list
6. ✅ **Create a branch** for your work
7. ✅ **Implement with tests**
8. ✅ **Submit a Pull Request**

**Need help?** Open a GitHub Discussion or ask in an existing issue!

---

*Last Updated: Phase 2 Complete, Starting Phase 3 (Strings & Control Structures)*