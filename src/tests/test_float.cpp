// filepath: c:\Hobby\HolyC++\src\tests\test_float.cpp
#include "../types/float.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace holycpp;

// Test function prototypes
void test_float_basics();
void test_float_construction();
void test_arithmetic();
void test_comparisons();
void test_math_functions();
void test_special_values();
void test_conversions();

int main() {
    std::cout << "🧪 Running HolyC++ Float Tests\n";
    std::cout << "==============================\n";
    
    try {
        test_float_basics();
        test_float_construction();
        test_arithmetic();
        test_comparisons();
        test_math_functions();
        test_special_values();
        test_conversions();
        
        std::cout << "\n✅ All float tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}

void test_float_basics() {
    std::cout << "\n🔹 Testing float basics...\n";
    
    // Default construction
    F32 f32_default;
    assert(f32_default == 0.0f);
    
    F64 f64_default;
    assert(f64_default == 0.0);
    
    // Value construction
    F32 f32_val = 3.14f;
    assert(f32_val == 3.14f);
    
    F64 f64_val = 3.14159265359;
    assert(f64_val == 3.14159265359);
    
    // Constants
    assert(F32::BITS == 32);
    assert(F64::BITS == 64);
    
    std::cout << "  ✓ Basic construction\n";
}

void test_float_construction() {
    std::cout << "\n🔹 Testing float construction...\n";
    
    // From float
    F32 f32_from_float = 2.71828f;
    assert(std::abs(f32_from_float.raw() - 2.71828f) < 0.0001f);
    
    // From double
    F64 f64_from_double = 2.71828182846;
    assert(std::abs(f64_from_double.raw() - 2.71828182846) < 0.0000001);
    
    // From integer
    F32 f32_from_int = 42;
    assert(f32_from_int == 42.0f);
    
    F64 f64_from_int = 100;
    assert(f64_from_int == 100.0);
    
    // Cross-conversion
    F32 small = 1.5f;
    F64 large = small;  // F32 -> F64
    assert(large == 1.5);
    
    std::cout << "  ✓ Construction from various types\n";
}

void test_arithmetic() {
    std::cout << "\n🔹 Testing arithmetic operations...\n";
    
    F32 a = 10.0f;
    F32 b = 3.0f;
    
    // Basic arithmetic
    assert(std::abs((a + b).raw() - 13.0f) < 0.0001f);
    assert(std::abs((a - b).raw() - 7.0f) < 0.0001f);
    assert(std::abs((a * b).raw() - 30.0f) < 0.0001f);
    assert(std::abs((a / b).raw() - 3.333f) < 0.01f);
    
    // Compound assignment
    F32 c = a;
    c += b;
    assert(std::abs(c.raw() - 13.0f) < 0.0001f);
    
    c = a;
    c -= b;
    assert(std::abs(c.raw() - 7.0f) < 0.0001f);
    
    c = a;
    c *= b;
    assert(std::abs(c.raw() - 30.0f) < 0.0001f);
    
    c = a;
    c /= b;
    assert(std::abs(c.raw() - 3.333f) < 0.01f);
    
    // Negation
    F32 neg = -a;
    assert(neg == -10.0f);
    
    // Modulo
    F32 x = 10.5f;
    F32 y = 3.0f;
    F32 mod_result = x % y;
    assert(std::abs(mod_result.raw() - 1.5f) < 0.0001f);
    
    std::cout << "  ✓ Arithmetic operations\n";
}

void test_comparisons() {
    std::cout << "\n🔹 Testing comparisons...\n";
    
    F32 small = 10.0f;
    F32 medium = 20.0f;
    F32 large = 30.0f;
    
    // Equality
    assert(small == 10.0f);
    assert(small != 20.0f);
    
    // Relational
    assert(small < medium);
    assert(medium > small);
    assert(small <= medium);
    assert(medium >= small);
    assert(large >= large);
    assert(medium <= medium);
    
    // Negative comparisons
    F32 neg = -10.0f;
    F32 pos = 10.0f;
    assert(neg < pos);
    assert(pos > neg);
    assert(neg < 0.0f);
    assert(pos > 0.0f);
    
    std::cout << "  ✓ Comparison operations\n";
}

void test_math_functions() {
    std::cout << "\n🔹 Testing math functions...\n";
    
    F64 val = 16.0;
    
    // Sqrt
    F64 sqrt_result = val.sqrt();
    assert(std::abs(sqrt_result.raw() - 4.0) < 0.0001);
    
    // Abs
    F64 neg_val = -5.5;
    F64 abs_result = neg_val.abs();
    assert(std::abs(abs_result.raw() - 5.5) < 0.0001);
    
    // Power
    F64 base = 2.0;
    F64 exp = 3.0;
    F64 pow_result = base.pow(exp);
    assert(std::abs(pow_result.raw() - 8.0) < 0.0001);
    
    // Trigonometric
    F64 angle = 0.0;  // sin(0) = 0, cos(0) = 1
    F64 sin_result = angle.sin();
    F64 cos_result = angle.cos();
    assert(std::abs(sin_result.raw() - 0.0) < 0.0001);
    assert(std::abs(cos_result.raw() - 1.0) < 0.0001);
    
    // Rounding
    F32 decimal = 3.7f;
    F32 floor_result = decimal.floor();
    F32 ceil_result = decimal.ceil();
    F32 round_result = decimal.round();
    assert(floor_result == 3.0f);
    assert(ceil_result == 4.0f);
    assert(round_result == 4.0f);
    
    std::cout << "  ✓ Math functions\n";
}

void test_special_values() {
    std::cout << "\n🔹 Testing special values...\n";
    
    // Normal values
    F32 normal = 3.14f;
    assert(normal.is_finite());
    assert(!normal.is_nan());
    assert(!normal.is_inf());
    
    // Infinity
    F32 inf = std::numeric_limits<float>::infinity();
    F32 inf_wrapped(inf);
    assert(inf_wrapped.is_inf());
    assert(!inf_wrapped.is_finite());
    
    // NaN
    F32 nan = std::numeric_limits<float>::quiet_NaN();
    F32 nan_wrapped(nan);
    assert(nan_wrapped.is_nan());
    assert(!nan_wrapped.is_finite());
    
    // Division by zero throws
    bool threw_div_zero = false;
    try {
        F32 a = 1.0f;
        F32 b = 0.0f;
        F32 result = a / b;
        (void)result;
    } catch (const std::domain_error&) {
        threw_div_zero = true;
    }
    assert(threw_div_zero);
    
    std::cout << "  ✓ Special values handled\n";
}

void test_conversions() {
    std::cout << "\n🔹 Testing type conversions...\n";
    
    // F32 to F64
    F32 f32_val = 1.5f;
    F64 f64_val = f32_val;
    assert(f64_val == 1.5);
    
    // F64 to F32
    F64 f64_large = 2.71828182846;
    F32 f32_from_f64 = f64_large;
    assert(std::abs(f32_from_f64.raw() - 2.71828f) < 0.0001f);
    
    // From integers
    F64 from_int = 42;
    assert(from_int == 42.0);
    
    // Implicit conversion via operators
    F32 a = 5.0f;
    F32 b = a + 3;  // int -> F32
    assert(b == 8.0f);
    
    F32 c = a * 2.0;  // double -> F32
    assert(c == 10.0f);
    
    std::cout << "  ✓ Type conversions\n";
}