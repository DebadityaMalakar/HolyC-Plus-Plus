#pragma once
#include "unsigned_int.hpp"
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <limits>

namespace holycpp {

// Base signed integer type
template<size_t Bits>
class SInt {
public:
    using storage_type = 
        std::conditional_t<Bits == 8, int8_t,
        std::conditional_t<Bits == 16, int16_t,
        std::conditional_t<Bits == 32, int32_t,
        int64_t>>>;
    
    using unsigned_type = UInt<Bits>;
    
protected:
    storage_type value;
    
    // Helper for bounds checking
    template<typename T>
    static void check_bounds(T val) {
        if constexpr (sizeof(T) > sizeof(storage_type) || std::is_unsigned_v<T>) {
            if (val < static_cast<T>(MIN) || val > static_cast<T>(MAX)) {
                throw std::out_of_range("Value out of range for this signed type");
            }
        }
    }
    
public:
    // Constants
    static constexpr size_t BITS = Bits;
    static constexpr storage_type MIN = 
        Bits == 8 ? INT8_MIN :
        Bits == 16 ? INT16_MIN :
        Bits == 32 ? INT32_MIN :
        INT64_MIN;
    static constexpr storage_type MAX = 
        Bits == 8 ? INT8_MAX :
        Bits == 16 ? INT16_MAX :
        Bits == 32 ? INT32_MAX :
        INT64_MAX;
    
    // Constructors
    SInt() : value(0) {}
    
    // From storage_type - no bounds checking
    explicit SInt(storage_type val) : value(val) {}
    
    // From unsigned (HolyC allows this) with bounds checking
    template<size_t OtherBits>
    SInt(const UInt<OtherBits>& other) {
        auto raw_val = other.raw();
        if (raw_val > static_cast<typename UInt<OtherBits>::storage_type>(MAX)) {
            throw std::out_of_range("Unsigned value too large for signed type");
        }
        value = static_cast<storage_type>(raw_val);
    }
    
    // From other signed types with bounds checking
    template<typename T, typename = std::enable_if_t<
        std::is_signed_v<T> && std::is_integral_v<T> && !std::is_same_v<T, storage_type>>>
    SInt(T val) {
        check_bounds(val);
        value = static_cast<storage_type>(val);
    }
    
    // From unsigned integral types with bounds checking
    template<typename T, typename = std::enable_if_t<
        std::is_unsigned_v<T> && std::is_integral_v<T>>, typename = void>
    SInt(T val) {
        if (val > static_cast<T>(MAX)) {
            throw std::out_of_range("Unsigned value too large for signed type");
        }
        value = static_cast<storage_type>(val);
    }
    
    // From pointers (HolyC style) - Fixed to avoid precision loss
    SInt(void* ptr) : value(static_cast<storage_type>(reinterpret_cast<intptr_t>(ptr))) {
        // Note: Pointer truncation is allowed but may lose data
    }
    
    // Assignment operators
    SInt& operator=(storage_type val) {
        value = val;
        return *this;
    }
    
    // Assignment from same type (no bounds checking needed)
    SInt& operator=(const SInt& other) = default;
    
    // Assignment from other types with bounds checking
    template<typename T, typename = std::enable_if_t<
        !std::is_same_v<T, SInt> && !std::is_same_v<T, storage_type>>>
    SInt& operator=(T val) {
        check_bounds(val);
        value = static_cast<storage_type>(val);
        return *this;
    }
    
    // Conversion to fundamental type
    operator storage_type() const { return value; }
    
    // Get raw value
    storage_type raw() const { return value; }
    
    // Get unsigned version
    unsigned_type as_unsigned() const { return unsigned_type(static_cast<typename unsigned_type::storage_type>(value)); }
    
    // Checked arithmetic operations
    SInt checked_add(const SInt& other) const {
        if ((other.value > 0 && value > MAX - other.value) ||
            (other.value < 0 && value < MIN - other.value)) {
            throw std::overflow_error("Signed addition overflow");
        }
        return SInt(value + other.value);
    }
    
    SInt checked_sub(const SInt& other) const {
        if ((other.value < 0 && value > MAX + other.value) ||
            (other.value > 0 && value < MIN + other.value)) {
            throw std::overflow_error("Signed subtraction overflow");
        }
        return SInt(value - other.value);
    }
    
    SInt checked_mul(const SInt& other) const {
        if (value > 0 && other.value > 0 && value > MAX / other.value) {
            throw std::overflow_error("Signed multiplication overflow");
        }
        if (value > 0 && other.value < 0 && other.value < MIN / value) {
            throw std::overflow_error("Signed multiplication overflow");
        }
        if (value < 0 && other.value > 0 && value < MIN / other.value) {
            throw std::overflow_error("Signed multiplication overflow");
        }
        if (value < 0 && other.value < 0 && value < MAX / other.value) {
            throw std::overflow_error("Signed multiplication overflow");
        }
        return SInt(value * other.value);
    }
    
    // Arithmetic operators (unchecked for performance)
    SInt operator+(const SInt& other) const { return SInt(static_cast<storage_type>(value + other.value)); }
    SInt operator-(const SInt& other) const { return SInt(static_cast<storage_type>(value - other.value)); }
    SInt operator*(const SInt& other) const { return SInt(static_cast<storage_type>(value * other.value)); }
    SInt operator/(const SInt& other) const { 
        if (other.value == 0) {
            throw std::domain_error("Division by zero");
        }
        if (value == MIN && other.value == -1) {
            throw std::overflow_error("Signed division overflow (MIN / -1)");
        }
        return SInt(static_cast<storage_type>(value / other.value)); 
    }
    SInt operator%(const SInt& other) const { 
        if (other.value == 0) {
            throw std::domain_error("Modulo by zero");
        }
        return SInt(static_cast<storage_type>(value % other.value)); 
    }
    SInt operator-() const { 
        if (value == MIN) {
            throw std::overflow_error("Negation of MIN value overflows");
        }
        return SInt(static_cast<storage_type>(-value)); 
    }
    
