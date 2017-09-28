#pragma once

#include <nms/core.h>

namespace nms::test
{

template<class T>
void assert_eq(const T& a, const T& b) {
    if (!(a == b)) {
        NMS_THROW(Eunexpect<T>{a, b});
    }
}

template<class T>
void assert_neq(const T& a, const T& b) {
    if (a == b) {
        NMS_THROW(Eunexpect<T>{a, b});
    }
}

inline void assert_true(bool value) {
    if (!value) {
        NMS_THROW(Eunexpect<bool>{true, value});
    }
}

inline void assert_false(bool value) {
    if (value) {
        NMS_THROW(Eunexpect<bool>{false, value});
    }
}

inline void assert_eq(f32 a, f32 b, f32 delta = 1e-6f) {
    if (fabs(a) < 1e-30f && fabs(b) < 1e-30f) {
        return;
    }

    int ea = 0;
    int eb = 0;

    const auto sa = ::frexpf(a, &ea);
    const auto sb = ::frexpf(b, &eb);

    if (ea != eb) {
        if (ea > -10 || eb > -10) {
            NMS_THROW(Eunexpect<f32>{a, b});
        }
    }
    else {
        auto corr = sa - sb;
        if (corr < -delta || corr > +delta) {
            NMS_THROW(Eunexpect<f32>{a, b});
        }
    }
}

inline void assert_eq( f64 a, f64 b, f64 delta = 1e-12f) {
    int ea = 0;
    int eb = 0;

    auto sa = ::frexp(a, &ea);
    auto sb = ::frexp(b, &eb);

    if (ea != eb) {
        if (ea > -20 || eb > -20) {
            NMS_THROW(Eunexpect<f64>{a, b});
        }
    }
    else {
        auto corr = sa - sb;
        if (corr < -delta || corr > +delta) {
            NMS_THROW(Eunexpect<f64>{a, b});
        }
    }
}

template<class T, u32 N>
inline void assert_eq(const Vec<T, N>& a, const Vec<T, N>& b) {
    for (u32 i = 0; i < N; ++i) {
        assert_eq(a[i], b[i]);
    }
}

}
