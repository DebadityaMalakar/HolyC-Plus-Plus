# HolyC++ Integer Types Specification

## 📋 Overview

This document specifies the integer type system implementation in HolyC++, which follows the HolyC language specification while adding modern safety features.

**Reference:** [HolyC Language Specification - Variables](https://holyc-lang.com/docs/language-spec/learn-variables)

## 🎯 Core Integer Types

### Unsigned Integers (Implemented ✅)

| Type | Size | Range | HolyC Equivalent | Implementation |
|------|------|-------|-----------------|----------------|
| `U8` | 8 bits | 0 to 255 | `U8` | `UInt<8>` |
| `U16` | 16 bits | 0 to 65,535 | `U16` | `UInt<16>` |
| `U32` | 32 bits | 0 to 4,294,967,295 | `U32` | `UInt<32>` |
| `U64` | 64 bits | 0 to 18,446,744,073,709,551,615 | `U64` | `UInt<64>` |

### Signed Integers (Implemented ✅)

| Type | Size | Range | HolyC Equivalent | Implementation |
|------|------|-------|-----------------|----------------|
| `I8` | 8 bits | -128 to 127 | `I8` | `SInt<8>` |
| `I16` | 16 bits | -32,768 to 32,767 | `I16` | `SInt<16>` |
| `I32` | 32 bits | -2,147,483,648 to 2,147,483,647 | `I32` | `SInt<32>` |
| `I64` | 64 bits | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 | `I64` | `SInt<64>` |

## 📐 Type Representation

### Storage Types (Internal)
```cpp
// UInt<Bits>::storage_type mapping
Bits == 8  -> uint8_t
Bits == 16 -> uint16_t
Bits == 32 -> uint32_t
Bits == 64 -> uint64_t

// SInt<Bits>::storage_type mapping
Bits == 8  -> int8_t
Bits == 16 -> int16_t
Bits == 32 -> int32_t
Bits == 64 -> int64_t
```

### Compile-time Constants
Each type provides static constants:
```cpp
U8::MIN   = 0          U8::MAX   = 255
U16::MIN  = 0          U16::MAX  = 65535
U32::MIN  = 0          U32::MAX  = 4294967295U
U64::MIN  = 0          U64::MAX  = 18446744073709551615ULL

I8::MIN   = -128       I8::MAX   = 127
I16::MIN  = -32768     I16::MAX  = 32767
I32::MIN  = -2147483648 I32::MAX = 2147483647
I64::MIN  = -9223372036854775808LL I64::MAX = 9223372036854775807LL
```

## 🔄 HolyC Compatibility Features

### 1. **Pointer Conversions** (HolyC Style)
```cpp
// HolyC allows direct pointer-to-integer conversion
void* ptr = /* ... */;
U64 int_val = ptr;  // Direct assignment

// Implementation matches HolyC behavior
UInt(void* ptr) : value(static_cast<storage_type>(
    reinterpret_cast<uintptr_t>(ptr))) {}
```

### 2. **Hex Operations** (HolyC Style)
```cpp
U32 val = 0xDEADBEEF;
const char* hex_str = val.to_hex();  // Returns "0xDEADBEEF"
val.PrintHex();                      // Prints hex to stdout
```

### 3. **Print Methods** (HolyC Style)
```cpp
U32 val = 42;
val.Print();    // Prints "42" to stdout (like HolyC's Print("%d", val))
```

### 4. **Type Aliases** (HolyC Compatibility)
```cpp
// Exact HolyC type names preserved
using U8 = UInt<8>;
using U16 = UInt<16>;
using U32 = UInt<32>;
using U64 = UInt<64>;
using I8 = SInt<8>;
using I16 = SInt<16>;
using I32 = SInt<32>;
using I64 = SInt<64>;
```

## 🛡️ Modern Safety Extensions (New in HolyC++)

### 1. **Bounds Checking on Construction**
**Problem in HolyC:** Silent overflow/underflow
```c
// HolyC (dangerous):
U8 val = 300;  // Silently becomes 44 (300 % 256)
```

**Solution in HolyC++:** Runtime bounds checking
```cpp
// HolyC++ (safe):
try {
    U8 val = 300;  // Throws std::out_of_range
} catch (const std::out_of_range& e) {
    // Handle error
}
```

**Implementation:**
```cpp
template<typename T>
UInt(T val) {
    check_bounds(val);  // Throws if out of range
    value = static_cast<storage_type>(val);
}
```

### 2. **Checked Arithmetic Operations**
**Problem in HolyC:** Silent overflow
```c
// HolyC:
U8 a = 200;
U8 b = 100;
U8 c = a + b;  // 44 (silent overflow)
```

**Solution in HolyC++:** Opt-in safe operations
```cpp
// HolyC++:
U8 a = 200;
U8 b = 100;

// Unsafe (HolyC-compatible, for performance)
U8 c = a + b;  // 44 (wraps like HolyC)

// Safe (new feature)
try {
    U8 c_safe = a.checked_add(b);  // Throws std::overflow_error
} catch (const std::overflow_error& e) {
    // Handle overflow
}
```

**Available Checked Methods:**
- `checked_add()` - Safe addition with overflow check
- `checked_sub()` - Safe subtraction with underflow check
- `checked_mul()` - Safe multiplication with overflow check

### 3. **Type-Safe Conversions**
**Problem in HolyC:** Dangerous implicit conversions
```c
// HolyC allows dangerous conversions:
U8 small = 255;
I32 large = small;  // OK, but sign extension issues possible
U32 from_signed = -1;  // Dangerous: -1 becomes 4294967295
```

**Solution in HolyC++:** Safe, explicit conversions
```cpp
// HolyC++ enforces safety:
U8 small = 255;
I32 large = small;  // OK: positive values convert safely

I32 negative = -1;
try {
    U32 from_signed = negative;  // Throws: negative to unsigned
} catch (const std::out_of_range& e) {
    // Prevents dangerous conversion
}
```

### 4. **Shift Safety**
**Problem in HolyC:** Undefined behavior on large shifts
```c
// HolyC:
U32 val = 1;
U32 result = val << 33;  // Undefined behavior
```

**Solution in HolyC++:** Bounded shifts
```cpp
// HolyC++:
U32 val = 1;
try {
    U32 result = val << 33;  // Throws std::out_of_range
} catch (const std::out_of_range& e) {
    // Prevents undefined behavior
}
```

### 5. **Division Safety**
**Problem in HolyC:** Crashes on division by zero
```c
// HolyC:
U32 a = 100;
U32 b = 0;
U32 result = a / b;  // Crashes
```

**Solution in HolyC++:** Exception on division by zero
```cpp
// HolyC++:
U32 a = 100;
U32 b = 0;
try {
    U32 result = a / b;  // Throws std::domain_error
} catch (const std::domain_error& e) {
    // Graceful error handling
}
```

## ⚡ Performance Considerations

### Zero-Cost When Safe
```cpp
// These compile to native operations (no overhead):
U32 a = 10;
U32 b = 20;
U32 c = a + b;      // Single ADD instruction
U32 d = a & b;      // Single AND instruction
bool e = a < b;     // Single CMP instruction
```

### Opt-in Safety
```cpp
// Safety only when explicitly requested:
U32 safe_result = a.checked_add(b);  // Has overhead
U32 fast_result = a + b;             // No overhead
```

## 📊 API Reference

### Core Methods (All Types)

| Method | Returns | Description | HolyC Compatible |
|--------|---------|-------------|------------------|
| `raw()` | `storage_type` | Get underlying value | No (extension) |
| `to_hex()` | `const char*` | Hex representation | Yes |
| `Print()` | `void` | Print decimal | Yes |
| `PrintHex()` | `void` | Print hex | Yes |
| `checked_add()` | `T` | Safe addition | No (extension) |
| `checked_sub()` | `T` | Safe subtraction | No (extension) |
| `checked_mul()` | `T` | Safe multiplication | No (extension) |

### Arithmetic Operators (All Implement HolyC Behavior)

| Operator | Example | HolyC Behavior | Safety |
|----------|---------|----------------|--------|
| `+` | `a + b` | Wraps on overflow | Unsafe |
| `-` | `a - b` | Wraps on underflow | Unsafe |
| `*` | `a * b` | Wraps on overflow | Unsafe |
| `/` | `a / b` | Exception on division by zero | Safe |
| `%` | `a % b` | Exception on modulo by zero | Safe |
| `<<` | `a << b` | Exception on large shift | Safe |
| `>>` | `a >> b` | Exception on large shift | Safe |

### Comparison Operators (Match HolyC)

All comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`) behave identically to HolyC with no safety overhead.

## 🔧 Implementation Details

### Template Design
```cpp
template<size_t Bits>
class UInt {
    using storage_type = /* appropriate uintXX_t */;
    storage_type value;
    
    // Bounds checking helper (constexpr when possible)
    template<typename T>
    static void check_bounds(T val) {
        if constexpr (std::is_signed_v<T>) {
            if (val < 0) throw std::out_of_range("Negative to unsigned");
        }
        if (val > static_cast<T>(MAX)) throw std::out_of_range("Too large");
    }
};
```

### Exception Hierarchy
```cpp
std::out_of_range      // Value out of type range
std::overflow_error    // Arithmetic overflow
std::underflow_error   // Arithmetic underflow  
std::domain_error      // Division/modulo by zero
```

## 🧪 Testing Compliance

### HolyC Behavior Tests
```cpp
// Test that HolyC++ matches HolyC behavior
U8 a = 255;
U8 b = 1;
assert((a + b) == 0);  // HolyC wraps, HolyC++ matches

U32 val = 0xFFFFFFFF;
assert(val << 1 == 0xFFFFFFFE);  // Shift matches
```

### Safety Extension Tests
```cpp
// Test that safety features work
U8 max_val = U8::MAX;
try {
    U8 overflow = max_val.checked_add(U8(1));
    assert(false);  // Should not reach here
} catch (const std::overflow_error&) {
    assert(true);  // Correctly caught overflow
}
```

## 📈 Migration Guide

### HolyC → HolyC++

1. **Direct Porting** (Unchanged):
```c
// HolyC
U32 counter = 0;
I32 temperature = -25;
U64 ptr_val = (U64)some_pointer;
```
```cpp
// HolyC++ (identical)
U32 counter = 0;
I32 temperature = -25;
U64 ptr_val = some_pointer;  // Even cleaner
```

2. **Adding Safety** (Optional):
```c
// HolyC (unsafe)
U8 process_byte(U8 a, U8 b) {
    return a + b;  // Might overflow
}
```
```cpp
// HolyC++ (safe)
U8 process_byte(U8 a, U8 b) {
    try {
        return a.checked_add(b);
    } catch (const std::overflow_error&) {
        // Handle overflow
        return U8::MAX;
    }
}
```

## 🎯 Design Rationale

### Why Add Safety Extensions?
1. **Prevent Silent Bugs**: HolyC's silent overflow causes hard-to-find bugs
2. **Gradual Adoption**: Safe operations are opt-in, not mandatory
3. **Performance Preservation**: Unsafe operations remain zero-cost
4. **Modern Expectations**: Developers expect bounds checking in 2024

### Why Maintain HolyC Compatibility?
1. **Code Portability**: Existing HolyC code should work
2. **Philosophical Respect**: Honor Terry Davis's design
3. **Educational Value**: Teach both original and safe patterns
4. **Community Alignment**: Work with existing HolyC ecosystem

## 🔮 Future Extensions

### Planned (Phase 2+):
- `F64` floating-point type with IEEE 754 compliance
- `U0` union type with type tagging
- Compile-time bounds checking where possible
- SIMD operations for vector types

### Optional (For Forks):
- Arbitrary-precision integers
- Decimal floating-point
- Fixed-point arithmetic

---

## 📚 References

1. [HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables)
2. [C++ Standard Template Library](https://en.cppreference.com/w/cpp/header)
3. [Integer Overflow Protection](https://blog.regehr.org/archives/1154)

---

**Status:** Fully Implemented and Tested  
**Next Phase:** `F64` Floating-Point Implementation  
**Maintainer:** @DebadityaMalakar  

*"All the safety of modern C++ with all the spirit of HolyC."*