    // Bitwise operators
    SInt operator&(const SInt& other) const { return SInt(static_cast<storage_type>(value & other.value)); }
    SInt operator|(const SInt& other) const { return SInt(static_cast<storage_type>(value | other.value)); }
    SInt operator^(const SInt& other) const { return SInt(static_cast<storage_type>(value ^ other.value)); }
    SInt operator~() const { return SInt(static_cast<storage_type>(~value)); }
    SInt operator<<(const SInt& other) const { 
        if (other.value < 0 || other.value >= static_cast<storage_type>(BITS)) {
            throw std::out_of_range("Shift amount out of range");
        }
        return SInt(static_cast<storage_type>(value << other.value)); 
    }
    SInt operator>>(const SInt& other) const { 
        if (other.value < 0 || other.value >= static_cast<storage_type>(BITS)) {
            throw std::out_of_range("Shift amount out of range");
        }
        return SInt(static_cast<storage_type>(value >> other.value)); 
    }
    
    // Bitwise operators with built-in types
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    SInt operator<<(T shift) const {
        if (shift < 0 || shift >= static_cast<T>(BITS)) {
            throw std::out_of_range("Shift amount out of range");
        }
        return SInt(static_cast<storage_type>(value << shift));
    }
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    SInt operator>>(T shift) const {
        if (shift < 0 || shift >= static_cast<T>(BITS)) {
            throw std::out_of_range("Shift amount out of range");
        }
        return SInt(static_cast<storage_type>(value >> shift));
    }
    
    // Comparison operators - with same type
    bool operator==(const SInt& other) const { return value == other.value; }
    bool operator!=(const SInt& other) const { return value != other.value; }
    bool operator<(const SInt& other) const { return value < other.value; }
    bool operator<=(const SInt& other) const { return value <= other.value; }
    bool operator>(const SInt& other) const { return value > other.value; }
    bool operator>=(const SInt& other) const { return value >= other.value; }
    
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
    SInt& operator+=(const SInt& other) { value += other.value; return *this; }
    SInt& operator-=(const SInt& other) { value -= other.value; return *this; }
    SInt& operator*=(const SInt& other) { value *= other.value; return *this; }
    SInt& operator/=(const SInt& other) { 
        if (other.value == 0) {
            throw std::domain_error("Division by zero");
        }
        if (value == MIN && other.value == -1) {
            throw std::overflow_error("Signed division overflow (MIN / -1)");
        }
        value /= other.value; 
        return *this; 
    }
    SInt& operator%=(const SInt& other) { 
        if (other.value == 0) {
            throw std::domain_error("Modulo by zero");
        }
        value %= other.value; 
        return *this; 
    }
    SInt& operator&=(const SInt& other) { value &= other.value; return *this; }
    SInt& operator|=(const SInt& other) { value |= other.value; return *this; }
    SInt& operator^=(const SInt& other) { value ^= other.value; return *this; }
    SInt& operator<<=(const SInt& other) { 
        if (other.value < 0 || other.value >= static_cast<storage_type>(BITS)) {
            throw std::out_of_range("Shift amount out of range");
        }
        value <<= other.value; 
        return *this; 
    }
    SInt& operator>>=(const SInt& other) { 
        if (other.value < 0 || other.value >= static_cast<storage_type>(BITS)) {
            throw std::out_of_range("Shift amount out of range");
        }
        value >>= other.value; 
        return *this; 
    }
    
    // Increment/Decrement
    SInt& operator++() { ++value; return *this; }
    SInt operator++(int) { SInt temp = *this; ++value; return temp; }
    SInt& operator--() { --value; return *this; }
    SInt operator--(int) { SInt temp = *this; --value; return temp; }
    
    // HolyC-style methods
    const char* to_hex() const {
        // Use unsigned representation for hex
        return as_unsigned().to_hex();
    }
    
    void Print() const {
        std::cout << value << std::endl;
    }
    
    void PrintHex() const {
        std::cout << to_hex() << std::endl;
    }
    
    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const SInt& val) {
        return os << val.value;
    }
};

// Concrete signed types
using I8 = SInt<8>;
using I16 = SInt<16>;
using I32 = SInt<32>;
using I64 = SInt<64>;

// Type traits
template<typename T>
struct is_signed_holyc : std::false_type {};

template<size_t B> struct is_signed_holyc<SInt<B>> : std::true_type {};

template<typename T>
inline constexpr bool is_signed_holyc_v = is_signed_holyc<T>::value;

// Implementation of UInt constructor from SInt (defined after SInt is complete)
template<size_t Bits>
template<size_t OtherBits>
inline UInt<Bits>::UInt(const SInt<OtherBits>& other) {
    auto raw_val = other.raw();
    if (raw_val < 0) {
        throw std::out_of_range("Cannot assign negative signed value to unsigned type");
    }
    if constexpr (OtherBits > Bits) {
        check_bounds(static_cast<typename SInt<OtherBits>::storage_type>(raw_val));
    }
    value = static_cast<storage_type>(raw_val);
}

} // namespace holycpp