#pragma once

#include <nms/core/base.h>

namespace nms
{

#pragma region Version
template<u32 Major, u32 Minor = 0>
struct Tver: public Tver<Major,   Minor-1>
{};

template<u32 Major>
struct Tver<Major, 0>: public Tver<Major-1, 0>
{};

template<>
struct Tver<0, 0>
{};
#pragma endregion

#pragma region Tval
template<class T, T ...v>
struct Tval
{
    constexpr static u32 $size = u32(sizeof...(v));
};

template<class T, T v>
struct Tval<T, v>
{
    constexpr static u32 $size  = 1;
    constexpr static T   $value = v;
};

template<u32  ...V> using Tu32  = Tval<u32, V...>;
template<i32  ...V> using Ti32  = Tval<i32, V...>;
template<bool ...V> using Tbool = Tval<bool, V...>;

namespace
{

template<i32  ...I> constexpr auto $i32  = Tval<i32, I...>{};
template<u32  ...I> constexpr auto $u32  = Tval<u32, I...>{};
template<bool ...I> constexpr auto $bool = Tval<bool, I...>{};

constexpr auto $0  = $i32< 0>;
constexpr auto $1  = $i32< 1>;
constexpr auto $2  = $i32< 2>;
constexpr auto $3  = $i32< 3>;
constexpr auto $4  = $i32< 4>;
constexpr auto $5  = $i32< 5>;
constexpr auto $6  = $i32< 6>;
constexpr auto $7  = $i32< 7>;
constexpr auto $8  = $i32< 8>;
constexpr auto $9  = $i32< 9>;
constexpr auto $10 = $i32<10>;
constexpr auto $11 = $i32<11>;
constexpr auto $12 = $i32<12>;
constexpr auto $13 = $i32<13>;
constexpr auto $14 = $i32<14>;
constexpr auto $15 = $i32<15>;
constexpr auto $16 = $i32<16>;
constexpr auto $17 = $i32<17>;
constexpr auto $18 = $i32<18>;
constexpr auto $19 = $i32<19>;
}
#pragma endregion

#pragma region repeat
template<u32 N, class T, T t, T ...V> struct _Trep;
template<u32 N, class T, T t, T ...V> struct _Trep                { using U = typename _Trep<N - 1, T, t, t, V...>::U; };
template<       class T, T t, T ...V> struct _Trep<0, T, t, V...> { using U = Tval<T, V...>; };

template<u32 N, class T=u32, T t=0> using Trep = typename _Trep<N, T, t>::U;
#pragma endregion

#pragma region index_seq
template<u32 N> using Tindex_seq = __make_integer_seq<Tval, u32, N>;

namespace
{
template<u32 N> constexpr auto $index_seq = __make_integer_seq<Tval, u32, N>{};
}
#pragma endregion

#pragma region idx op
template<class V>
constexpr auto idx_sum(Tu32<>, const V& v) {
    (void)v;
    return decltype(v[0]){0};
}

template<u32 ...I, class V>
constexpr auto idx_sum(Tu32<I...>, const V& v) {
    return sum(v[I]...);
}

template<class V>
constexpr auto iprod(Tu32<>, const V& v) {
    (void)v;
    return decltype(v[0]){1};
}

template<u32 ...I, class V>
constexpr auto iprod(Tu32<I...>, const V& v) {
    return prod(v[I]...);
}
#pragma endregion

#pragma region add/mul
template<class T, T ...V>           struct Tadd;
template<class T, T S>              struct Tadd<T, S>           : public Tval<T, S> {};
template<class T, T A, T B, T ...C> struct Tadd<T, A, B, C...>  : public Tadd<T, A + B, C...> {};

template<class T, T ...V>           struct Tmul;
template<class T, T S>              struct Tmul<T, S>           : public Tval<T, S> {};
template<class T, T A, T B, T ...C> struct Tmul<T, A, B, C...>  : public Tmul<T, A * B, C...> {};
#pragma endregion

#pragma region all/any/cnt
template<bool ...V>                 struct Tall;
template<bool S>                    struct Tall<S>          : public Tbool<S> {};
template<bool A, bool B, bool ...C> struct Tall<A, B, C...> : public Tall<A && B, C...> {};

template<bool ...V>                 struct Tany;
template<bool S>                    struct Tany<S>          : public Tbool<S> {};
template<bool A, bool B, bool ...C> struct Tany<A, B, C...> : public Tany<A || B, C...> {};

template<u32 S, bool ...V>          struct _Tcnt;
template<u32 S>                     struct _Tcnt<S>          : public Tu32<S> {};
template<u32 S, bool U, bool ...V>  struct _Tcnt<S, U, V...> : public _Tcnt<S+(U?1:0), V...> {};

template<bool ...V>
using Tcnt = _Tcnt<0, V...>;

namespace
{
template<bool ...V> constexpr auto $cnt = Tcnt<V...>::$value;
template<bool ...V> constexpr auto $all = Tall<V...>::$value;
template<bool ...V> constexpr auto $any = Tany<V...>::$value;
}

#pragma endregion

#pragma region index
template<u32 K, class  I, bool ...V> struct _Tindex;
template<u32 K, u32 ...I           > struct _Tindex<K, Tu32<I...>             > { using U = Tu32<I...>; };
template<u32 K, u32 ...I, bool ...V> struct _Tindex<K, Tu32<I...>, true,  V...> { using U = typename _Tindex<K + 1, Tu32<I..., K>, V...>::U; };
template<u32 K, u32 ...I, bool ...V> struct _Tindex<K, Tu32<I...>, false, V...> { using U = typename _Tindex<K + 1, Tu32<I...   >, V...>::U; };

template<bool ...V> using       Tindex_if  = typename _Tindex<0, Tu32<>, V...>::U;

namespace
{
template<bool ...V> constexpr   Tindex_if <V...> $index_if ={};
}
#pragma endregion

#pragma region select
template<class T, class ...U>
decltype(auto) _select_idx(Tu32<0>, T&& t, U&& .../*u*/) {
    return fwd<T>(t);
}

template<u32 I, class T, class ...U>
decltype(auto) _select_idx(Tu32<I>, T&& /*t*/, U&& ...u) {
    return _select_idx(Tu32<I - 1>{}, fwd<U>(u)...);
}

/*! select value at index=I */
template<u32 I, class ...T>
decltype(auto) select_idx(T&& ...t) {
    return _select_idx(Tu32<I>{}, fwd<T>(t)...);
}

#pragma endregion

#pragma region is
template<class T, class U>
struct Is
{
    static constexpr bool $value = __is_base_of(T, U);
};

template<class T>
struct Is<T, T>
{
    static constexpr bool $value = true;
};
/* test if T is U */
template<class T, class    U>
constexpr bool $is     = Is<T, U>::$value;

// check if all U is T
template<class T, class ...U>
constexpr bool $all_is = $all<Is<T, U>::$value...>;

// check if any U is T
template<class T, class ...U>
constexpr bool $any_is = $any<Is<T, U>::$value...>;

#pragma endregion

#pragma region as

template<class T, class ...U>
struct As
{
    static constexpr bool $value = __is_constructible(T, U...);
};

/* test if T as U */
template<class T, class ...U>
constexpr bool $as = As<T, U...>::$value;

// check if any U as T
template<class T, class ...U>
constexpr bool $all_as = $all<As<T, U>::$value...>;

// check if any U as T
template<class T, class ...U>
constexpr bool $any_as = $any<As<T, U>::$value...>;
#pragma endregion

#pragma region when
template<bool Xcond, class Ttype = void>
struct _$When;

template<class Ttype>
struct _$When<true, Ttype>
{
    using U = Ttype;
};

template<bool Xcond, class Ttype = void>
using $when = typename _$When<Xcond, Ttype>::U;

/* compile when U==T */
template<class Texpect, class Tvalue, class Ttype=void >
using $when_is = $when<Is<Texpect, Tvalue>::$value, Ttype>;

/* compile when U==T */
template<class T, class ...U>
using $when_as = $when<As<T, U...>::$value, void>;
#pragma endregion

#pragma region is: impl
struct $enum;       // check if type is enum
struct $class;      // check if type is class/struct
struct $pod;        // check if type is POD type
struct $union;      // check if type is union
struct $empty;      // check if type is empty
struct $copyable;   // check if type is copyable
struct $moveable;   // check if type is moveable;

template<class T>   struct Is<$enum ,       T> { static constexpr auto $value = __is_enum(T);                       };
template<class T>   struct Is<$class,       T> { static constexpr auto $value = __is_class(T);                      };
template<class T>   struct Is<$pod  ,       T> { static constexpr auto $value = __is_pod(T);                        };
template<class T>   struct Is<$union,       T> { static constexpr auto $value = __is_union(T);                      };
template<class T>   struct Is<$empty,       T> { static constexpr auto $value = __is_empty(T);                      };
template<class T>   struct Is<$copyable,    T> { static constexpr auto $value = __is_constructible(T, const T&);    };
template<class T>   struct Is<$moveable,    T> { static constexpr auto $value = __is_constructible(T, T&&);         };

struct $sint;       // check if type is signed integer
struct $uint;       // check if type is unsigned integer
struct $int;        // check if type is integer
struct $float;      // check if type is floating point
struct $number;     // check if type is number(integer or float)
template<class T>   struct Is<$sint,    T> { static constexpr auto $value = $is<i8,T>       || $is<i16,T>   || $is<i32,T>   || $is<i64,T>; };
template<class T>   struct Is<$uint,    T> { static constexpr auto $value = $is<u8,T>       || $is<u16,T>   || $is<u32,T>   || $is<u64,T>; };
template<class T>   struct Is<$int,     T> { static constexpr auto $value = $is<$sint,T>    || $is<$uint,T>;    };
template<class T>   struct Is<$float,   T> { static constexpr auto $value = $is<f32,T>      || $is<f64,T>;      };
template<class T>   struct Is<$number,  T> { static constexpr auto $value = $is<$int,T>     || $is<$float,T>;   };
#pragma endregion

#pragma region Tthis
template<class T>
struct Tfunc;

template<class R, class T, class ...X>
struct Tfunc<R(T::*)(X...)>
{
    using Tret  = R;
    using Ttype = T;
};
#pragma endregion

}