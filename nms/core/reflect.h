#pragma once

#include <nms/core/base.h>

namespace nms
{

template<typename T, u64 offset, typename Tname>
struct Tmember
{
    static auto name() {
        // .....::[name]_mt..
        auto& type_name = nms::$typeid<Tname>.name;

        auto  idx_end = type_name.count - 4;
        auto  idx_beg = idx_end - 1;

        while (type_name[idx_beg] != ':' && idx_beg != 0) {
            --idx_beg;
        }

        const auto ret = str(type_name.data + idx_beg + 1, idx_end - idx_beg);
        return ret;
    }

    template<typename U>
    static const T& value(const U& object) {
        return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(&object) + offset);
    }
};

template<class T>
struct Tmembers
{
    constexpr static auto $count = T::_$member_cnt;

    template<u32 I>
    using type = decltype(T::_get_member(Tu32<I>{}));
};


#define NMS_REFLECT_BEGIN                                                               \
private:                                                                                \
    void _dummy_for_reflect();                                                          \
    using Tthis = typename nms::Tfunc<decltype(&_dummy_for_reflect)>::Ttype;            \
    friend struct Tmembers<Tthis>;                                                      \
    template<class, u64, class> friend struct Tmember;                                  \
public:                                                                                 \
    constexpr static size_t _$member_idx = __COUNTER__ + 1

#define NMS_REFLECT_END                                                                 \
public:                                                                                 \
    constexpr static size_t _$member_cnt = __COUNTER__ - _$member_idx

#define NMS_REFLECT_MEMBER(member, name)                                                \
constexpr static auto _get_member(Tu32<__COUNTER__-_$member_idx>) {                     \
    return Tmember<T##member, offsetof(Tthis, member), struct member##_mt>{};           \
}

#define NMS_MEMBER(member)                                                              \
    T##member;                                                                          \
    private:    NMS_REFLECT_MEMBER(member, #member)                                     \
    public:     T##member member

#pragma endregion

}
