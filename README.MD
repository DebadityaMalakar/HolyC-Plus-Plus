# HolyC++: A Modern Reimplementation of HolyC

![GPLv3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![C++17](https://img.shields.io/badge/C++-17-red.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)

> **A modern, type-safe reimplementation of Terry A. Davis's HolyC programming language**  
> Implementing the HolyC language specification with modern usability extensions.

## 📊 Current Status

### ✅ **Phase 1: Core Type System (COMPLETE)**
Following the [HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables), the core integer type system is fully implemented:

**Basic Types (as per spec):**
- `U8` - 8-bit unsigned integer (0 to 255)
- `U16` - 16-bit unsigned integer (0 to 65,535)
- `U32` - 32-bit unsigned integer (0 to 4,294,967,295)
- `U64` - 64-bit unsigned integer (0 to 18,446,744,073,709,551,615)
- `I8` - 8-bit signed integer (-128 to 127)
- `I16` - 16-bit signed integer (-32,768 to 32,767)
- `I32` - 32-bit signed integer (-2,147,483,648 to 2,147,483,647)
- `I64` - 64-bit signed integer (-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807)

### ✅ **Basic Error System (FOUNDATIONAL)**
A foundational error system has been implemented and needs further development:

**✅ Implemented:**
- **Core Error Classes**: `CompilerError`, `ContextualError`, `InternalError`
- **Error Manager**: Singleton for error collection, reporting, and statistics
- **Source Location Tracking**: File, line, column position tracking
- **Error Builder Pattern**: Fluent interface for constructing complex errors
- **Error Severity Levels**: NOTE, WARNING, ERROR, FATAL
- **Basic Error Categories**: Lexer, Parser, Type, Semantic, CodeGen errors
- **Error Code Registry**: Predefined error codes with categories
- **Comprehensive Testing**: Full test suite covering all error features

**🔧 Needs Improvement:**
- **Error Recovery Mechanisms**: More sophisticated error recovery strategies
- **Multithreading Support**: Thread-safe error reporting
- **Internationalization**: Error message localization support
- **Performance Optimization**: Reduced overhead in production builds
- **Enhanced Context Management**: Better contextual error tracking

**📋 See `tests/test_error.cpp` for complete error system tests**

### 🚧 **Phase 2: HolyC Language Features (IN PROGRESS)**
- [ ] **F64**: 64-bit floating point (double in HolyC)
- [ ] **U0 Improvements**: Full union type implementation
- [ ] **String Literals**: HolyC-style string handling
- [ ] **Control Structures**: if, while, for (HolyC syntax)
- [ ] **Functions**: HolyC function declaration and calling
- [ ] **Arrays**: HolyC array syntax and semantics
- [ ] **Pointers**: Full pointer type system

## 🚀 Quick Start

### Prerequisites
- C++17 compatible compiler (GCC 8+, Clang 7+, MSVC 2019+)

### Building & Testing
```bash
# Clone the repository
git clone https://github.com/DebadityaMalakar/holyc-plus-plus.git
cd holyc-plus-plus

# Make build script executable
chmod +x run.sh

# Build and run all tests
./run.sh

# Run error system tests specifically
./run.sh --test-error

# Clean build
./run.sh --clean
```

### Example Usage (Error System)
```cpp
#include "src/lib/error.hpp"
#include "src/lib/error_types.hpp"
#include <iostream>

using namespace holycpp;

int main() {
    // Quick error reporting
    ErrorManager::get().error("Syntax error at line 42");
    ErrorManager::get().warning("Deprecated function used");
    
    // Error with source location
    SourceLocation loc("program.hc", 42, 15);
    ErrorManager::get().error("Type mismatch", loc);
    
    // Using error builder for complex errors
    auto error = (ErrorManager::get().buildError()
        .at("file.hc", 30, 5)
        .code("T001")
        << "Cannot convert 'float' to 'int'")
        .inContext("In function 'calculate'")
        .inContext("While processing argument 2")
        .build();
    
    // Report the built error
    ErrorManager::get().report(std::move(error));
    
    // Check for errors
    if (ErrorManager::get().hasErrors()) {
        std::cerr << "Compilation failed with " 
                  << ErrorManager::get().getErrorCount() 
                  << " errors\n";
        return 1;
    }
    
    return 0;
}
```

## 🗺️ ROADMAP

### Phase 1: Core Type System ✅ **COMPLETE**
- [x] **Integer Types**: Full HolyC integer support (U8-U64, I8-I64)
- [x] **Basic Operations**: Arithmetic, bitwise, comparisons per HolyC spec
- [x] **Pointer Integration**: HolyC-style pointer arithmetic
- [x] **Hex Operations**: `.to_hex()` and `PrintHex()` methods
- [x] **Error System**: Foundational error handling framework
- [x] **Testing**: Comprehensive test suite

### Phase 2: HolyC Language Features 🔄 **CURRENT**
- [ ] **F64**: 64-bit floating point (double in HolyC)
- [ ] **U0 Improvements**: Full union type implementation
- [ ] **String Literals**: HolyC-style string handling
- [ ] **Control Structures**: if, while, for (HolyC syntax)
- [ ] **Functions**: HolyC function declaration and calling
- [ ] **Arrays**: HolyC array syntax and semantics
- [ ] **Pointers**: Full pointer type system

### Phase 3: Parser & Interpreter 📋 **NEXT**
- [ ] **Lexer**: Tokenize HolyC source code
- [ ] **Parser**: Generate AST from HolyC syntax
- [ ] **Type Checker**: Static analysis
- [ ] **Interpreter**: Execute HolyC code directly
- [ ] **C++ Transpiler**: Convert HolyC to C++

### Phase 4: Compiler Backend 📋
- [ ] **Code Generation**: Native machine code (x86_64)
- [ ] **Optimizations**: Basic compiler optimizations
- [ ] **Linker**: Object file generation
- [ ] **Standard Library**: HolyC stdlib reimplementation

### Phase 5: Modern Extensions 📋
- [ ] **Enhanced Error Recovery**: More sophisticated error handling
- [ ] **Memory Safety**: Bounds checking, use-after-free detection
- [ ] **Modern Tooling**: Debugger, profiler, IDE integration
- [ ] **Cross-Platform**: Linux, Windows, macOS support
- [ ] **Package Manager**: Library distribution system

### Phase 6: HolyC++ Language 📋
- [ ] **Language Specification**: HolyC++ official spec
- [ ] **Advanced Features**: Generics, modules, concepts
- [ ] **Ecosystem**: Libraries, frameworks, tools
- [ ] **Documentation**: Tutorials, guides, reference

## 🤝 Contributing to the Error System

The error system is **functional but needs improvement**. Here's how you can help:

### High Priority Error System Improvements:
1. **Enhanced Error Recovery**: 
   - Implement better error recovery in parser
   - Add configurable error suppression policies
   - Develop multi-file error tracking

2. **Performance Optimizations**:
   - Thread-safe error reporting
   - Reduced memory overhead
   - Faster error formatting

3. **Advanced Features**:
   - Error message internationalization
   - Plug-in error handlers
   - Error correlation across compilation stages
   - Automated error suggestion system

4. **Testing & Documentation**:
   - More comprehensive error scenarios
   - Performance benchmarks
   - API documentation examples

### Getting Started with Error System Development:
```bash
# Study the current implementation:
ls -la src/lib/error*          # Error system headers
ls -la tests/test_error.cpp    # Test suite

# Run error tests:
./run.sh --test-error

# Add new error types:
# 1. Add to error_types.hpp
# 2. Implement in error_types.cpp  
# 3. Add tests in test_error.cpp
```

### Error System Architecture:
```
src/lib/
├── error.hpp              # Core error classes
├── error.cpp              # Singleton definitions
├── error_types.hpp        # Specialized error types
└── error_types.cpp        # Error type implementations
```

**See `CONTRIBUTING.md` for full contribution guidelines.**

## 📁 Project Structure
```
holyc-plus-plus/
├── src/
│   ├── types/           # Core type system (COMPLETE)
│   ├── lib/             # Error system and utilities
│   │   ├── error.hpp
│   │   ├── error.cpp
│   │   ├── error_types.hpp
│   │   └── error_types.cpp
│   ├── parser/          # HolyC parser (IN PROGRESS)
│   ├── ast/             # Abstract Syntax Tree
│   ├── compiler/        # Compiler backend
│   └── runtime/         # Runtime library
├── tests/               # Test suite
│   ├── test_int.cpp    # Integer type tests
│   └── test_error.cpp  # Error system tests
├── examples/            # Example HolyC programs
├── docs/                # Documentation
├── spec/                # HolyC++ language specification
├── run.sh              # Build script
└── LICENSE             # GNU GPL v3.0
```

## 📜 License

This project is licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for details.

This ensures:
- All derivatives must remain open source
- Terry Davis's work remains freely accessible
- Community collaboration is encouraged
- Prevents proprietary appropriation

## 🙏 Acknowledgments

- **Terry A. Davis** (1969-2018): For creating HolyC and TempleOS
- **HolyC Language Specification**: Reference documentation
- **TempleOS Community**: For preserving Terry's work
- **Contributors**: Everyone who helps build HolyC++
