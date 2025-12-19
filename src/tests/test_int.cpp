#include "../types/unsigned_int.hpp"
#include "../types/signed_int.hpp"
#include <iostream>
#include <cassert>

using namespace holycpp;

// Test function prototypes
void test_unsigned_basics();
void test_signed_basics();
void test_arithmetic();
void test_bitwise();
void test_comparisons();
void test_conversions();
void test_pointers();
void test_edge_cases();

int main() {
    std::cout << "🧪 Running HolyC++ Integer Tests\n";
    std::cout << "=================================\n";
    
    try {
        test_unsigned_basics();
        test_signed_basics();
        test_arithmetic();
        test_bitwise();
        test_comparisons();
        test_conversions();
        test_pointers();
        test_edge_cases();
        
        std::cout << "\n✅ All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}

void test_unsigned_basics() {
    std::cout << "\n🔹 Testing unsigned basics...\n";
    
    // Default construction
    U8 u8_default;
    assert(u8_default == 0);
    
    // Value construction
    U8 u8_val = 255;
    assert(u8_val == 255);
    assert(u8_val.raw() == 255);
    
    U16 u16_val = 65535;
    assert(u16_val == 65535);
    
    U32 u32_val = 4294967295U;
    assert(u32_val == 4294967295U);
    
    U64 u64_val = 18446744073709551615ULL;
    assert(u64_val == 18446744073709551615ULL);
    
    // Constants
    assert(U8::MIN == 0);
    assert(U8::MAX == 255);
    assert(U16::MAX == 65535);
    assert(U32::MAX == 4294967295U);
    assert(U64::MAX == 18446744073709551615ULL);
    
    std::cout << "  ✓ Basic construction and constants\n";
}

void test_signed_basics() {
    std::cout << "\n🔹 Testing signed basics...\n";
    
    // Default construction
    I8 i8_default;
    assert(i8_default == 0);
    
    // Value construction
    I8 i8_val = -128;
    assert(i8_val == -128);
    assert(i8_val.raw() == -128);
    
    I16 i16_val = -32768;
    assert(i16_val == -32768);
    
    I32 i32_val = -2147483648;
    assert(i32_val == -2147483648);
    
    I64 i64_val = I64(-9223372036854775807LL);
    assert(i64_val == I64(-9223372036854775807LL));
    
    // Constants
    assert(I8::MIN == -128);
    assert(I8::MAX == 127);
    assert(I16::MIN == -32768);
    assert(I32::MIN == -2147483648);
    
    std::cout << "  ✓ Basic signed construction\n";
}

void test_arithmetic() {
    std::cout << "\n🔹 Testing arithmetic operations...\n";
    
    U32 a = 100;
    U32 b = 50;
    
    // Basic arithmetic
    assert(a + b == 150);
    assert(a - b == 50);
    assert(a * b == 5000);
    assert(a / b == 2);
    assert(a % 30 == 10);
    
    // Compound assignment
    U32 c = a;
    c += b;
    assert(c == 150);
    
    c = a;
    c -= b;
    assert(c == 50);
    
    // Increment/decrement
    U32 d = 10;
    assert(d++ == 10);
    assert(d == 11);
    assert(++d == 12);
    
    // Negative arithmetic (signed)
    I32 neg1 = I32(-100);
    I32 neg2 = I32(50);
    assert(neg1 + neg2 == -50);
    assert(neg1 - neg2 == -150);
    assert(neg2 - neg1 == 150);
    assert(-neg1 == 100);
    
    std::cout << "  ✓ Arithmetic operations\n";
}

void test_bitwise() {
    std::cout << "\n🔹 Testing bitwise operations...\n";
    
    U32 x = 0xAAAAAAAA;  // 10101010...
    U32 y = 0x55555555;  // 01010101...
    
    // Basic bitwise
    assert((x & y) == 0);
    assert((x | y) == 0xFFFFFFFF);
    assert((x ^ y) == 0xFFFFFFFF);
    assert((~x) == 0x55555555);
    
    // Shifts
    U32 val = 0x0000000F;
    assert((val << 4) == 0x000000F0);
    assert((val >> 2) == 0x00000003);
    
    // Compound bitwise
    U32 z = x;
    z &= y;
    assert(z == 0);
    
    z = x;
    z |= y;
    assert(z == 0xFFFFFFFF);
    
    std::cout << "  ✓ Bitwise operations\n";
}

void test_comparisons() {
    std::cout << "\n🔹 Testing comparisons...\n";
    
    U32 small = 10;
    U32 medium = 20;
    U32 large = 30;
    
    // Equality
    assert(small == 10);
    assert(small != 20);
    
    // Relational
    assert(small < medium);
    assert(medium > small);
    assert(small <= medium);
    assert(medium >= small);
    assert(large >= large);
    assert(medium <= medium);
    
    // Signed comparisons with negatives
    I32 neg = I32(-10);
    I32 pos = I32(10);
    assert(neg < pos);
    assert(pos > neg);
    assert(neg < 0);
    assert(pos > 0);
    
    std::cout << "  ✓ Comparison operations\n";
}

void test_conversions() {
    std::cout << "\n🔹 Testing type conversions...\n";
    
    // Implicit conversions
    U8 u8 = 100;
    U16 u16 = u8;  // U8 -> U16
    assert(u16 == 100);
    
    U32 u32 = u16; // U16 -> U32
    assert(u32 == 100);
    
    U64 u64 = u32; // U32 -> U64
    assert(u64 == 100);
    
    // Signed to unsigned (HolyC++ style with bounds checking)
    I32 i32_pos = I32(42);
    U32 from_signed_pos = i32_pos;  // Positive values work
    assert(from_signed_pos == 42);
    
    // Negative values throw exception
    I32 i32_neg = I32(-1);
    bool threw_exception = false;
    try {
        U32 from_signed_neg = i32_neg;
        (void)from_signed_neg; // Suppress unused warning
    } catch (const std::out_of_range&) {
        threw_exception = true;
    }
    assert(threw_exception);
    
    // Unsigned to signed
    U32 small_val = 100;
    I32 to_signed = small_val;
    assert(to_signed == 100);
    
    std::cout << "  ✓ Type conversions\n";
}

void test_pointers() {
    std::cout << "\n🔹 Testing pointer conversions...\n";
    
    int value = 42;
    void* ptr = &value;
    
    // Pointer to integer (HolyC style)
    U64 ptr_as_int = reinterpret_cast<U64::storage_type>(ptr);
    
    // Integer to pointer (HolyC style)
    void* ptr2 = reinterpret_cast<void*>(ptr_as_int.raw());
    
    // Should round-trip
    assert(ptr == ptr2);
    
    // Also test constructor from pointer
    U64 from_ptr = U64(ptr);
    assert(from_ptr == ptr_as_int);
    
    std::cout << "  ✓ Pointer conversions\n";
}

void test_edge_cases() {
    std::cout << "\n🔹 Testing edge cases...\n";
    
    // Test checked arithmetic operations
    U8 max_u8 = U8::MAX;
    bool overflow_threw = false;
    try {
        U8 overflow_checked = max_u8.checked_add(U8(1));
        (void)overflow_checked;
    } catch (const std::overflow_error&) {
        overflow_threw = true;
    }
    assert(overflow_threw);
    std::cout << "  ✓ Checked overflow detection\n";
    
    // Test underflow detection
    U8 min_u8 = U8::MIN;
    bool underflow_threw = false;
    try {
        U8 underflow_checked = min_u8.checked_sub(U8(1));
        (void)underflow_checked;
    } catch (const std::underflow_error&) {
        underflow_threw = true;
    }
    assert(underflow_threw);
    std::cout << "  ✓ Checked underflow detection\n";
    
    // Unchecked operations allow wrapping behavior (HolyC-style)
    // Note: The arithmetic operators don't check, but construction from
    // the result might. We test wrapping within the same size.
    U8 val1 = 200;
    U8 val2 = 100;
    U8 result = val1 + val2;  // 300, wraps to 44
    assert(result == 44);
    std::cout << "  ✓ Unchecked arithmetic wraps\n";
    
    // Signed overflow (implementation defined, just test it compiles)
    I8 max_i8 = I8(I8::MAX);
    I8 signed_overflow = max_i8 + I8(1);
    (void)signed_overflow; // Use variable to suppress warning
    
    // Division by zero throws exception
    bool threw_div_zero = false;
    try {
        U32 a = 100;
        U32 b = 0;
        U32 result = a / b;
        (void)result; // Suppress unused warning
    } catch (const std::domain_error&) {
        threw_div_zero = true;
    }
    assert(threw_div_zero);
    
    // Hex printing
    U32 hex_val = 0xDEADBEEF;
    const char* hex_str = hex_val.to_hex();
    std::cout << "  Hex test: " << hex_val << " -> " << hex_str << "\n";
    
    // Print methods
    std::cout << "  Testing Print(): ";
    U32 print_test = 12345;
    print_test.Print(); // Actually call it
    
    std::cout << "  ✓ Edge cases handled\n";
}