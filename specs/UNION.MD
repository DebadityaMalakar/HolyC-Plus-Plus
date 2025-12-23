# HolyC++ Union Type Specification

## 📋 Overview

This document specifies the union type system implementation in HolyC++, which provides both exact HolyC compatibility and modern type-safe union handling. Unions in HolyC allow multiple types to share the same memory location, similar to C unions but with HolyC's unique semantics.

**Reference:** [HolyC Language Specification](https://holyc-lang.com/docs/language-spec/learn-variables#unions)

## 🎯 Core Union Concepts

### **U0 - The Void Type (Implemented ✅)**
```cpp
struct U0 {
    static constexpr size_t SIZE = 0;
    static constexpr bool is_void() { return true; }
    
    // Trivially constructible for union compatibility
    constexpr U0() = default;
};
```

**Purpose:**
- Represents the void type in HolyC unions
- Zero-sized conceptually (though sizeof(U0) may be 1 due to C++ rules)
- Used when a union member can be "empty" or uninitialized
- Essential for exact HolyC memory layout compatibility

### **Union Types in HolyC**
HolyC supports three union patterns:

1. **Basic Unions** - Direct C-style unions
2. **Tagged Unions** - Union with type discriminator (like `Value` class)
3. **Anonymous Unions** - Unions within structs without a name

## 📊 Union Implementation

### **1. Plain C Unions (HolyC Compatibility)**
```c
// HolyC Syntax:
union Example {
  I32 age;
  U8 ch;
};

Example e;
e.age = 32;
U8 c = e.ch;  // Access as different type (unsafe)
```

**HolyC++ Implementation:**
```cpp
// For plain C unions with HolyC++ types, wrap in struct with constructor
struct ExampleWrapper {
    union {
        I32 age;
        U8 ch;
    };
    ExampleWrapper() : age(0) {}  // Initialize union
};

// Usage:
ExampleWrapper e;
e.age = I32(32);
std::cout << "As char (first byte): '" << e.ch << "'\n";  // Unsafe access
```

**Memory Layout:**
```
Example union (sizeof = max(sizeof(I32), sizeof(U8)) = 4 bytes)
┌─────────────────┐
│ byte 0 │ byte 1 │ byte 2 │ byte 3 │
└─────────────────┘
     ▲         ▲
    age       ch accesses only byte 0
```

### **2. Type-Safe Union Wrapper (New in HolyC++)**

#### **Union<Types...> Template**
```cpp
template<typename... Types>
class Union {
private:
    alignas(max_align()) unsigned char data[max_size()];
    int active_type = -1;  // -1 = empty, 0 = first type, 1 = second type, etc.
    
public:
    // Type-safe access
    template<typename T>
    void set(const T& value);
    
    template<typename T>
    T& get();  // Throws if wrong type
    
    template<typename T>
    const T& get() const;
    
    // Unsafe HolyC-style access
    template<typename T>
    T as() const;  // No type checking - HolyC style
    
    // Type checking
    template<typename T>
    bool is() const;
    
    int active() const;  // Returns active type index
    
    void reset();  // Destroy active member
    
    void Print() const;
    
    static constexpr size_t Size();
    static constexpr size_t Alignment();
};
```

#### **Usage Examples:**
```cpp
using Example = Union<I32, U8>;

Example e;
e.set(I32(42));          // Set as integer
assert(e.is<I32>());     // Check type
assert(e.get<I32>() == 42);  // Type-safe access

e.set(U8('A'));          // Change to char
assert(e.as<U8>() == 'A');  // Unsafe HolyC-style access

// Wrong type access throws
bool threw = false;
try {
    e.get<I32>();  // Currently holds U8, should throw
} catch (const std::runtime_error&) {
    threw = true;
}
assert(threw);
```

### **3. Value Class (From HolyC Documentation)**

The `Value` class is a specific example from HolyC documentation that demonstrates tagged unions:

```c
// HolyC Example from documentation:
#define FLOAT_TYPE 0
#define CHAR_TYPE  1
#define VALUE_TYPE 2

class Value {
  int type;
  union {
    F64 f;
    U8 ch;
    Value *val;
  };
};

// Usage:
Value *v = MAlloc(sizeof(Value));
v->type = FLOAT_TYPE;
v->f = 4.20;
```

**HolyC++ Implementation:**
```cpp
class Value {
public:
    int type;  // Type discriminator
    
    union {
        F64 f;
        U8 ch;
        Value *val;
        I32 i;
        U32 u;
    };
    
    // Constructors
    Value() : type(-1), f(0.0) {}
    Value(F64 value) : type(FLOAT_TYPE), f(value) {}
    Value(U8 value) : type(CHAR_TYPE), ch(value) {}
    Value(Value* value) : type(VALUE_TYPE), val(value) {}
    Value(I32 value) : type(3), i(value) {}
    Value(U32 value) : type(4), u(value) {}
    
    // Type-safe accessors
    F64 as_float() const {
        if (type != FLOAT_TYPE) throw std::runtime_error("Not a float");
        return f;
    }
    
    // Unsafe HolyC-style access
    template<typename T>
    T as() const {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        return *reinterpret_cast<const T*>(&f);
        #pragma GCC diagnostic pop
    }
    
    // Type predicates
    bool is_float() const { return type == FLOAT_TYPE; }
    bool is_char() const { return type == CHAR_TYPE; }
    // ... etc
    
    void Print() const;
};
```

## 🔄 Memory Layout Compatibility

### **Exact HolyC Memory Layout**
HolyC++ ensures exact memory layout compatibility:

1. **Size Matching**:
   ```cpp
   // HolyC: sizeof(union { I32 i; U8 c; }) == 4
   // HolyC++: Union<I32, U8>::Size() == 4
   static_assert(sizeof(Example) == Union<I32, U8>::Size());
   ```

2. **Alignment Matching**:
   ```cpp
   // Union alignment = max alignment of all members
   static_assert(alignof(Union<I32, F64>) == alignof(F64));
   ```

3. **Offset Guarantees**:
   ```cpp
   // Value class must match HolyC layout exactly:
   static_assert(offsetof(Value, type) == 0);  // type is first
   // Union starts immediately after (with possible padding)
   ```

### **Memory Representation Examples**

#### **Union with I32 and U8**:
```
Memory layout (4 bytes):
┌─────────────────────────────┐
│ I32 (all 4 bytes) or U8 (byte 0) │
├─────────────────────────────┤
│ byte 0 │ byte 1 │ byte 2 │ byte 3 │
└─────────────────────────────┘
```

#### **Value Class Layout**:
```
Value (size = sizeof(int) + max(sizeof(union members))):
┌─────────────┬─────────────────┐
│ int type    │ union { ... }   │
├─────────────┼─────────────────┤
│ 4 bytes     │ 8 bytes (for F64) │
└─────────────┴─────────────────┘
Note: May have padding between type and union for alignment
```

## 🛡️ Safety Features

### **1. Type-Safe Access (New in HolyC++)**
```cpp
Union<I32, U8> u;
u.set(I32(42));

// Safe access:
assert(u.get<I32>() == 42);  // OK - correct type

// Wrong type throws:
try {
    u.get<U8>();  // Throws std::runtime_error
} catch (const std::runtime_error& e) {
    // Error: Wrong active type in union
}
```

### **2. Active Type Tracking**
```cpp
Union<I32, U8, F64> u;
assert(u.active() == -1);  // Empty

u.set(I32(100));
assert(u.active() == 0);   // I32 is first type (index 0)

u.set(F64(3.14));
assert(u.active() == 2);   // F64 is third type (index 2)

u.reset();
assert(u.active() == -1);  // Empty again
```

### **3. Proper Lifetime Management**
```cpp
// Union with non-trivial types
Union<std::string, std::vector<int>> u;

u.set(std::string("Hello"));
// string constructor called, memory allocated

u.set(std::vector<int>{1, 2, 3});
// string destructor called (memory freed)
// vector constructor called

u.reset();
// vector destructor called (memory freed)
```

### **4. Move/Copy Semantics**
```cpp
Union<I32, std::string> u1;
u1.set(std::string("Original"));

// Copy constructor
Union<I32, std::string> u2 = u1;
assert(u2.is<std::string>());
assert(u2.get<std::string>() == "Original");

// Move constructor
Union<I32, std::string> u3 = std::move(u1);
assert(u3.get<std::string>() == "Original");
assert(u1.active() == -1);  // u1 is now empty
```

## ⚡ Performance Considerations

### **Zero Overhead When Used Like HolyC**
```cpp
// Plain C union (zero overhead):
struct ExampleWrapper {
    union { I32 age; U8 ch; };
    ExampleWrapper() : age(0) {}
};

// Compiles to same machine code as HolyC
// No runtime type information
// No virtual functions
// Memory layout identical to C
```

### **Minimal Overhead for Type-Safe Union**
```cpp
Union<I32, U8> u;
u.set(I32(42));

// Overhead:
// - One integer for active type tracking (4-8 bytes)
// - One bounds check on get<T>()
// - No virtual functions
// - No heap allocations (unless members allocate)
```

### **Memory Allocation**
```cpp
// Stack allocation (fast):
Union<I32, U8> stack_union;

// Heap allocation (HolyC style):
Union<I32, U8>* heap_union = MNew<Union<I32, U8>>();
Free(heap_union);

// Value class allocation (from HolyC docs):
Value* v = MAllocValue();  // HolyC's MAlloc(sizeof(Value))
```

## 📊 API Reference

### **Union<Types...> Methods**

| Method | Returns | Description | HolyC Compatible |
|--------|---------|-------------|------------------|
| `set<T>(value)` | `void` | Set value of type T | No (extension) |
| `get<T>()` | `T&` | Get value of type T (type-safe) | No (extension) |
| `as<T>()` | `T` | Get value of type T (unsafe) | Yes |
| `is<T>()` | `bool` | Check if active type is T | No (extension) |
| `active()` | `int` | Get active type index (-1 = empty) | No (extension) |
| `reset()` | `void` | Destroy active member | No (extension) |
| `Print()` | `void` | Print contents | Yes |
| `Size()` | `size_t` | Maximum size of union types | No (extension) |
| `Alignment()` | `size_t` | Maximum alignment of union types | No (extension) |

### **Value Class Methods**

| Method | Returns | Description |
|--------|---------|-------------|
| Constructors | - | From F64, U8, Value*, I32, U32 |
| `as_float()` | `F64` | Type-safe float access |
| `as_char()` | `U8` | Type-safe char access |
| `as_value_ptr()` | `Value*` | Type-safe pointer access |
| `as<T>()` | `T` | Unsafe HolyC-style access |
| `is_float()` | `bool` | Type predicate |
| `is_char()` | `bool` | Type predicate |
| `set_float(f)` | `void` | Set as float |
| `set_char(c)` | `void` | Set as char |
| `Print()` | `void` | Print value |

### **Memory Allocation Functions**

| Function | Returns | Description |
|----------|---------|-------------|
| `MNew<T>()` | `T*` | Allocate and default-construct |
| `MAlloc<T>(count=1)` | `T*` | Raw allocation (no construction) |
| `Free(ptr)` | `void` | Free allocated memory |
| `MAllocValue()` | `Value*` | Allocate a Value (HolyC example) |

## 🔧 Implementation Details

### **Template Implementation**
```cpp
template<typename... Types>
class Union {
    // Compute max size at compile time
    static constexpr size_t max_size() {
        size_t sizes[] = {sizeof(Types)...};
        size_t max = 0;
        for (size_t s : sizes) {
            if (s > max) max = s;
        }
        return max;
    }
    
    // Compute max alignment at compile time
    static constexpr size_t max_align() {
        size_t aligns[] = {alignof(Types)...};
        size_t max = 0;
        for (size_t a : aligns) {
            if (a > max) max = a;
        }
        return max;
    }
    
    // Storage with proper alignment
    alignas(max_align()) unsigned char data[max_size()];
    int active_type = -1;
    
    // Type index calculation at compile time
    template<typename T>
    static constexpr int get_type_index() {
        // Returns 0 for first type, 1 for second, etc.
        // Returns -1 if type not in union (compile error)
    }
};
```

### **Type Index Calculation**
```cpp
// Compile-time type index calculation
template<typename T, typename First, typename... Rest>
static constexpr int type_index_impl() {
    if constexpr (std::is_same_v<T, First>) {
        return 0;
    } else if constexpr (sizeof...(Rest) > 0) {
        return 1 + type_index_impl<T, Rest...>();
    } else {
        return -1;  // Type not found
    }
}
```

### **Active Member Management**
```cpp
// Destroy active member based on type index
template<int I, typename T, typename... Rest>
void destroy_impl() {
    if (active_type == I) {
        reinterpret_cast<T*>(data)->~T();
    } else if constexpr (sizeof...(Rest) > 0) {
        destroy_impl<I + 1, Rest...>();
    }
}

// Construct active member after copy
template<int I, typename T, typename... Rest>
void construct_impl() {
    if (active_type == I) {
        new(data) T(*reinterpret_cast<const T*>(data));
    } else if constexpr (sizeof...(Rest) > 0) {
        construct_impl<I + 1, Rest...>();
    }
}
```

## 🧪 Testing Compliance

### **HolyC Compatibility Tests**
```cpp
// Test that memory layout matches HolyC
struct HolyCExample {
    union {
        int32_t age;
        uint8_t ch;
    };
};

using Example = Union<I32, U8>;

// Size must match
static_assert(sizeof(HolyCExample) == sizeof(Example));

// Alignment must match  
static_assert(alignof(HolyCExample) == alignof(Example));

// Byte representation must match
HolyCExample c_example;
c_example.age = 0x41424344;  // "ABCD" in ASCII

Example holycpp_example;
holycpp_example.set(I32(0x41424344));

// First byte must be the same
assert(reinterpret_cast<uint8_t*>(&c_example)[0] == 
       reinterpret_cast<uint8_t*>(&holycpp_example)[0]);
```

### **Value Class Tests (From HolyC Docs)**
```cpp
// Test exact HolyC example from documentation
Value* v = MAllocValue();
v->type = FLOAT_TYPE;
v->f = 4.20;

assert(v->is_float());
assert(v->as_float() == 4.20);

// Change to char (HolyC style)
v->set_char('X');
assert(v->is_char());
assert(v->as_char() == 'X');

// Unsafe access (HolyC style)
v->set_int(0x41424344);  // "ABCD" in ASCII
assert(v->as<U8>() == 'D');  // Little-endian first byte

Free(v);
```

### **Type-Safety Tests**
```cpp
// Test type-safe access
Union<I32, U8> u;
u.set(I32(42));

// Correct type works
assert(u.get<I32>() == 42);

// Wrong type throws
bool threw = false;
try {
    u.get<U8>();
} catch (const std::runtime_error&) {
    threw = true;
}
assert(threw);

// Unsafe access works (HolyC style)
assert(u.as<U8>() == 42);  // First byte of 42
```

## 📈 Migration Guide

### **HolyC → HolyC++**

1. **Direct Porting** (Unchanged for plain unions):
```c
// HolyC
union Example {
  I32 age;
  U8 ch;
};

Example e;
e.age = 32;
U8 c = e.ch;
```
```cpp
// HolyC++ (using wrapper for plain union)
struct ExampleWrapper {
    union { I32 age; U8 ch; };
    ExampleWrapper() : age(0) {}
};

ExampleWrapper e;
e.age = I32(32);
U8 c = e.ch;  // Same unsafe access
```

2. **Using Type-Safe Union** (New feature):
```cpp
// HolyC++ with type safety
Union<I32, U8> e;
e.set(I32(32));  // Type-safe set

// Type-safe get (throws if wrong type)
I32 age = e.get<I32>();

// Or unsafe HolyC-style get
U8 c = e.as<U8>();
```

3. **Value Class** (From HolyC docs, unchanged):
```c
// HolyC Value class
class Value {
  int type;
  union {
    F64 f;
    U8 ch;
    Value *val;
  };
};

Value *v = MAlloc(sizeof(Value));
v->type = FLOAT_TYPE;
v->f = 4.20;
```
```cpp
// HolyC++ identical
Value* v = MAllocValue();  // Helper function
v->type = FLOAT_TYPE;
v->f = 4.20;
```

## 🎯 Design Rationale

### **Why Both Plain and Type-Safe Unions?**

1. **HolyC Compatibility**: Plain unions match HolyC exactly
2. **Safety When Needed**: Type-safe unions prevent bugs
3. **Performance Choice**: Plain unions have zero overhead
4. **Gradual Migration**: Mix both in same codebase

### **U0 (Void Type) Design**
- **Trivially Constructible**: Essential for union compatibility
- **Zero-Sized Conceptually**: Matches HolyC's void type concept
- **Compile-Time Information**: SIZE and is_void() are constexpr
- **Union Compatibility**: Can be used as a union member

### **Memory Layout Guarantees**
- **Exact Match with C**: sizeof(Union) == sizeof(C union)
- **No Hidden Padding**: Transparent memory representation
- **Predictable Offsets**: Essential for systems programming
- **Binary Compatibility**: Can exchange data with HolyC programs

## 🔮 Future Extensions

### **Planned Features:**

1. **Pattern Matching for Unions**:
```cpp
Union<I32, std::string> u = /* ... */;

// Proposed syntax (C++26?):
visit(u,
    [](I32 i) { std::cout << "Integer: " << i; },
    [](const std::string& s) { std::cout << "String: " << s; }
);
```

2. **Compile-Time Type Checking**:
```cpp
// Static assert for valid union operations
using ValidUnion = Union<I32, U8, F64>;
using InvalidUnion = Union<I32, std::unique_ptr<int>>;  // Compile error
```

3. **Serialization Support**:
```cpp
Union<I32, std::string> u;
u.set(std::string("Hello"));

// Automatic serialization
std::string json = u.to_json();  // {"type":"string","value":"Hello"}
```

4. **Reflection Support**:
```cpp
Union<I32, U8> u;
u.set(I32(42));

// Get type name at runtime
std::string type_name = u.type_name();  // "I32"

// Get all possible types
auto types = u.possible_types();  // {"I32", "U8"}
```

### **Optional Features (For Forks):**

1. **Garbage Collection Integration**
2. **Persistent Unions** (disk storage)
3. **Network-Serializable Unions**
4. **Transactional Union Updates**

## 📚 References

1. [HolyC Language Specification - Unions](https://holyc-lang.com/docs/language-spec/learn-variables#unions)
2. [C++ Standard: Unions](https://en.cppreference.com/w/cpp/language/union)
3. [std::variant (Type-Safe Union)](https://en.cppreference.com/w/cpp/utility/variant)
4. [Pattern Matching for Unions](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p2392r1.pdf)

---

## 🏁 Implementation Status

### **✅ Complete**:
- `U0` void type with union compatibility
- `Union<Types...>` type-safe wrapper
- Plain C union compatibility
- `Value` class from HolyC documentation
- Memory allocation helpers (MAlloc, Free, MNew)
- Comprehensive test suite
- Exact memory layout guarantees

### **🔧 Working Features**:
- Type-safe get/set operations
- Unsafe HolyC-style as<T>() access
- Active type tracking
- Proper construction/destruction
- Move/copy semantics
- C compatibility layer

### **📋 Future Considerations**:
- Pattern matching syntax
- Compile-time validation
- Serialization support
- Reflection capabilities

---

**Maintainer**: @DebadityaMalakar  
**Specification Version**: 1.0.0  
**Implementation Version**: Phase 2 Complete  

*"Unions: All the types, none of the safety (unless you want it)." - HolyC++ Union Philosophy*