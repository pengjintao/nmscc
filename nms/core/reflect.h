#pragma once

#include <nms/core/base.h>

namespace nms
{

template<class ...T>
struct ReflectType
{

};

#define NMS_REFLECT_BEGIN                                                           \
private:                                                                            \
    void _this_type_helper();                                                       \
    using Tthis = typename nms::Tfunc<decltype(&_this_type_helper)>::Ttype;         \
public:                                                                             \
    constexpr static size_t _$member_idx = __COUNTER__ + 1

#define NMS_REFLECT_END                                                             \
public:                                                                             \
    constexpr static size_t _$member_cnt = __COUNTER__ - _$member_idx

#define NMS_REFLECT_MEMBER(member)                                                  \
struct _##member##_t{                                                               \
    using type = T##member;                                                         \
    constexpr static size_t offset() {                                              \
        return (size_t)(&(((Tthis*)nullptr)->member));                              \
    }                                                                               \
};                                                                                  \
static _##member##_t _get_member_type(Tu32<__COUNTER__-_$member_idx>) {             \
    return {};                                                                      \
}

#define NMS_MEMBER(member)                                                          \
    T##member;                                                                      \
    private:    NMS_REFLECT_MEMBER(member)                                          \
    public:     T##member member

#pragma endregion

}
