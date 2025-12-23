#include "../types/union_type.hpp"
#include "../types/signed_int.hpp"
#include "../types/unsigned_int.hpp"
#include "../types/float.hpp"
#include <iostream>
#include <cassert>
#include <type_traits>
#include <cstring>
#include <cmath>

using namespace holycpp;

// Forward declare C functions
extern "C" {
    typedef struct holyc_example_union holyc_example_union_t;
    holyc_example_union_t* holyc_create_example();
    void holyc_free_example(holyc_example_union_t* u);
    void holyc_set_age(holyc_example_union_t* u, int32_t age);
    void holyc_set_char(holyc_example_union_t* u, uint8_t ch);
}

// Test function prototypes
void test_u0_type();
void test_plain_c_union();
void test_type_safe_union();
void test_value_class();
void test_void_union();
void test_union_memory_layout();
void test_union_copy_move();
void test_union_edge_cases();
void test_memory_allocation();
void test_c_compatibility();
void test_compile_time_checks();

int main() {
    std::cout << "🧪 Running HolyC++ Union Type Tests\n";
    std::cout << "=====================================\n";
    
    try {
        test_u0_type();
        test_plain_c_union();
        test_type_safe_union();
        test_value_class();
        test_void_union();
        test_union_memory_layout();
        test_union_copy_move();
        test_union_edge_cases();
        test_memory_allocation();
        test_c_compatibility();
        test_compile_time_checks();
        
        std::cout << "\n✅ All union tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}

void test_u0_type() {
    std::cout << "\n🔹 Testing U0 (void type)...\n";
    
    // Verify U0 properties
    static_assert(U0::SIZE == 0, "U0 must have zero size conceptually");
    static_assert(std::is_same_v<decltype(U0::SIZE), const size_t>, "SIZE must be constexpr");
    static_assert(U0::is_void() == true, "U0 must identify as void");
    
    // U0 is now trivially constructible (needed for unions)
    U0 u;  // Valid - used in unions
    (void)u;  // Suppress unused warning
    
    // But can use it in type calculations
    std::cout << "  U0::SIZE = " << U0::SIZE << " bytes\n";
    std::cout << "  U0::is_void() = " << (U0::is_void() ? "true" : "false") << "\n";
    
    // Can create pointers/references to U0
    U0* ptr = nullptr;
    assert(ptr == nullptr);
    
    // U0 in unions - now works with plain C unions too
    // Note: unions with non-trivial types need explicit constructors
    struct VoidTestWrapper {
        union {
            U0 v;
            I32 i;
        };
        VoidTestWrapper() : i(0) {}  // Initialize union
    };
    
    VoidTestWrapper vt;
    vt.i = 42;
    assert(vt.i == 42);
    
    std::cout << "  ✓ U0 type properties verified\n";
}

void test_plain_c_union() {
    std::cout << "\n🔹 Testing plain C unions (HolyC compatibility)...\n";
    
    // For plain C unions with HolyC++ types, wrap in struct with constructor
    struct ExampleWrapper {
        union {
            I32 age;
            U8 ch;
        };
        ExampleWrapper() : age(0) {}  // Initialize union
    };
    
    ExampleWrapper e;
    
    // Set as integer
    e.age = I32(32);
    std::cout << "  Set age to 32: " << e.age << "\n";
    assert(e.age == 32);
    
    // Read as char (unsafe - typical HolyC)
    std::cout << "  As char (first byte): '" << e.ch << "'\n";
    
    // Set as char
    e.ch = 'A';
    std::cout << "  Set char to 'A'\n";
    assert(e.ch == 'A');
    
    // Read as integer (unsafe - shows byte representation)
    std::cout << "  As integer: " << e.age << " (byte representation)\n";
    
    // Test with more types
    struct MultiTypeWrapper {
        union {
            I32 i;
            F64 f;
            U8 c[8];
        };
        MultiTypeWrapper() : i(0) {}  // Initialize
    };
    
    MultiTypeWrapper mt;
    mt.i = 0x41424344;  // "ABCD" in ASCII
    std::cout << "  Integer 0x41424344 as chars: '";
    for (int i = 0; i < 4; i++) {
        std::cout << mt.c[i];
    }
    std::cout << "'\n";
    assert(mt.c[0] == 'D');  // Little-endian
    
    // Anonymous union in struct
    struct WithAnonymousUnion {
        I32 type;
        union {
            I32 i;
            F64 f;
            U8 c;
        };
        
        // Constructor to initialize union
        WithAnonymousUnion() : type(0), i(0) {}
    };
    
    WithAnonymousUnion wau;
    wau.type = 1;
    wau.i = 42;
    assert(wau.i == 42);
    
    std::cout << "  ✓ Plain C unions work like HolyC\n";
}

void test_type_safe_union() {
    std::cout << "\n🔹 Testing type-safe Union wrapper...\n";
    
    // Create a union like HolyC Example (using type-safe wrapper)
    using ExampleUnion = Union<I32, U8>;
    ExampleUnion e;
    
    // Set as integer
    e.set(I32(42));
    assert(e.is<I32>());
    assert(e.get<I32>() == 42);
    std::cout << "  Set to I32: " << e.get<I32>() << "\n";
    
    // Change to char
    e.set(U8('X'));
    assert(e.is<U8>());
    assert(e.get<U8>() == 'X');
    std::cout << "  Changed to U8: '" << e.get<U8>() << "'\n";
    
    // Check active type
    assert(e.active() == 1);  // U8 should be index 1 (I32 is 0)
    
    // Try to get wrong type - should throw
    bool threw_wrong_type = false;
    try {
        e.get<I32>();  // Should throw
    } catch (const std::runtime_error&) {
        threw_wrong_type = true;
    }
    assert(threw_wrong_type);
    
    // Test with more types
    using MultiUnion = Union<I32, F64, U8, void*>;
    MultiUnion mu;
    
    mu.set(F64(3.14159));
    assert(mu.is<F64>());
    assert(std::abs(mu.get<F64>() - 3.14159) < 0.0001);
    std::cout << "  MultiUnion as F64: " << mu.get<F64>() << "\n";
    
    // Test unsafe access (HolyC style)
    mu.set(I32(100));
    assert(mu.as<I32>() == 100);  // Unsafe but works if we know type
    std::cout << "  Unsafe access as I32: " << mu.as<I32>() << "\n";
    
    // Test reset
    mu.reset();
    assert(mu.active() == -1);
    assert(!mu.is<I32>());
    assert(!mu.is<F64>());
    
    std::cout << "  ✓ Type-safe Union works correctly\n";
}

void test_value_class() {
    std::cout << "\n🔹 Testing Value class (HolyC example)...\n";
    
    // Test 1: Exact HolyC example
    std::cout << "  Testing HolyC example:\n";
    Value* v = MAllocValue();
    v->type = FLOAT_TYPE;
    v->f = 4.20;
    
    assert(v->is_float());
    assert(v->as_float() == 4.20);
    std::cout << "    Created Value with float: " << v->as_float() << "\n";
    
    // Change to char
    v->set_char('Z');
    assert(v->is_char());
    assert(v->as_char() == 'Z');
    std::cout << "    Changed to char: '" << v->as_char() << "'\n";
    
    // Test 2: Using constructors - use explicit type to avoid ambiguity
    Value v2(F64(3.14));
    assert(v2.is_float());
    assert(std::abs(v2.as_float() - 3.14) < 0.001);
    
    Value v3(U8('A'));
    assert(v3.is_char());
    assert(v3.as_char() == 'A');
    
    // Test 3: Pointer to Value
    Value* child = MAllocValue();
    child->set_float(2.71);
    
    Value parent;
    parent.set_value_ptr(child);
    assert(parent.is_value_ptr());
    assert(parent.as_value_ptr() == child);
    assert(std::abs(parent.as_value_ptr()->as_float() - 2.71) < 0.001);
    
    // Test 4: Integer types - use explicit constructor
    Value v4(I32(42));
    assert(v4.is_int());
    assert(v4.as<I32>() == 42);  // Unsafe access through template
    
    // Test 5: Print method
    std::cout << "  Value prints:\n";
    std::cout << "    ";
    v2.Print();
    std::cout << "    ";
    v3.Print();
    
    // Test 6: Memory layout (must match HolyC)
    static_assert(offsetof(Value, type) == 0, "type must be first");
    // Note: union offset may have padding after 'int type' due to alignment
    
    // Cleanup
    Free(v);
    Free(child);
    
    std::cout << "  ✓ Value class works like HolyC\n";
}

void test_void_union() {
    std::cout << "\n🔹 Testing union with U0...\n";
    
    VoidUnion vu;
    
    // Test with U0
    vu.set(U0());
    assert(vu.is<U0>());
    assert(vu.active() == 0);
    std::cout << "  Set to U0 (void)\n";
    
    // Test with I32
    vu.set(I32(100));
    assert(vu.is<I32>());
    assert(vu.get<I32>() == 100);
    std::cout << "  Set to I32: " << vu.get<I32>() << "\n";
    
    // Test with F64
    vu.set(F64(2.71828));
    assert(vu.is<F64>());
    assert(std::abs(vu.get<F64>() - 2.71828) < 0.0001);
    std::cout << "  Set to F64: " << vu.get<F64>() << "\n";
    
    // Test size - max of all types
    assert(vu.Size() >= sizeof(U0));
    assert(vu.Size() >= sizeof(I32));
    assert(vu.Size() >= sizeof(F64));
    
    std::cout << "  ✓ Void union handles U0 correctly\n";
}

void test_union_memory_layout() {
    std::cout << "\n🔹 Testing union memory layout...\n";
    
    // Test 1: Plain C union layout with initialization
    struct LayoutTestWrapper {
        union {
            I32 i;
            F64 f;
            U8 c[8];
        };
        LayoutTestWrapper() : i(0) {}  // Initialize
    };
    
    LayoutTestWrapper lt;
    
    // Verify all members share same address
    assert(reinterpret_cast<void*>(&lt.i) == reinterpret_cast<void*>(&lt.f));
    assert(reinterpret_cast<void*>(&lt.i) == reinterpret_cast<void*>(&lt.c));
    
    // Test 2: Type-safe union storage
    using ExampleUnion = Union<I32, U8>;
    ExampleUnion e;
    e.set(I32(0x12345678));
    
    // Verify storage size
    assert(sizeof(e) >= sizeof(I32));
    assert(sizeof(e) >= sizeof(U8));
    
    // Test 3: Value class layout
    Value v;
    v.set_float(3.14);
    
    // Type and union should be contiguous (with possible padding)
    size_t offset = reinterpret_cast<char*>(&v.f) - reinterpret_cast<char*>(&v.type);
    std::cout << "  Offset from type to union: " << offset << " bytes\n";
    // Note: offset may be >= sizeof(int) due to alignment
    
    // Test 4: Alignment
    using AlignedUnion = Union<I32, F64>;
    // Alignment should be at least as strict as the largest member
    assert(alignof(AlignedUnion) >= alignof(I32));
    assert(alignof(AlignedUnion) >= alignof(F64));
    
    std::cout << "  ✓ Memory layout matches HolyC expectations\n";
}

void test_union_copy_move() {
    std::cout << "\n🔹 Testing union copy and move semantics...\n";
    
    using ExampleUnion = Union<I32, U8>;
    
    // Test 1: Copy constructor
    ExampleUnion e1;
    e1.set(I32(42));
    
    ExampleUnion e2 = e1;  // Copy
    assert(e2.is<I32>());
    assert(e2.get<I32>() == 42);
    
    // Test 2: Assignment
    ExampleUnion e3;
    e3.set(U8('A'));
    e3 = e1;  // Copy assignment
    assert(e3.is<I32>());
    assert(e3.get<I32>() == 42);
    
    // Test 3: Move constructor
    ExampleUnion e4 = std::move(e1);
    assert(e4.is<I32>());
    assert(e4.get<I32>() == 42);
    assert(e1.active() == -1);  // e1 should be empty after move
    
    // Test 4: Move assignment
    ExampleUnion e5;
    e5.set(U8('B'));
    e5 = std::move(e4);
    assert(e5.is<I32>());
    assert(e5.get<I32>() == 42);
    assert(e4.active() == -1);  // e4 should be empty after move
    
    // Test 5: Self-assignment
    e5 = e5;  // Should handle correctly
    assert(e5.is<I32>());
    assert(e5.get<I32>() == 42);
    
    std::cout << "  ✓ Copy and move semantics work correctly\n";
}

void test_union_edge_cases() {
    std::cout << "\n🔹 Testing edge cases...\n";
    
    using ExampleUnion = Union<I32, U8>;
    
    // Test 1: Empty union access
    ExampleUnion e;
    bool threw_empty = false;
    try {
        e.get<I32>();  // Should throw - union is empty
    } catch (const std::runtime_error&) {
        threw_empty = true;
    }
    assert(threw_empty);
    
    // Test 2: Double set
    e.set(I32(1));
    e.set(I32(2));  // Should destroy old and create new
    assert(e.get<I32>() == 2);
    
    // Test 3: Type not in union
    // Compile-time check: e.set(F64(3.14));  // Should not compile
    
    // Test 4: Large union
    using LargeUnion = Union<I8, I16, I32, I64, F32, F64, void*>;
    LargeUnion lu;
    lu.set(I64(0x123456789ABCDEF0LL));
    assert(lu.is<I64>());
    assert(lu.get<I64>() == 0x123456789ABCDEF0LL);
    
    // Test 5: Union of unions
    using NestedUnion = Union<ExampleUnion, F64>;
    NestedUnion nu;
    ExampleUnion inner;
    inner.set(U8('C'));
    nu.set(inner);
    assert(nu.is<ExampleUnion>());
    assert(nu.get<ExampleUnion>().get<U8>() == 'C');
    
    // Test 6: Print on empty union
    std::cout << "  Printing empty union: ";
    e.reset();
    e.Print();  // Should print "<empty union>"
    
    std::cout << "  ✓ Edge cases handled properly\n";
}

void test_memory_allocation() {
    std::cout << "\n🔹 Testing HolyC-style memory allocation...\n";
    
    // Test MAlloc
    I32* intArray = MAlloc<I32>(10);
    assert(intArray != nullptr);
    for (int i = 0; i < 10; i++) {
        new(&intArray[i]) I32(i * i);  // Placement new for initialization
    }
    
    // Verify values
    for (int i = 0; i < 10; i++) {
        assert(intArray[i] == i * i);
    }
    
    // Test MNew
    using ExampleUnion = Union<I32, U8>;
    ExampleUnion* e = MNew<ExampleUnion>();
    e->set(I32(99));
    assert(e->get<I32>() == 99);
    
    // Test MAllocValue (from HolyC example)
    Value* v = MAllocValue();
    v->set_float(3.14159);
    assert(v->is_float());
    
    // Test Free
    Free(intArray);
    Free(e);
    Free(v);
    
    std::cout << "  ✓ Memory allocation works like HolyC\n";
}

void test_c_compatibility() {
    std::cout << "\n🔹 Testing C compatibility...\n";
    
    // Check that C-compatible types are defined
    holyc_example_union_t* test_union = holyc_create_example();
    assert(test_union != nullptr);
    
    // Test C interface
    holyc_set_age(test_union, 25);
    // Note: We can't directly access members from C++ side without knowing the struct layout
    
    holyc_set_char(test_union, 'Q');
    
    holyc_free_example(test_union);
    
    std::cout << "  ✓ C-compatible interface works\n";
}

void test_compile_time_checks() {
    std::cout << "\n🔹 Running compile-time checks...\n";
    
    // These are compile-time assertions
    static_assert(U0::SIZE == 0, "U0 must be zero-sized conceptually");
    static_assert(U0::is_void() == true, "U0 must identify as void");
    
    // Union size checks
    static_assert(Union<I32, U8>::Size() >= sizeof(I32), "Union size check");
    static_assert(Union<I32, U8>::Size() >= sizeof(U8), "Union size check");
    
    // Value class layout checks
    static_assert(offsetof(Value, type) == 0, "Value layout check");
    static_assert(sizeof(Value) >= sizeof(int) + sizeof(F64), "Value size check");
    
    // Type traits - fixed to use int instead of I32
    static_assert(std::is_same_v<decltype(FLOAT_TYPE), const int>, "Constant type check");
    static_assert(FLOAT_TYPE == 0, "Constant value check");
    static_assert(CHAR_TYPE == 1, "Constant value check");
    static_assert(VALUE_TYPE == 2, "Constant value check");
    
    std::cout << "  ✓ All compile-time checks passed\n";
}

// Additional demonstration functions
void demonstrate_holyc_examples() {
    std::cout << "\n📚 Demonstrating HolyC examples from documentation:\n";
    
    // Example 1: Basic union - use wrapper struct
    std::cout << "\n1. Basic union example:\n";
    struct ExampleWrapper {
        union {
            I32 age;
            U8 ch;
        };
        ExampleWrapper() : age(0) {}
    };
    
    ExampleWrapper e;
    e.age = 32;
    std::cout << "   union Example e;\n";
    std::cout << "   e.age = 32;\n";
    std::cout << "   Result: e.age = " << e.age << "\n";
    
    // Example 2: Value class
    std::cout << "\n2. Value class example:\n";
    std::cout << "   #define FLOAT_TYPE 0\n";
    std::cout << "   #define CHAR_TYPE  1\n";
    std::cout << "   #define VALUE_TYPE 2\n";
    std::cout << "   \n";
    std::cout << "   class Value {\n";
    std::cout << "     int type;\n";
    std::cout << "     union {\n";
    std::cout << "       F64 f;\n";
    std::cout << "       U8 ch;\n";
    std::cout << "       Value *val;\n";
    std::cout << "     };\n";
    std::cout << "   };\n";
    std::cout << "   \n";
    std::cout << "   Value *v = MAlloc(sizeof(Value));\n";
    std::cout << "   v->type = FLOAT_TYPE;\n";
    std::cout << "   v->f = 4.20;\n";
    
    Value* v = MAllocValue();
    v->type = FLOAT_TYPE;
    v->f = 4.20;
    std::cout << "   Result: v->f = " << v->f << "\n";
    
    Free(v);
}