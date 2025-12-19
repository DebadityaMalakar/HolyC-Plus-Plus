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

## 🚧 **URGENTLY NEEDED: Phase 2 Implementation**

### **HIGH PRIORITY - Core Language Features**
We need help implementing the full HolyC language beyond basic integers:

#### **1. Floating Point Type: F64** 🔥 **URGENT**
- **Status**: Not started
- **Goal**: Implement IEEE 754 64-bit floating point (HolyC's `F64`)
- **Requirements**:
  - Basic arithmetic operations
  - Comparison operators
  - Conversion to/from integers
  - HolyC compatibility (no `NaN`/`Inf` unless in spec)
  - Optional overflow/underflow detection
- **Files to create**: `src/types/float64.hpp`, `src/types/float64.cpp`

#### **2. U0 Union Type Enhancement** 🔥 **URGENT**
- **Status**: Basic placeholder exists
- **Goal**: Full union type with type tagging and safe access
- **Requirements**:
  - Type-safe variant type
  - Pattern matching support
  - Memory layout matching HolyC
  - Safe casting operations
- **Files to modify**: `src/types/u0.hpp`, `src/types/u0.cpp`

#### **3. String Literal Support** 🔥 **URGENT**
- **Status**: Not started
- **Goal**: HolyC-style string handling
- **Requirements**:
  - Null-terminated string type
  - String operations (concatenation, slicing)
  - String literals in parser
  - Memory management (stack vs heap)
- **Files to create**: `src/types/string.hpp`, `src/types/string.cpp`

### **MEDIUM PRIORITY - Error System Improvements**

#### **1. Enhanced Error Recovery** 🛠️
- **Status**: Basic implementation
- **Needs**:
  - Better error recovery in parser
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

#### **3. Advanced Error Features** 🧪
- **Status**: Basic features only
- **Needs**:
  - Error message internationalization
  - Plug-in error handlers
  - Automated error suggestion system
  - Error severity escalation rules
- **Files**: New files in `src/lib/`

## 📋 **Phase 3 & Beyond - Future Work**

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
./run.sh --test-error    # Error system tests
```

### 2. Contribution Workflow

1. **Choose a task** from the priority list above
2. **Fork the repository** on GitHub
3. **Create a branch** for your feature/fix:
   ```bash
   git checkout -b feature/f64-implementation
   ```
4. **Study existing code** for patterns:
   ```bash
   # Look at integer implementation as reference
   cat src/types/unsigned_int.hpp
   cat src/types/signed_int.hpp
   
   # Look at error system
   cat src/lib/error.hpp
   cat tests/test_error.cpp
   ```
5. **Implement your changes** following our coding standards
6. **Write tests** (mandatory for all contributions):
   ```bash
   # Add tests to existing files or create new test files
   # Follow patterns in test_int.cpp and test_error.cpp
   ```
7. **Run the test suite**:
   ```bash
   ./run.sh
   ```
8. **Commit your changes** with descriptive commit messages
9. **Push to your fork** and submit a Pull Request

### 3. Coding Standards

#### **For Type Implementations (like F64, U0):**
- Follow patterns in `unsigned_int.hpp` and `signed_int.hpp`
- Provide both HolyC-compatible and safe operations
- Include comprehensive bounds checking
- Add `Print()` and `PrintHex()` methods
- Include type traits (`is_float64_v`, etc.)
- Add to global namespace (`holycpp::`)

#### **For Error System Improvements:**
- Maintain backward compatibility
- Keep performance in mind (avoid virtual calls where possible)
- Use the builder pattern for complex errors
- Add new error codes to registry with documentation
- Test all error conditions

#### **General Standards:**
- **C++17**: Use modern C++ features appropriately
- **HolyC Compatibility**: When in doubt, refer to the [HolyC spec](https://holyc-lang.com/docs/language-spec/learn-variables)
- **Documentation**: Comment complex algorithms and HolyC-specific behavior
- **Testing**: Write comprehensive tests for new features
- **No Bootstrapping Code**: Main project will not include self-hosting features

### 4. Testing Requirements

**All contributions must include tests:**

#### **For New Types (like F64):**
```cpp
// tests/test_float.cpp
void test_f64_basics() {
    F64 a = 3.14;
    F64 b = 2.71;
    assert(a + b == 5.85);
    // ... more tests
}

void test_f64_conversions() {
    F64 f = 42.0;
    I32 i = f;  // Should work
    // ... conversion tests
}
```

#### **For Error System Improvements:**
```cpp
// tests/test_error.cpp additions
void test_new_error_feature() {
    // Test the new feature
    // Verify it works correctly
    // Test edge cases
}
```

#### **Running Tests:**
```bash
# Run all tests
./run.sh

# Run specific test
./run.sh --test test_float
```

### 5. Documentation Requirements

**All contributions must update documentation:**

1. **Update README.md** if adding major features
2. **Create/Update type specification** (like `FLOAT.MD` for F64)
3. **Add API documentation** in header files
4. **Update examples** if applicable
5. **Document breaking changes** clearly

## 🔥 **High-Priority Contribution Areas**

### **Immediate Needs (Week 1-2):**
1. **F64 Implementation** - Critical for Phase 2
2. **Enhanced U0 Type** - Needed for union support
3. **Basic String Type** - Required for string literals

### **Short-Term Needs (Week 3-4):**
1. **Error Recovery Improvements** - Better compiler UX
2. **Thread-Safe Error Reporting** - For parallel compilation
3. **Performance Optimizations** - Reduce overhead

### **Medium-Term Needs (Month 2-3):**
1. **Lexer Implementation** - Start Phase 3
2. **Basic Parser** - AST generation
3. **Control Structures** - if, while, for

## 📚 Learning Resources

### **For Understanding HolyC:**
1. **[HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables)** - Primary reference
2. **[TempleOS Source](https://github.com/cia-foundation/TempleOS)** - Original implementation
3. **[INT.MD](INT.MD)** - Our integer type specification

### **For Understanding Our Codebase:**
```bash
# Key files to study:
src/types/unsigned_int.hpp    # Template pattern for types
src/types/signed_int.hpp      # Signed type implementation  
src/lib/error.hpp             # Error system architecture
tests/test_int.cpp            # Testing patterns
tests/test_error.cpp          # Error testing patterns
```

### **For C++ Template Programming:**
- `template<size_t Bits>` pattern in our integer types
- SFINAE for type constraints (`std::enable_if_t`)
- `constexpr` for compile-time computations
- Type traits for compile-time introspection

## 🤝 Collaboration Guidelines

### **Working on Large Features (like F64):**
1. **Discuss first**: Open an issue to discuss approach
2. **Break it down**: Implement in small, testable pieces
3. **Get early feedback**: Submit draft PRs for review
4. **Coordinate**: Check if others are working on related features

### **Code Review Process:**
1. **Self-review**: Test your code thoroughly before submitting
2. **CI checks**: Ensure all tests pass
3. **Review comments**: Address all feedback promptly
4. **Merge strategy**: Squash commits for clean history

### **Communication:**
- Use GitHub Issues for bug reports
- Use GitHub Discussions for questions
- Keep PR descriptions detailed
- Tag relevant contributors for review

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
   #include "src/types/unsigned_int.hpp"
   int main() {
       U8 val = 300;  // Should throw but doesn't
       return 0;
   }
   ```
4. **Specify expected vs actual behavior**
5. **Include platform details**: OS, compiler, architecture

## 🔄 Forking Policy

**Explicit Permission from @DebadityaMalakar:**
> "I am not bootstrapping but you may fork and make a bootstrapped version for yourself."

This means:
- Main project stays non-bootstrapped (author's choice)
- Forks may implement bootstrapping
- Forks may add features not in main project
- All forks must remain GPLv3

**Creating a Bootstrapped Fork:**
```bash
# Create your fork on GitHub
# Add bootstrapping features in a separate branch
# Keep GPLv3 license
# Credit original project
```

## 📜 License: GNU GPL v3.0

### Why This License Matters

**This project uses the GNU General Public License version 3.0 (GPLv3).** This is a philosophical statement about software freedom that aligns with Terry Davis's belief in open software.

### 🆓 What GPLv3 **ALLOWS** You To Do:
- Use, study, modify, and redistribute the software
- Create bootstrapped forks (explicitly permitted!)
- Charge for distribution or support
- Patent protection for contributors

### 🚫 What GPLv3 **DOES NOT ALLOW**:
- Making HolyC++ proprietary
- Removing copyleft requirements from derivatives
- Restricting users from modifying software

**By contributing, you agree to license your work under GPLv3.**

## 🙏 Acknowledgments

By contributing to HolyC++, you help build:

1. **A faithful reimplementation** of Terry Davis's HolyC
2. **A safer, modern language** with optional safety features  
3. **An educational resource** for compiler and language design
4. **A community project** that remains free and open forever

**Special Thanks:**
- **Terry A. Davis** - For creating HolyC
- **@DebadityaMalakar** - Project founder and maintainer
- **All Contributors** - Helping build modern HolyC

---

## 📝 Contributor Agreement

By submitting a contribution to HolyC++, you:

1. **Agree to license** your contribution under GPLv3
2. **Confirm you have the right** to submit the work
3. **Acknowledge** that contributions become part of the project
4. **Understand** main project won't bootstrap (but forks may)

**Thank you for helping build Phase 2 of HolyC++!**

*Current Focus: Implementing F64, U0, and string support to complete basic HolyC language features.*

*"The compiler is the Temple and the language is the Word." - Terry A. Davis*  
*"No bootstrapping, but fork freely." - @DebadityaMalakar*  
*"Let's build Phase 2 together!" - HolyC++ Contributors*
```

This updated CONTRIBUTING.md:

1. **Clearly summarizes** what's already implemented (Phase 1 complete)
2. **Highlights urgent needs** for Phase 2 (F64, U0, strings)
3. **Provides specific guidance** on what to work on
4. **Includes concrete examples** of code patterns to follow
5. **Structures tasks by priority** (urgent, short-term, medium-term)
6. **Maintains the project philosophy** while encouraging contributions
7. **References existing code** for learning and patterns
8. **Sets clear expectations** for testing and documentation

The document is now action-oriented, helping new contributors understand exactly what needs to be done and how to do it.