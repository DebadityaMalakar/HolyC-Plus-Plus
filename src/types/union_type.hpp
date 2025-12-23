#pragma once
#include <cstddef>
#include <type_traits>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "unsigned_int.hpp"
#include "signed_int.hpp"
#include "float.hpp"

namespace holycpp {

// ==================== U0 - Void Type ====================
struct U0 {
    constexpr U0() = default;
    ~U0() = default;
    U0(const U0&) = default;
    U0& operator=(const U0&) = default;
    
    static constexpr size_t SIZE = 0;
    static constexpr bool is_void() { return true; }
    
    static void Print() {
        std::cout << "U0 (void)" << std::endl;
    }
};

// ==================== Union Declaration Macros ====================
#define UNION_BEGIN(name) union name {
#define UNION_END() };
#define UNION(name, ...) union name { __VA_ARGS__ };

// ==================== Basic Union Implementation ====================
template<typename... Members>
struct CUnion;

template<typename T>
struct CUnion<T> {
    T value;
    operator T&() { return value; }
    operator const T&() const { return value; }
    T& get() { return value; }
    const T& get() const { return value; }
    void Print() const {
        if constexpr (std::is_same_v<T, U0>) {
            std::cout << "U0" << std::endl;
        } else {
            std::cout << value << std::endl;
        }
    }
};

template<typename T1, typename T2, typename... Rest>
struct CUnion<T1, T2, Rest...> {
    union {
        T1 first;
        T2 second;
        CUnion<Rest...> rest;
    };
    
    T1& as_first() { return first; }
    const T1& as_first() const { return first; }
    T2& as_second() { return second; }
    const T2& as_second() const { return second; }
    CUnion<Rest...>& as_rest() { return rest; }
    const CUnion<Rest...>& as_rest() const { return rest; }
    
    void Print() const {
        std::cout << "<C Union>" << std::endl;
    }
};

// ==================== HolyC Union Wrapper ====================
template<typename... Types>
class Union {
private:
    // Helper to compute max size at compile time
    static constexpr size_t max_size() {
        size_t sizes[] = {sizeof(Types)...};
        size_t max = 0;
        for (size_t s : sizes) {
            if (s > max) max = s;
        }
        return max;
    }
    
    static constexpr size_t max_align() {
        size_t aligns[] = {alignof(Types)...};
        size_t max = 0;
        for (size_t a : aligns) {
            if (a > max) max = a;
        }
        return max;
    }
    
    alignas(max_align()) unsigned char data[max_size()];
    int active_type = -1;
    
    // Fixed type index implementation
    template<typename T, typename First, typename... Rest>
    static constexpr int type_index_impl() {
        if constexpr (std::is_same_v<T, First>) {
            return 0;
        } else if constexpr (sizeof...(Rest) > 0) {
            return 1 + type_index_impl<T, Rest...>();
        } else {
            return -1;
        }
    }
    
    template<typename T>
    static constexpr int get_type_index() {
        return type_index_impl<T, Types...>();
    }
    
public:
    Union() = default;
    
    template<typename T>
    Union(const T& value) {
        set(value);
    }
    
    ~Union() {
        reset();
    }
    
    Union(const Union& other) {
        if (other.active_type >= 0) {
            copy_from(other);
        }
    }
    
    Union(Union&& other) noexcept {
        if (other.active_type >= 0) {
            move_from(std::move(other));
            other.active_type = -1;
        }
    }
    
    Union& operator=(const Union& other) {
        if (this != &other) {
            reset();
            if (other.active_type >= 0) {
                copy_from(other);
            }
        }
        return *this;
    }
    
    Union& operator=(Union&& other) noexcept {
        if (this != &other) {
            reset();
            if (other.active_type >= 0) {
                move_from(std::move(other));
                other.active_type = -1;
            }
        }
        return *this;
    }
    
    template<typename T>
    void set(const T& value) {
        static_assert((std::is_same_v<T, Types> || ...), "Type not in union");
        reset();
        new(data) T(value);
        active_type = get_type_index<T>();
    }
    
    template<typename T>
    T& get() {
        static_assert((std::is_same_v<T, Types> || ...), "Type not in union");
        if (active_type != get_type_index<T>()) {
            throw std::runtime_error("Wrong active type in union");
        }
        return *reinterpret_cast<T*>(data);
    }
    
    template<typename T>
    const T& get() const {
        static_assert((std::is_same_v<T, Types> || ...), "Type not in union");
        if (active_type != get_type_index<T>()) {
            throw std::runtime_error("Wrong active type in union");
        }
        return *reinterpret_cast<const T*>(data);
    }
    
    template<typename T>
    bool is() const {
        static_assert((std::is_same_v<T, Types> || ...), "Type not in union");
        return active_type == get_type_index<T>();
    }
    
    int active() const { return active_type; }
    
    void reset() {
        if (active_type >= 0) {
            destroy_active();
            active_type = -1;
        }
    }
    
    // Unsafe HolyC-style access - intentionally suppresses warnings
    // This mimics HolyC's unsafe union access behavior
    template<typename T>
    T as() const {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #pragma GCC diagnostic ignored "-Wclass-memaccess"
        return *reinterpret_cast<const T*>(data);
        #pragma GCC diagnostic pop
    }
    
    void Print() const {
        if (active_type < 0) {
            std::cout << "<empty union>" << std::endl;
            return;
        }
        print_active();
    }
    
    static constexpr size_t Size() {
        return max_size();
    }
    
    static constexpr size_t Alignment() {
        return max_align();
    }
    
private:
    void copy_from(const Union& other) {
        std::memcpy(data, other.data, Size());
        active_type = other.active_type;
        if (active_type >= 0) {
            construct_active();
        }
    }
    
