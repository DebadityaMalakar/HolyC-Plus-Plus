#pragma once
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <limits>

namespace holycpp {

// Forward declarations
template<size_t> class SInt;  // Forward declare SInt template
class HBool; class HStr; class U0;

// Base unsigned integer type
template<size_t Bits>
class UInt {
public:
    using storage_type = 
        std::conditional_t<Bits == 8, uint8_t,
        std::conditional_t<Bits == 16, uint16_t,
        std::conditional_t<Bits == 32, uint32_t,
        uint64_t>>>;
    
protected:
    storage_type value;
    
    // Helper for bounds checking
    template<typename T>
    static void check_bounds(T val) {
        if constexpr (std::is_signed_v<T>) {
            // Negative values wrap to large unsigned values
            if (val < 0) {
                throw std::out_of_range("Cannot assign negative value to unsigned type");
            }
        }
        if constexpr (sizeof(T) > sizeof(storage_type)) {
            if (val > static_cast<T>(MAX)) {
                throw std::out_of_range("Value exceeds maximum for this unsigned type");
            }
        }
    }
    
public:
    // Constants
    static constexpr size_t BITS = Bits;
    static constexpr storage_type MIN = 0;
    static constexpr storage_type MAX = 
        Bits == 8 ? UINT8_MAX :
        Bits == 16 ? UINT16_MAX :
        Bits == 32 ? UINT32_MAX :
        UINT64_MAX;
    
    // Constructors
    UInt() : value(0) {}
    
    UInt(storage_type val) : value(val) {}
    
    // Implicit conversions from other unsigned types with bounds checking
    template<size_t OtherBits>
    UInt(const UInt<OtherBits>& other) {
        if constexpr (OtherBits > Bits) {
            check_bounds(other.raw());
        }
        value = static_cast<storage_type>(other.raw());
    }
    
    // From signed HolyC++ types (with bounds checking)
    // Implemented in signed_int.hpp after SInt is defined
    template<size_t OtherBits>
    UInt(const SInt<OtherBits>& other);
    
    // From signed with bounds checking
    template<typename T, typename = std::enable_if_t<
        std::is_signed_v<T> && std::is_integral_v<T> && !std::is_same_v<T, storage_type>>>
    UInt(T val) {
        check_bounds(val);
        value = static_cast<storage_type>(val);
    }
    
    // From unsigned integral types with bounds checking
    template<typename T, typename = std::enable_if_t<
        std::is_unsigned_v<T> && std::is_integral_v<T> && !std::is_same_v<T, storage_type>>, typename = void>
    UInt(T val) {
        check_bounds(val);
        value = static_cast<storage_type>(val);
    }
    
    // From pointers (HolyC style) - Fixed to avoid precision loss
    UInt(void* ptr) : value(static_cast<storage_type>(reinterpret_cast<uintptr_t>(ptr))) {
        // Note: Pointer truncation is allowed but may lose data
    }
    
    // Assignment operators
    UInt& operator=(storage_type val) {
        value = val;
        return *this;
    }
    
    // Assignment from same type (no bounds checking needed)
    UInt& operator=(const UInt& other) = default;
    
    // Assignment from other types with bounds checking
    template<typename T, typename = std::enable_if_t<
        !std::is_same_v<T, UInt> && !std::is_same_v<T, storage_type>>>
    UInt& operator=(T val) {
        check_bounds(val);
        value = static_cast<storage_type>(val);
        return *this;
    }
    
    // Conversion to fundamental type
    operator storage_type() const { return value; }
    
    // Get raw value
    storage_type raw() const { return value; }
    
    // Checked arithmetic operations
    UInt checked_add(const UInt& other) const {
        if (value > MAX - other.value) {
            throw std::overflow_error("Unsigned addition overflow");
        }
        return UInt(value + other.value);
    }
    
    UInt checked_sub(const UInt& other) const {
        if (value < other.value) {
            throw std::underflow_error("Unsigned subtraction underflow");
        }
        return UInt(value - other.value);
    }
    
    UInt checked_mul(const UInt& other) const {
        if (other.value != 0 && value > MAX / other.value) {
            throw std::overflow_error("Unsigned multiplication overflow");
        }
        return UInt(value * other.value);
    }
    
    // Arithmetic operators (unchecked for performance, HolyC-style)
    UInt operator+(const UInt& other) const { return UInt(static_cast<storage_type>(value + other.value)); }
    UInt operator-(const UInt& other) const { return UInt(static_cast<storage_type>(value - other.value)); }
    UInt operator*(const UInt& other) const { return UInt(static_cast<storage_type>(value * other.value)); }
    UInt operator/(const UInt& other) const { 
        if (other.value == 0) {
            throw std::domain_error("Division by zero");
        }
        return UInt(static_cast<storage_type>(value / other.value)); 
    }
    UInt operator%(const UInt& other) const { 
        if (other.value == 0) {
            throw std::domain_error("Modulo by zero");
        }
        return UInt(static_cast<storage_type>(value % other.value)); 
    }
    
