#include "union_type.hpp"

namespace holycpp {

// ==================== Explicit Template Instantiations ====================
// Force instantiation of common union types
template class Union<I32, U8>;
template class Union<F64, U8, void*>;
template class Union<U0, I32, F64>;
template class Union<void*, I64, U64>;
template class Union<I32, F64, U8>;
template class Union<U8, U16, U32, U64>;
template class Union<I8, I16, I32, I64>;
template class Union<F32, F64>;

// ==================== Global Examples ====================
// Example from HolyC documentation
void DemonstrateHolyCUnion() {
    // HolyC syntax:
    // union Example { I32 age; U8 ch; };
    // Example e;
    // e.age = 32;
    
    Example e;
    e.set(I32(32));
    
    std::cout << "Union holds I32: ";
    if (e.is<I32>()) {
        std::cout << e.get<I32>() << std::endl;
    }
    
    // Change to char
    e.set(U8('A'));
    std::cout << "Union holds U8: ";
    if (e.is<U8>()) {
        std::cout << "'" << e.get<U8>() << "'" << std::endl;
    }
    
    // Unsafe HolyC-style access
    std::cout << "As char (unsafe): '" << e.as<U8>() << "'" << std::endl;
}

// Value class example from HolyC documentation
void DemonstrateValueClass() {
    // From HolyC:
    // #define FLOAT_TYPE 0
    // #define CHAR_TYPE  1
    // #define VALUE_TYPE 2
    // 
    // class Value {
    //   I32 type;
    //   union {
    //     F64 f;
    //     U8 ch;
    //     Value *val;
    //   };
    // };
    // 
    // Value *v = MAlloc(sizeof(Value));
    // v->type = FLOAT_TYPE;
    // v->f = 4.20;
    
    Value* v = MAllocValue();
    v->type = FLOAT_TYPE;
    v->f = 4.20;
    
    std::cout << "\nHolyC Value example:" << std::endl;
    std::cout << "Value type: " << v->type << " (FLOAT_TYPE)" << std::endl;
    std::cout << "Value as float: " << v->as_float() << std::endl;
    
    v->Print();
    
    // Change to char
    v->set_char('X');
    v->Print();
    
    // Clean up
    Free(v);
}

// U0 (void type) demonstration
void DemonstrateU0() {
    std::cout << "\nU0 (void type) demonstration:" << std::endl;
    std::cout << "Size of U0: " << U0::SIZE << " bytes" << std::endl;
    std::cout << "Is U0 void? " << (U0::is_void() ? "Yes" : "No") << std::endl;
    
    U0::Print();
    
    // Now we CAN create instances (U0 is trivially constructible)
    U0 u;  // Valid - used in unions
    (void)u;  // Suppress unused warning
}

// Union with U0
void DemonstrateVoidUnion() {
    std::cout << "\nUnion with U0:" << std::endl;
    
    VoidUnion vu;
    
    // Can set to U0 (though it has no value)
    vu.set(U0());
    std::cout << "Union holds U0: ";
    if (vu.is<U0>()) {
        std::cout << "Yes" << std::endl;
    }
    
    // Can also set to other types
    vu.set(I32(42));
    std::cout << "Union holds I32: " << vu.get<I32>() << std::endl;
    
    vu.set(F64(3.14));
    std::cout << "Union holds F64: " << vu.get<F64>() << std::endl;
}

// Direct C union compatibility
void DemonstrateCCompatibleUnion() {
    std::cout << "\nC-compatible union:" << std::endl;
    
    // C-style anonymous union - use plain C types
    union {
        int32_t i;
        double f;
        uint8_t c;
    } c_union = {0};  // Initialize to avoid undefined behavior
    
    // Exactly like HolyC
    c_union.i = 42;
    std::cout << "C union as int: " << c_union.i << std::endl;
    
    c_union.f = 3.14;
    std::cout << "C union as float: " << c_union.f << std::endl;
    
    c_union.c = 'A';
    std::cout << "C union as char: '" << c_union.c << "'" << std::endl;
    
    // The problem: we don't know which is active!
    std::cout << "But what's really in memory? int: " << c_union.i 
              << ", float: " << c_union.f << std::endl;
}

// Size and alignment information
void PrintUnionSizes() {
    std::cout << "\nUnion sizes and alignments:" << std::endl;
    std::cout << "sizeof(Example): " << sizeof(Example) 
              << " (should be max of sizeof(I32), sizeof(U8))" << std::endl;
    std::cout << "Example::Size(): " << Example::Size() << std::endl;
    std::cout << "Example::Alignment(): " << Example::Alignment() << std::endl;
    
    std::cout << "\nsizeof(Value): " << sizeof(Value) << std::endl;
    std::cout << "Offset of type: 0" << std::endl;
    std::cout << "Offset of union: " << offsetof(Value, f) << std::endl;
}

// Test all features
void TestAllUnionFeatures() {
    std::cout << "=== HolyC++ Union Type Tests ===" << std::endl;
    
    DemonstrateU0();
    DemonstrateHolyCUnion();
    DemonstrateValueClass();
    DemonstrateVoidUnion();
    DemonstrateCCompatibleUnion();
    PrintUnionSizes();
    
    std::cout << "\n=== All tests completed ===" << std::endl;
}

// ==================== Helper Functions ====================
// Factory functions
Example CreateExampleInt(I32 value) {
    Example e;
    e.set(value);
    return e;
}

Example CreateExampleChar(U8 value) {
    Example e;
    e.set(value);
    return e;
}

// Union operations
template<typename... Types>
void PrintUnionInfo(const Union<Types...>& u) {
    std::cout << "Union active type: " << u.active() << std::endl;
    std::cout << "Union contents: ";
    u.Print();
}

// ==================== Compile-time Type Checks ====================
// Check that U0 is trivially constructible (needed for union compatibility)
static_assert(std::is_trivially_constructible_v<U0>, "U0 must be trivially constructible");

// Check that unions are properly sized
static_assert(sizeof(Example) >= sizeof(I32), "Union must be at least as large as its largest member");
static_assert(sizeof(Example) >= sizeof(U8), "Union must be at least as large as its largest member");

// Check Value class layout (must match HolyC)
static_assert(offsetof(Value, type) == 0, "type must be first member");
// Note: union offset depends on alignment padding after 'int type'

// Type traits
static_assert(std::is_same_v<decltype(FLOAT_TYPE), const int>, "Constant type check");
static_assert(FLOAT_TYPE == 0, "Constant value check");
static_assert(CHAR_TYPE == 1, "Constant value check");
static_assert(VALUE_TYPE == 2, "Constant value check");

// ==================== Runtime Tests ====================
bool RunUnionTests() {
    try {
        TestAllUnionFeatures();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return false;
    }
}

} // namespace holycpp

// ==================== C Compatibility ====================
// C-compatible exports for linking with C code
extern "C" {
    
    // C-compatible union (exactly matches HolyC layout) - use plain C types
    typedef struct holyc_example_union {
        union {
            int32_t age;
            uint8_t ch;
        };
    } holyc_example_union_t;
    
    // Create a union in C
    holyc_example_union_t* holyc_create_example() {
        auto* u = new holyc_example_union_t();
        u->age = 0;  // Initialize
        return u;
    }
    
    // Free a union
    void holyc_free_example(holyc_example_union_t* u) {
        delete u;
    }
    
    // Set age (C interface)
    void holyc_set_age(holyc_example_union_t* u, int32_t age) {
        u->age = age;
    }
    
    // Set char (C interface)
    void holyc_set_char(holyc_example_union_t* u, uint8_t ch) {
        u->ch = ch;
    }
}