    void move_from(Union&& other) {
        std::memcpy(data, other.data, Size());
        active_type = other.active_type;
        other.active_type = -1;
    }
    
    void destroy_active() {
        destroy_impl<0, Types...>();
    }
    
    template<int I, typename T, typename... Rest>
    void destroy_impl() {
        if (active_type == I) {
            reinterpret_cast<T*>(data)->~T();
        } else if constexpr (sizeof...(Rest) > 0) {
            destroy_impl<I + 1, Rest...>();
        }
    }
    
    void construct_active() {
        construct_impl<0, Types...>();
    }
    
    template<int I, typename T, typename... Rest>
    void construct_impl() {
        if (active_type == I) {
            new(data) T(*reinterpret_cast<const T*>(data));
        } else if constexpr (sizeof...(Rest) > 0) {
            construct_impl<I + 1, Rest...>();
        }
    }
    
    void print_active() const {
        print_impl<0, Types...>();
    }
    
    template<int I, typename T, typename... Rest>
    void print_impl() const {
        if (active_type == I) {
            if constexpr (std::is_same_v<T, U0>) {
                std::cout << "U0" << std::endl;
            } else {
                std::cout << *reinterpret_cast<const T*>(data) << std::endl;
            }
        } else if constexpr (sizeof...(Rest) > 0) {
            print_impl<I + 1, Rest...>();
        }
    }
};

// ==================== Common Union Types ====================
using Example = Union<I32, U8>;
using VoidUnion = Union<U0, I32, F64>;
using PtrUnion = Union<void*, I64, U64>;

// ==================== Anonymous Union Support ====================
template<typename... Types>
class AnonymousUnion {
private:
    static constexpr size_t max_size() {
        size_t sizes[] = {sizeof(Types)...};
        size_t max = 0;
        for (size_t s : sizes) {
            if (s > max) max = s;
        }
        return max;
    }
    
    union {
        unsigned char data[max_size()];
    };
    
public:
    template<typename T>
    T as() const {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #pragma GCC diagnostic ignored "-Wclass-memaccess"
        return *reinterpret_cast<const T*>(data);
        #pragma GCC diagnostic pop
    }
    
    template<typename T>
    void init(const T& value) {
        new(data) T(value);
    }
    
    static constexpr size_t size() {
        return max_size();
    }
};

// ==================== Value Class (HolyC Example) ====================
constexpr int FLOAT_TYPE = 0;
constexpr int CHAR_TYPE = 1;
constexpr int VALUE_TYPE = 2;

class Value {
public:
    int type;
    
    union {
        F64 f;
        U8 ch;
        Value *val;
        I32 i;
        U32 u;
    };
    
    Value() : type(-1), f(0.0) {}
    Value(F64 value) : type(FLOAT_TYPE), f(value) {}
    Value(U8 value) : type(CHAR_TYPE), ch(value) {}
    Value(Value* value) : type(VALUE_TYPE), val(value) {}
    Value(I32 value) : type(3), i(value) {}
    Value(U32 value) : type(4), u(value) {}
    
    F64 as_float() const {
        if (type != FLOAT_TYPE) {
            throw std::runtime_error("Value is not a float");
        }
        return f;
    }
    
    U8 as_char() const {
        if (type != CHAR_TYPE) {
            throw std::runtime_error("Value is not a char");
        }
        return ch;
    }
    
    Value* as_value_ptr() const {
        if (type != VALUE_TYPE) {
            throw std::runtime_error("Value is not a Value pointer");
        }
        return val;
    }
    
    // Unsafe access (HolyC style) - intentionally suppresses warnings
    // This mimics HolyC's unsafe union access behavior
    template<typename T>
    T as() const {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #pragma GCC diagnostic ignored "-Wclass-memaccess"
        return *reinterpret_cast<const T*>(&f);
        #pragma GCC diagnostic pop
    }
    
    void Print() const {
        switch (type) {
            case FLOAT_TYPE:
                std::cout << "Float: " << f << std::endl;
                break;
            case CHAR_TYPE:
                std::cout << "Char: '" << ch << "'" << std::endl;
                break;
            case VALUE_TYPE:
                std::cout << "Value pointer: " << val << std::endl;
                break;
            case 3:
                std::cout << "Int: " << i << std::endl;
                break;
            case 4:
                std::cout << "UInt: " << u << std::endl;
                break;
            default:
                std::cout << "<invalid Value>" << std::endl;
                break;
        }
    }
    
    bool is_float() const { return type == FLOAT_TYPE; }
    bool is_char() const { return type == CHAR_TYPE; }
    bool is_value_ptr() const { return type == VALUE_TYPE; }
    bool is_int() const { return type == 3; }
    bool is_uint() const { return type == 4; }
    
    void set_float(F64 value) { type = FLOAT_TYPE; f = value; }
    void set_char(U8 value) { type = CHAR_TYPE; ch = value; }
    void set_value_ptr(Value* value) { type = VALUE_TYPE; val = value; }
    void set_int(I32 value) { type = 3; i = value; }
    void set_uint(U32 value) { type = 4; u = value; }
};

// ==================== Memory Allocation Helpers ====================
template<typename T>
inline T* MNew() {
    return new T();
}

template<typename T>
inline T* MAlloc(size_t count = 1) {
    return static_cast<T*>(operator new(sizeof(T) * count));
}

inline void Free(void* ptr) {
    operator delete(ptr);
}

inline Value* MAllocValue() {
    return MAlloc<Value>();
}

} // namespace holycpp