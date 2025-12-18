# HolyC++: A Modern Reimplementation of HolyC

![GPLv3](https://img.shields.io/badge/License-GPLv3-blue.svg)
![C++17](https://img.shields.io/badge/C++-17-red.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)

> **A modern, type-safe reimplementation of Terry A. Davis's HolyC programming language**
> Implementing the HolyC language specification with modern usability extensions.

## 📊 Current Status

### ✅ **Implemented: Integer Type System (Phase 1)**
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

**HolyC-Specific Features Implemented:**
- Zero-cost abstractions (compiles to native operations)
- `U0` placeholder for incomplete types (simple version)
- Pointer-to-integer conversions (HolyC style)
- Hex operations (`.to_hex()`, `PrintHex()`)
- Full arithmetic, bitwise, and comparison operations

**Deviation from Original HolyC:**
- Runtime bounds checking (optional safety)
- Overflow/underflow detection with exceptions (opt-in)
- Type-safe conversions (prevents accidental truncation)

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

# Clean build
./run.sh --clean
```

### Example Usage (HolyC++ Style)
```cpp
#include "src/types/unsigned_int.hpp"
#include "src/types/signed_int.hpp"
#include <iostream>

using namespace holycpp;

int main() {
    // HolyC-style variable declaration
    U64 counter = 42;
    I32 temperature = -25;
    
    // HolyC pointer-to-integer conversion
    int value = 42;
    void* ptr = &value;
    U64 ptr_as_int = ptr;  // Direct conversion like HolyC
    
    // HolyC-style operations
    counter.Print();      // Prints decimal value
    counter.PrintHex();   // Prints hex value (0x2A)
    
    // HolyC-style array (basic emulation)
    U8 buffer[256];
    buffer[0] = 'H';
    buffer[1] = 'i';
    
    // Safety-enhanced operations (HolyC++ extension)
    U8 max_val = U8::MAX;
    try {
        U8 result = max_val.checked_add(U8(1));  // Throws on overflow
    } catch (const std::overflow_error& e) {
        Print("Safety: %s", e.what());  // HolyC++ style error handling
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
- [ ] **Enhanced Type Safety**: Optional strict mode
- [ ] **Memory Safety**: Bounds checking, use-after-free detection
- [ ] **Modern Tooling**: Debugger, profiler, IDE integration
- [ ] **Cross-Platform**: Linux, Windows, macOS support
- [ ] **Package Manager**: Library distribution system

### Phase 6: HolyC++ Language 📋
- [ ] **Language Specification**: HolyC++ official spec
- [ ] **Advanced Features**: Generics, modules, concepts
- [ ] **Ecosystem**: Libraries, frameworks, tools
- [ ] **Documentation**: Tutorials, guides, reference

## 🎯 Project Philosophy

**Based on HolyC, Enhanced for Modern Use:**

1. **HolyC Compatibility First**: Faithful to Terry Davis's original specification
2. **No Bootstrapping**: This implementation will never be self-hosted in HolyC++ (philosophical choice)
3. **Modern Safety**: Add optional safety features without breaking HolyC compatibility
4. **Educational**: Clean implementation for studying language and compiler design
5. **Practical**: Usable for real projects while honoring the original

**Reference Documentation:**
- [HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables)
- [TempleOS Documentation](https://templeos.org/)
- [Original HolyC Source](https://github.com/cia-foundation/TempleOS)

## 📁 Project Structure
```
holyc-plus-plus/
├── src/
│   ├── types/           # Core type system (COMPLETE)
│   ├── parser/          # HolyC parser (IN PROGRESS)
│   ├── ast/             # Abstract Syntax Tree
│   ├── compiler/        # Compiler backend
│   └── runtime/         # Runtime library
├── tests/               # Test suite
├── examples/            # Example HolyC programs
├── docs/                # Documentation
├── spec/                # HolyC++ language specification
├── run.sh              # Build script
└── LICENSE             # GNU GPL v3.0
```

## 🤝 Contributing

Please refer to [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute to this project. We welcome contributions of all kinds!

**Areas Needing Help:**
- Implementing F64 and improved U0 types
- Building the HolyC parser
- Writing HolyC-to-C++ transpiler
- Testing and documentation

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

---

*Current Progress: Phase 1 Complete. Working on Phase 2 (Full HolyC Language Support).*

*"The compiler is the Temple and the language is the Word." - Terry A. Davis*