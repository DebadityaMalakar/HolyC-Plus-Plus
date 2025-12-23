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

### ✅ **Phase 2: Floating Point Types (COMPLETE)**
Full floating-point support following HolyC's approach:

**✅ Implemented:**
- `F32` - 32-bit floating point (IEEE-754 single precision)
- `F64` - 64-bit floating point (IEEE-754 double precision, same as HolyC's "double")

**Features:**
- **Arithmetic Operations**: +, -, *, /, % (with bounds checking)
- **Math Functions**: sqrt(), abs(), pow(), sin(), cos(), tan()
- **Rounding**: floor(), ceil(), round()
- **Special Values**: NaN, Infinity detection with is_nan(), is_inf()
- **Type Conversions**: Seamless conversion from/to integer types
- **Comparisons**: Full relational operators with proper floating-point semantics
- **Error Handling**: Division by zero, domain errors
- **HolyC Compatibility**: Direct F64 support matching HolyC's double type

**📋 See `tests/test_float.cpp` for complete floating-point tests**

### ✅ **Phase 3: Union Type System (COMPLETE)**
Full union support with HolyC compatibility:

**✅ Implemented:**
- `U0` - Void type for union compatibility
- `Union<Types...>` - Type-safe union wrapper
- `Value` class - Example from HolyC documentation
- Plain C unions - Direct HolyC compatibility

**Features:**
- **Type-Safe Access**: get<T>() with runtime type checking
- **Unsafe Access**: as<T>() HolyC-style unsafe casting
- **Active Type Tracking**: Knows which type is currently active
- **Memory Management**: Proper construction/destruction of union members
- **C Compatibility**: Exact memory layout matching HolyC
- **Move/Copy Semantics**: Full support for modern C++
- **Union Macros**: `UNION_BEGIN()`, `UNION_END()`, `UNION()` macros

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

**📋 See `tests/test_union.cpp` for complete union tests**

### 🚧 **Phase 4: String & Memory Management (IN PROGRESS)**
- [ ] **String Types**: HolyC-style string handling
- [ ] **Memory Allocation**: MAlloc, Free wrappers
- [ ] **Basic String Operations**: Concatenation, comparison, formatting
- [ ] **String Literal Support**: HolyC string syntax

### 📋 **Future Phases:**
- [ ] **Control Structures**: if, while, for (HolyC syntax)
- [ ] **Functions**: HolyC function declaration and calling
- [ ] **Arrays**: HolyC array syntax and semantics
- [ ] **Pointers**: Full pointer type system
- [ ] **Parser & Interpreter**: Lexer, Parser, Type Checker
- [ ] **Compiler Backend**: Code generation, optimizations
- [ ] **Modern Extensions**: Enhanced error recovery, memory safety

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

# Test specific components
./run.sh --test-int      # Integer type tests
./run.sh --test-float    # Floating-point tests  
./run.sh --test-union    # Union type tests
./run.sh --test-error    # Error system tests

# Clean build
./run.sh --clean
```

### Example Usage (Union Types)
```cpp
#include "src/types/union_type.hpp"
#include <iostream>

using namespace holycpp;

int main() {
    // HolyC-style union
    Example e;  // Union<I32, U8>
    
    // Type-safe access
    e.set(I32(42));
    std::cout << "Age: " << e.get<I32>() << std::endl;
    
    e.set(U8('A'));
    std::cout << "Char: '" << e.get<U8>() << "'" << std::endl;
    
    // HolyC-style unsafe access
    e.set(I32(0x41424344));  // "ABCD" in ASCII
    std::cout << "First char: '" << e.as<U8>() << "'" << std::endl;
    
    // Value class example from HolyC docs
    Value* v = MAllocValue();
    v->set_float(3.14159);
    v->Print();
    
    Free(v);
    return 0;
}
```

### Example Usage (Floating Point)
```cpp
#include "src/types/float.hpp"
#include <iostream>

using namespace holycpp;

int main() {
    // HolyC-style floating point
    F64 pi = 3.14159265359;  // Same as double in HolyC
    F32 half = 0.5f;         // Single precision (HolyC++)
    
    // Arithmetic operations
    F64 radius = 5.0;
    F64 area = pi * radius * radius;
    std::cout << "Area: " << area << std::endl;
    
    // Math functions
    F64 x = 16.0;
    F64 sqrt_x = x.sqrt();
    std::cout << "sqrt(" << x << ") = " << sqrt_x << std::endl;
    
    // Trigonometric functions
    F64 angle = 0.0;
    std::cout << "sin(0) = " << angle.sin() << std::endl;
    std::cout << "cos(0) = " << angle.cos() << std::endl;
    
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

### Phase 2: Floating Point Types ✅ **COMPLETE**
- [x] **F64**: 64-bit floating point (double in HolyC)
- [x] **F32**: 32-bit floating point (HolyC++ extension)
- [x] **Math Functions**: sqrt, pow, sin, cos, etc.
- [x] **Special Values**: NaN, Infinity handling
- [x] **Rounding Functions**: floor, ceil, round
- [x] **Type Conversions**: Seamless integer ↔ float conversion
- [x] **Testing**: Complete floating-point test suite

### Phase 3: Union Type System ✅ **COMPLETE**
- [x] **U0 Type**: Void type for union compatibility
- [x] **Type-Safe Union**: Union<Types...> wrapper
- [x] **HolyC Value Class**: Exact implementation from documentation
- [x] **Plain C Unions**: Direct HolyC compatibility
- [x] **Memory Layout**: Exact match with HolyC memory layout
- [x] **Unsafe Access**: as<T>() HolyC-style casting
- [x] **Testing**: Comprehensive union test suite

### Phase 4: String & Memory Management 🔄 **CURRENT**
- [ ] **String Types**: HolyC-style string handling
- [ ] **Memory Allocation**: MAlloc, Free wrappers
- [ ] **Basic String Operations**: Concatenation, comparison, formatting
- [ ] **String Literal Support**: HolyC string syntax

### Phase 5: Control Structures 📋 **NEXT**
- [ ] **if/else**: HolyC conditional statements
- [ ] **while/for**: HolyC loop constructs
- [ ] **switch**: HolyC switch statements
- [ ] **goto**: HolyC goto support

### Phase 6: Functions & Pointers 📋
- [ ] **Function Declaration**: HolyC function syntax
- [ ] **Pointer Types**: Full pointer type system
- [ ] **Arrays**: HolyC array syntax and semantics
- [ ] **Structs**: HolyC structure support

### Phase 7: Parser & Interpreter 📋
- [ ] **Lexer**: Tokenize HolyC source code
- [ ] **Parser**: Generate AST from HolyC syntax
- [ ] **Type Checker**: Static analysis
- [ ] **Interpreter**: Execute HolyC code directly
- [ ] **C++ Transpiler**: Convert HolyC to C++

### Phase 8: Compiler Backend 📋
- [ ] **Code Generation**: Native machine code (x86_64)
- [ ] **Optimizations**: Basic compiler optimizations
- [ ] **Linker**: Object file generation
- [ ] **Standard Library**: HolyC stdlib reimplementation

### Phase 9: Modern Extensions 📋
- [ ] **Enhanced Error Recovery**: More sophisticated error handling
- [ ] **Memory Safety**: Bounds checking, use-after-free detection
- [ ] **Modern Tooling**: Debugger, profiler, IDE integration
- [ ] **Cross-Platform**: Linux, Windows, macOS support
- [ ] **Package Manager**: Library distribution system

### Phase 10: HolyC++ Language 📋
- [ ] **Language Specification**: HolyC++ official spec
- [ ] **Advanced Features**: Generics, modules, concepts
- [ ] **Ecosystem**: Libraries, frameworks, tools
- [ ] **Documentation**: Tutorials, guides, reference

## 📁 Project Structure
```
holyc-plus-plus/
├── src/
│   ├── types/           # Core type system (COMPLETE)
│   │   ├── unsigned_int.hpp
│   │   ├── signed_int.hpp
│   │   ├── float.hpp
│   │   ├── union_type.hpp
│   │   ├── unsigned_int.cpp
│   │   ├── signed_int.cpp
│   │   ├── float.cpp
│   │   └── union_type.cpp
│   ├── lib/             # Error system and utilities
│   │   ├── error.hpp
│   │   ├── error.cpp
│   │   ├── error_types.hpp
│   │   └── error_types.cpp
│   ├── parser/          # HolyC parser (IN PROGRESS)
│   ├── ast/             # Abstract Syntax Tree
│   ├── compiler/        # Compiler backend
│   └── runtime/         # Runtime library
├── tests/               # Comprehensive test suite
│   ├── test_int.cpp    # Integer type tests
│   ├── test_float.cpp  # Floating-point tests
│   ├── test_union.cpp  # Union type tests
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

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Current Priority Areas:
1. **String Type Implementation**: HolyC-style string handling
2. **Memory Management**: MAlloc/Free improvements
3. **Error System Enhancements**: Better recovery and performance
4. **Control Structures**: if, while, for syntax implementation
5. **Testing**: More comprehensive test coverage

### Getting Started:
```bash
# Fork and clone the repository
git clone https://github.com/your-username/holyc-plus-plus.git
cd holyc-plus-plus

# Create a feature branch
git checkout -b feature/your-feature

# Build and test
./run.sh --test-all

# Make changes and test
# Submit a pull request
```