    // Bitwise operators
    UInt operator&(const UInt& other) const { return UInt(static_cast<storage_type>(value & other.value)); }
    UInt operator|(const UInt& other) const { return UInt(static_cast<storage_type>(value | other.value)); }
    UInt operator^(const UInt& other) const { return UInt(static_cast<storage_type>(value ^ other.value)); }
    UInt operator~() const { return UInt(static_cast<storage_type>(~value)); }
    UInt operator<<(const UInt& other) const { 
        if (other.value >= BITS) {
            throw std::out_of_range("Shift amount exceeds bit width");
        }
        return UInt(static_cast<storage_type>(value << other.value)); 
    }
    UInt operator>>(const UInt& other) const { 
        if (other.value >= BITS) {
            throw std::out_of_range("Shift amount exceeds bit width");
        }
        return UInt(static_cast<storage_type>(value >> other.value)); 
    }
    
    // Bitwise operators with built-in types
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    UInt operator<<(T shift) const {
        if (shift >= static_cast<T>(BITS)) {
            throw std::out_of_range("Shift amount exceeds bit width");
        }
        return UInt(static_cast<storage_type>(value << shift));
    }
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    UInt operator>>(T shift) const {
        if (shift >= static_cast<T>(BITS)) {
            throw std::out_of_range("Shift amount exceeds bit width");
        }
        return UInt(static_cast<storage_type>(value >> shift));
    }
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    UInt operator%(T other) const {
        if (other == 0) {
            throw std::domain_error("Modulo by zero");
        }
        return UInt(static_cast<storage_type>(value % other));
    }
    
    // Comparison operators - with same type
    bool operator==(const UInt& other) const { return value == other.value; }
    bool operator!=(const UInt& other) const { return value != other.value; }
    bool operator<(const UInt& other) const { return value < other.value; }
    bool operator<=(const UInt& other) const { return value <= other.value; }
    bool operator>(const UInt& other) const { return value > other.value; }
    bool operator>=(const UInt& other) const { return value >= other.value; }
    
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
    UInt& operator+=(const UInt& other) { value += other.value; return *this; }
    UInt& operator-=(const UInt& other) { value -= other.value; return *this; }
    UInt& operator*=(const UInt& other) { value *= other.value; return *this; }
    UInt& operator/=(const UInt& other) { 
        if (other.value == 0) {
            throw std::domain_error("Division by zero");
        }
        value /= other.value; 
        return *this; 
    }
    UInt& operator%=(const UInt& other) { 
        if (other.value == 0) {
            throw std::domain_error("Modulo by zero");
        }
        value %= other.value; 
        return *this; 
    }
    UInt& operator&=(const UInt& other) { value &= other.value; return *this; }
    UInt& operator|=(const UInt& other) { value |= other.value; return *this; }
    UInt& operator^=(const UInt& other) { value ^= other.value; return *this; }
    UInt& operator<<=(const UInt& other) { 
        if (other.value >= BITS) {
            throw std::out_of_range("Shift amount exceeds bit width");
        }
        value <<= other.value; 
        return *this; 
    }
    UInt& operator>>=(const UInt& other) { 
        if (other.value >= BITS) {
            throw std::out_of_range("Shift amount exceeds bit width");
        }
        value >>= other.value; 
        return *this; 
    }
    
    // Increment/Decrement
    UInt& operator++() { ++value; return *this; }
    UInt operator++(int) { UInt temp = *this; ++value; return temp; }
    UInt& operator--() { --value; return *this; }
    UInt operator--(int) { UInt temp = *this; --value; return temp; }
    
    // HolyC-style methods
    const char* to_hex() const {
        // Returns hex representation (simplified)
        static thread_local char buffer[20];
        if constexpr (Bits <= 8) {
            snprintf(buffer, sizeof(buffer), "0x%02X", value);
        } else if constexpr (Bits <= 16) {
            snprintf(buffer, sizeof(buffer), "0x%04X", value);
        } else if constexpr (Bits <= 32) {
            snprintf(buffer, sizeof(buffer), "0x%08X", value);
        } else {
            snprintf(buffer, sizeof(buffer), "0x%016llX", (unsigned long long)value);
        }
        return buffer;
    }
    
    void Print() const {
        std::cout << value << std::endl;
    }
    
    void PrintHex() const {
        std::cout << to_hex() << std::endl;
    }
    
    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const UInt& val) {
        return os << val.value;
    }
};

// Concrete unsigned types
using U8 = UInt<8>;
using U16 = UInt<16>;
using U32 = UInt<32>;
using U64 = UInt<64>;

// Type traits
template<typename T>
struct is_unsigned_holyc : std::false_type {};

template<size_t B> struct is_unsigned_holyc<UInt<B>> : std::true_type {};

template<typename T>
inline constexpr bool is_unsigned_holyc_v = is_unsigned_holyc<T>::value;

} // namespace holycpp