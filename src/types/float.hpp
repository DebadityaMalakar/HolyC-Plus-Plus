#pragma once
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <limits>
#include <cmath>

namespace holycpp {

// Forward declarations
template<size_t> class UInt;
template<size_t> class SInt;

// Base floating-point type
template<size_t Bits>
class FInt {
public:
    using storage_type = 
        std::conditional_t<Bits == 32, float,
        double>;
    
protected:
    storage_type value;
    
public:
    // Constants
    static constexpr size_t BITS = Bits;
    static constexpr storage_type MIN = std::numeric_limits<storage_type>::lowest();
    static constexpr storage_type MAX = std::numeric_limits<storage_type>::max();
    static constexpr storage_type EPSILON = std::numeric_limits<storage_type>::epsilon();
    
    // Constructors
    FInt() : value(0.0) {}
    
    FInt(storage_type val) : value(val) {}
    
    // From other floating-point types
    template<size_t OtherBits>
    FInt(const FInt<OtherBits>& other) : value(static_cast<storage_type>(other.raw())) {}
    
    // From unsigned integer types
    template<size_t OtherBits>
    FInt(const UInt<OtherBits>& other) : value(static_cast<storage_type>(other.raw())) {}
    
    // From signed integer types
    template<size_t OtherBits>
    FInt(const SInt<OtherBits>& other) : value(static_cast<storage_type>(other.raw())) {}
    
    // From fundamental floating-point and integral types
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, storage_type>>>
    FInt(T val) : value(static_cast<storage_type>(val)) {}
    
    // Assignment operators
    FInt& operator=(storage_type val) {
        value = val;
        return *this;
    }
    
    FInt& operator=(const FInt& other) = default;
    
    template<typename T, typename = std::enable_if_t<
        !std::is_same_v<T, FInt> && !std::is_same_v<T, storage_type>>>
    FInt& operator=(T val) {
        value = static_cast<storage_type>(val);
        return *this;
    }
    
    // Conversion to fundamental type
    operator storage_type() const { return value; }
    
    // Get raw value
    storage_type raw() const { return value; }
    
    // Arithmetic operators
    FInt operator+(const FInt& other) const { return FInt(value + other.value); }
    FInt operator-(const FInt& other) const { return FInt(value - other.value); }
    FInt operator*(const FInt& other) const { return FInt(value * other.value); }
    FInt operator/(const FInt& other) const { 
        if (other.value == 0.0) {
            throw std::domain_error("Division by zero");
        }
        return FInt(value / other.value); 
    }
    FInt operator%(const FInt& other) const { 
        if (other.value == 0.0) {
            throw std::domain_error("Modulo by zero");
        }
        return FInt(std::fmod(value, other.value)); 
    }
    FInt operator-() const { return FInt(-value); }
    
    // Arithmetic with built-in types
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    FInt operator+(T other) const { return FInt(value + static_cast<storage_type>(other)); }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    FInt operator-(T other) const { return FInt(value - static_cast<storage_type>(other)); }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    FInt operator*(T other) const { return FInt(value * static_cast<storage_type>(other)); }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    FInt operator/(T other) const { 
        if (other == 0) {
            throw std::domain_error("Division by zero");
        }
        return FInt(value / static_cast<storage_type>(other)); 
    }
    
    // Comparison operators - with same type
    bool operator==(const FInt& other) const { return value == other.value; }
    bool operator!=(const FInt& other) const { return value != other.value; }
    bool operator<(const FInt& other) const { return value < other.value; }
    bool operator<=(const FInt& other) const { return value <= other.value; }
    bool operator>(const FInt& other) const { return value > other.value; }
    bool operator>=(const FInt& other) const { return value >= other.value; }
    
    // Comparison operators - with built-in types
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    bool operator==(T other) const { 
        return value == static_cast<storage_type>(other); 
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    bool operator!=(T other) const { 
        return value != static_cast<storage_type>(other); 
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    bool operator<(T other) const { 
        return value < static_cast<storage_type>(other); 
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    bool operator<=(T other) const { 
        return value <= static_cast<storage_type>(other); 
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    bool operator>(T other) const { 
        return value > static_cast<storage_type>(other); 
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    bool operator>=(T other) const { 
        return value >= static_cast<storage_type>(other); 
    }
    
    // Compound assignment
    FInt& operator+=(const FInt& other) { value += other.value; return *this; }
    FInt& operator-=(const FInt& other) { value -= other.value; return *this; }
    FInt& operator*=(const FInt& other) { value *= other.value; return *this; }
    FInt& operator/=(const FInt& other) { 
        if (other.value == 0.0) {
            throw std::domain_error("Division by zero");
        }
        value /= other.value; 
        return *this; 
    }
    FInt& operator%=(const FInt& other) { 
        if (other.value == 0.0) {
            throw std::domain_error("Modulo by zero");
        }
        value = std::fmod(value, other.value); 
        return *this; 
    }
    
    // Math functions
    FInt abs() const { return FInt(std::abs(value)); }
    FInt sqrt() const { return FInt(std::sqrt(value)); }
    FInt pow(const FInt& exponent) const { return FInt(std::pow(value, exponent.value)); }
    FInt sin() const { return FInt(std::sin(value)); }
    FInt cos() const { return FInt(std::cos(value)); }
    FInt tan() const { return FInt(std::tan(value)); }
    FInt floor() const { return FInt(std::floor(value)); }
    FInt ceil() const { return FInt(std::ceil(value)); }
    FInt round() const { return FInt(std::round(value)); }
    
    // Check for special values
    bool is_nan() const { return std::isnan(value); }
    bool is_inf() const { return std::isinf(value); }
    bool is_finite() const { return std::isfinite(value); }
    
    // HolyC-style methods
    void Print() const {
        std::cout << value << std::endl;
    }
    
    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const FInt& val) {
        return os << val.value;
    }
};

// Concrete floating-point types
using F32 = FInt<32>;  // Single precision (new to HolyC++)
using F64 = FInt<64>;  // Double precision (from HolyC)

// Type traits
template<typename T>
struct is_float_holyc : std::false_type {};

template<size_t B> struct is_float_holyc<FInt<B>> : std::true_type {};

template<typename T>
inline constexpr bool is_float_holyc_v = is_float_holyc<T>::value;

} // namespace holycpp