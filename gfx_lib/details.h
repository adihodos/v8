#pragma once

namespace gfx {

namespace implementation_details {

template<typename T>
inline T min(const T& left, const T& right) {
    return left < right ? left : right;
}

template<typename T, typename U>
struct types_eq {
    static const bool result = false;
};

template<typename T>
struct types_eq<T, T> {
    static const bool result = true;
};

template<typename real_t, bool is_floating_point = false>
struct transform_dividend_for_division {
    static real_t transform(real_t dividend) {
        return dividend;
    }
};

template<typename real_t>
struct transform_dividend_for_division<real_t, true> {
    static real_t transform(real_t dividend) {
        return real_t(1) / dividend;
    }
};

template<typename real_t, bool is_floating_point = false>
struct divide_helper {
    static real_t divide(real_t divided, real_t dividend) {
        return divided / dividend;
    }
};

template<typename real_t>
struct divide_helper<real_t, true> {
    static real_t divide(real_t divided, real_t inverse_of_dividend) {
        return divided * inverse_of_dividend;
    }
};

template<typename ty>
inline void swap(ty& left, ty& right) {
    ty temp = left;
    left = right;
    right = temp;
}

} // namespace details

} // namespace gfx
