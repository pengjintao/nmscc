#pragma once

#include <nms/core/trait.h>

namespace nms
{

#pragma region predef: format
struct FormatStyle;

template<class Tview>
inline void sformat_view(IString& buf, const FormatStyle& style, const Tview& view);
#pragma endregion

template<typename T, u32 ...Ns>
struct Vec;

template<class T>
struct Vec<T, 0>
{
    static constexpr auto $rank  = 0;
    static constexpr auto $size  = 0;
    static constexpr auto $count = 0;
    using Tdata = T;

    Tdata data;

    template<typename I> __forceinline Tdata&       operator[] (I)       noexcept { return data; }
    template<typename I> __forceinline const Tdata& operator[] (I) const noexcept { return data; }

};

template<typename T, u32 N>
struct Vec<T, N>
{
    static constexpr auto $rank  = 0;
    static constexpr auto $size  = N;
    static constexpr auto $count = N;
    using Tdata = T;

    Tdata data[$size];

    static Vec from_array(const Tdata(&array)[N]) {
        return from_seq(Tindex_seq<N>{}, array);
    }

    template<u32 ...I, typename Tarray>
    static Vec from_seq(Tu32<I...>, const Tarray& array) {
        return Vec{ { array[I]... } };
    }

    __declspec(property(get=get_count)) u32 count;
    constexpr u32 get_count() const {
        return $count;
    }

    template<typename I> __forceinline Tdata&       operator[] (I idx)       noexcept { return data[idx]; }
    template<typename I> __forceinline const Tdata& operator[] (I idx) const noexcept { return data[idx]; }

#pragma region format
    template<typename Tstring>
    void sformat(Tstring& outbuf, const FormatStyle& style) const {
        sformat_view(outbuf, style, *this);
    }
#pragma endregion
};

template<typename T, u32 N>
bool operator==(const Vec<T, N>& a, const Vec<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

template<typename T, u32 N>
bool operator!=(const Vec<T, N>& a, const Vec<T, N>& b) {
    return !(a == b);
}

#define NMS_VEC_OP(op)                                                          \
template<typename T, u32 N>                                                        \
__forceinline Vec<T, N> operator op(const Vec<T, N>& a, const Vec<T, N>& b) {   \
    Vec<T, N> c;                                                                \
    for (u32 i = 0; i < N; ++i) {                                               \
        c[i] = a[i] op b[i];                                                    \
    }                                                                           \
    return c;                                                                   \
}
NMS_VEC_OP(+)
NMS_VEC_OP(-)
NMS_VEC_OP(*)
NMS_VEC_OP(/ )
#undef  NMS_VEC_OP

/* --- vec alias --- */
using i8x1  = Vec<i8,  1>; using u8x1  = Vec<u8,  1>;
using i16x1 = Vec<i16, 1>; using u16x1 = Vec<u16, 1>;
using i32x1 = Vec<i32, 1>; using u32x1 = Vec<u32, 1>;
using i64x1 = Vec<i64, 1>; using u64x1 = Vec<u64, 1>;
using f32x1 = Vec<f32, 1>; using f64x1 = Vec<f64, 1>;

using i8x2  = Vec<i8,  2>; using u8x2  = Vec<u8,  2>;
using i16x2 = Vec<i16, 2>; using u16x2 = Vec<u16, 2>;
using i32x2 = Vec<i32, 2>; using u32x2 = Vec<u32, 2>;
using i64x2 = Vec<i64, 2>; using u64x2 = Vec<u64, 2>;
using f32x2 = Vec<f32, 2>; using f64x2 = Vec<f64, 2>;

using i8x3  = Vec<i8,  3>; using u8x3  = Vec<u8,  3>;
using i16x3 = Vec<i16, 3>; using u16x3 = Vec<u16, 3>;
using i32x3 = Vec<i32, 3>; using u32x3 = Vec<u32, 3>;
using i64x3 = Vec<i64, 3>; using u64x3 = Vec<u64, 3>;
using f32x3 = Vec<f32, 3>; using f64x3 = Vec<f64, 3>;

using i8x4  = Vec<i8 , 4>; using u8x4  = Vec<u8,  4>;
using i16x4 = Vec<i16, 4>; using u16x4 = Vec<u16, 4>;
using i32x4 = Vec<i32, 4>; using u32x4 = Vec<u32, 4>;
using i64x4 = Vec<i64, 4>; using u64x4 = Vec<u64, 4>;
using f32x4 = Vec<f32, 4>; using f64x4 = Vec<f64, 4>;

using i8x8  = Vec<i8,  8>; using u8x8  = Vec<u8,  8>;
using i16x8 = Vec<i16, 8>; using u16x8 = Vec<u16, 8>;
using i32x8 = Vec<i32, 8>; using u32x8 = Vec<u32, 8>;
using i64x8 = Vec<i64, 8>; using u64x8 = Vec<u64, 8>;
using f32x8 = Vec<f32, 8>; using f64x8 = Vec<f64, 8>;

#pragma endregion
}