#pragma once

#include <nms/config.h>

namespace nms
{

#pragma region builtin types
using   i8          = int8_t;       using   u8      = uint8_t;
using   i16         = int16_t;      using   u16     = uint16_t;
using   i32         = int32_t;      using   u32     = uint32_t;
using   i64         = int64_t;      using   u64     = uint64_t;

using   byte        = int8_t;
using   ubyte       = uint8_t;
using   f32         = float;
using   f64         = double;

using   uchar       = unsigned char;
using   ushort      = unsigned short;
using   uint        = unsigned int;
using   ulong       = unsigned long;
using   llong       = long long;
using   ullong      = unsigned long long;
using   nullptr_t   = decltype(nullptr);

#pragma endregion

#pragma region predef types
template<typename T, u32 N = 0>
struct View;

template<typename T>
struct IList;

using StrView   = View<const char>;
using IString   = IList<char>;
using str       = View<const char>;
#pragma endregion

#pragma region type modifiers
template<typename T> struct _Tconst            { using U = const T; };
template<typename T> struct _Tconst<const T>   { using U = const T; };

template<typename T> struct _Tmutable          { using U = T;       };
template<typename T> struct _Tmutable<const T> { using U = T;       };

template<typename T> struct _Tvalue            { using U = T;       };
template<typename T> struct _Tvalue<T&>        { using U = T;       };
template<typename T> struct _Tvalue<T&&>       { using U = T;       };

template<typename T> using Tconst      = typename _Tconst<T>::U;
template<typename T> using Tmutable    = typename _Tmutable<T>::U;
template<typename T> using Tvalue      = typename _Tvalue<T>::U;

template<typename T>
__forceinline T& mutable_cast(const T& v) noexcept {
    return const_cast<T&>(v);
}

template<bool X, typename T, typename U>
struct _Tcond;

template<typename A, typename B> struct _Tcond<true,  A, B> { using U = A; };
template<typename A, typename B> struct _Tcond<false, A, B> { using U = B; };

template<bool  X, typename A, typename B=void>
using Tcond = typename _Tcond<X, A, B>::U;
#pragma endregion

#pragma region type cast
template<typename T> constexpr auto  declptr() { using P = Tvalue<T>*; return  P{}; }
template<typename T> constexpr auto& declval() { using P = Tvalue<T>*; return *P{}; }

template<typename T> constexpr T*  $declptr = nullptr;
template<typename T> constexpr T&  $declval = *static_cast<T*>(nullptr);

template<typename T> __forceinline constexpr T&& fwd(Tvalue<T>&  t) noexcept { return (static_cast<T&&>(t)); }
template<typename T> __forceinline constexpr T&& fwd(Tvalue<T>&& t) noexcept { return (static_cast<T&&>(t)); }

#ifdef NMS_STDCXX_UTILITY
using std::move;
#else
template<typename T> __forceinline constexpr T&& move(T&  val) noexcept { return static_cast<T&&>(val); }
template<typename T> __forceinline constexpr T&& move(T&& val) noexcept { return static_cast<T&&>(val); }
#endif

#pragma endregion

#pragma region array
/* get elements count of this array */
template<typename T, u32 N>
__forceinline constexpr u32 numel(const T(&/*array*/)[N]) {
    return N;
}
#pragma endregion

#pragma region math: reduce
template<typename T>
constexpr auto (sum)(T t) {
    return t;
}

template<typename T, typename U, typename ...S>
constexpr auto (sum)(T t, U u, S ...s) {
    return t + (sum)(u, s...);
}

template<typename T>
constexpr auto (prod)(T t) {
    return t;
}

template<typename T, typename U, typename ...S>
constexpr auto (prod)(T t, U u, S ...s) {
    return t * (prod)(u, s...);
}

template<typename T>
constexpr auto (max)(T t) {
    return t;
}

template<typename T, typename U, typename ...S>
constexpr auto (max)(T t, U u, S ...s) {
    return t > (max)(u, s...) ? t : (max)(u, s...);
}

template<typename T>
constexpr auto (min)(T t) {
    return t;
}

template<typename T, typename U, typename ...S>
constexpr auto (min)(T t, U u, S ...s) {
    return t < (min)(u, s...) ? t : (min)(u, s...);
}

#pragma endregion

#pragma region typeid

#pragma region exception
struct Iexception
{
    virtual ~Iexception() = default;

    virtual void sformat(IString& outbuf) const = 0;
};
#pragma endregion

#pragma region static init
template<typename Tret, Tret(*func)(), typename Tid>
struct StaticInitor
{
    StaticInitor() {
        (void)value;
    }

    static Tret value;
};

template<typename Tret, Tret(*func)(), typename Tid>
Tret StaticInitor<Tret, func, Tid>::value = (*func)();

template<typename Tid, u32(*func)()>
u32 static_init() {
    StaticInitor<u32, func, Tid> static_initor;
    return static_initor.value;
}
#pragma endregion

}
