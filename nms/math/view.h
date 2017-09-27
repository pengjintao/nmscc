#pragma once

#include <nms/core.h>

namespace nms::math
{

#pragma region view
template<typename T>
auto _view_cast(const T& t, Tver<0> ) -> Scalar<T> {
    return { t };
}

template<typename T>
auto _view_cast(const T& t, Tver<1> ) -> typename T::Tview {
    return t;
}

template<typename T>
auto view_cast(const T&t) {
    return _view_cast(t, Tver<1>{});
}

template<typename X, typename Y, typename= typename X::Tview, typename = typename Y::Tview>
auto _view_test_xy(Tver<2>)  {
    return 0;
}

template<typename X, typename Y, typename = typename X::Tview, typename = $when<($is<$number, Y> || $is<bool, Y>)> >
auto _view_test_xy(Tver<1>)  {
    return 0;
}

template<typename X, typename Y, typename = $when_is<$number, X>, typename = typename Y::Tview>
auto _view_test_xy(Tver<0>)  {
    return 0;
}

template<typename X, typename Y>
auto view_test_xy() -> decltype(_view_test_xy<X, Y>(Tver<2>{})) {
    return 0;
}
#pragma endregion

#pragma region Parallel
template<typename F, typename ...T>
struct Parallel;

template<typename F, typename T>
struct Parallel<F, T>
{
    constexpr static const auto $rank = T::$rank;
    using Tview = Parallel;
    using Tsize = typename T::Tsize;
    using Tdims = Vec<Tsize, $rank>;

    T   vt;

    /* dims of the view */
    __declspec(property(get=get_dims)) Tdims dims;

    Tdims get_dims() const noexcept {
        return vt.dims;
    }

    template<typename ...I>
    __forceinline auto operator()(I ...idx) const noexcept {
        return F::run(vt(idx...));
    }
};

template<typename F, typename Tx, typename Ty>
struct Parallel<F, Tx, Ty>
{
    constexpr static const auto $rank = Tx::$rank | Ty::$rank;

    using Tview = Parallel;
    using Tsize = typename Tx::Tsize;
    using Tdims = Vec<Tsize, $rank>;

    Tx  vx;
    Ty  vy;

    /* dims of the view */
    __declspec(property(get=get_dims)) Tdims dims;

    Tdims get_dims() const noexcept {
        return select_idx<Tx::$rank != 0 ? 0 : 1>(vx.dims, vy.dims);
    }

    template<typename ...I>
    __forceinline auto operator()(I ...idx) const noexcept {
        return F::run(vx(idx...), vy(idx...));
    }
};

/* make Parallel<F(x)> */
template<typename F, typename X>
auto make_parallel(const X& x) {
    using Vx = decltype(view_cast(x));
    return Parallel<F, Vx>{ x };
}

/* make Parallel<F(A,B)> */
template<typename F, typename X, typename Y>
auto make_parallel(const X& x, const Y& y) {
    using Vx = decltype(view_cast(x));
    using Vy = decltype(view_cast(y));
    return Parallel<F, Vx, Vy>{ x, y };
}

#pragma endregion

#pragma region Reduce
template<typename F, typename ...T>
struct Reduce;

template<typename F, typename T>
struct Reduce<F, T>
{
    constexpr static const auto $rank = T::$rank - 1;

    using Tview = Reduce;
    using Tsize = typename T::Tsize;
    using Tdims = Vec<Tsize, T::$rank - 1>;

    T   vt;

    /* dims of the view */
    __declspec(property(get=get_dims)) Tdims dims;

    Tdims get_dims() const noexcept {
        return Reduce::_make_dims($index_seq<$rank>, vt.dims);
    }

    template<typename ...I>
    auto operator()(I ...ids) const {
        const auto  n = vt.dims[0];

        auto ret = vt(0, ids...);
        for (Tsize i = 1; i < n; ++i) {
            ret = F::run(ret, vt(i, ids...));
        }
        return ret;
    }

private:
    template<typename Tdims>
    static Vec<Tsize, 0> _make_dims(Tu32<>, const Tdims& /*dims*/) {
        return { 0 };
    }

    template<u32 ...I, typename Tdims>
    static Vec<Tsize, u32(sizeof...(I))> _make_dims(Tu32<I...>, const Tdims& dims) {
        return { dims[I + 1]... };
    }
};

template<typename F, typename X>
auto make_reduce(const X& x) -> Reduce<F, decltype(view_cast(x)) > {
    return { view_cast(x) };
}

#pragma endregion

#pragma region vline

template<typename T, u32 N>
struct Vline
{
    constexpr static const auto $rank = N;

    using Tview = Vline;
    using Tidxs = Vec<T, N>;
    using Tsize = u32;
    using Tdata = T;
    using Tdims = Vec<Tsize, $rank>;

    Tidxs   step;

    __declspec(property(get=get_dims)) Tdims dims;
    Tdims get_dims() const noexcept {
        return { 0 };
    }

    template<typename ...Tidx>
    Tdata operator()(Tidx ...idxs) const {
        static_assert(sizeof...(Tidx) == $rank,   "unexpect arguments count, should be `$rank`");
        return get_offsets($index_seq<$rank>, idxs...);
    }

private:
    template<u32 ...Idim, typename ...Tidx>
    Tdata get_offsets(Tu32<Idim...>, Tidx ...idxs) const {
        return sum((Tdata(step[Idim])*idxs)...);
    }
};

template<typename T, typename ...U>
constexpr auto vline(const T& t, const U& ...u) {
    return Vline<T, u32(1 + sizeof...(U))>({ T(t), T(u)... });
}
#pragma endregion

#pragma region veye

template<typename T>
struct Veye
{
public:
    using Tview = Veye;
    using Tsize = u32;
    using Tdims = Vec<Tsize, 0>;

    constexpr static const auto $rank = 0;

    __declspec(property(get=get_dims)) Tdims dims;
    Tdims get_dims() const noexcept {
        return { 0 };
    }

    template<typename ...I>
    T operator()(I ...i) const noexcept {
        static_assert($all_is<$int, I...>,  "unexpect type, shold be int");
        static constexpr auto N = u32(sizeof...(I));

        const T idx[] = { T(i)... };

        auto v = idx[0];
        for (u32 k = 1; k < N; ++k) {
            if (idx[k] != v) {
                return T(0);
            }
        }
        return T(1);
    }
};

template<typename T>
constexpr auto veye() {
    return Veye<T>{};
}
#pragma endregion

}
