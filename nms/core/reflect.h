#pragma once

#include <nms/core/base.h>
#include <nms/core/view.h>

namespace nms
{

/*! compile time typeid */
struct TypeId
{
    using Tname = const str&;
    using Tfunc = Tname(*)();

    Tfunc _func_get_name;

#pragma region constructors
    template<typename T>
    constexpr static TypeId make() {
        return { &TypeId::func_get_name<T> };
    }
#pragma endregion

#pragma region property
    Tname get_name() const {
        return _func_get_name();
    }
    __declspec(property(get=get_name)) Tname name;
#pragma endregion

#pragma region operator
    constexpr friend bool operator==(TypeId x, TypeId y) {
        return x._func_get_name == y._func_get_name;
    }

    constexpr friend bool operator!=(TypeId x, TypeId y) {
        return x._func_get_name != y._func_get_name;
    }
#pragma endregion

private:
    constexpr TypeId(Tfunc func)
        : _func_get_name(func)
    { }

    template<typename T>
    static Tname func_get_name() {
    #if defined(NMS_CC_MSVC)
        static constexpr u32 funcsig_head_size_ = u32(sizeof("const struct nms::View<char const, 0> &__cdecl nms::TypeId::func_get_name<")) - 1;
        static constexpr u32 funcsig_tail_size_ = u32(sizeof(">(void)")) - 1;
    #elif defined(NMS_CC_CLANG)
        static constexpr u32 funcsig_head_size_ = u32(sizeof("static const nms::View<const char>& nms::Type::_func_get_name() [T = ")) - 1;
        static constexpr u32 funcsig_tail_size_ = u32(sizeof("]")) - 1;
    #else
    #   error("unknow c++ compiler")
    #endif

        static const char*  full_name   = __PRETTY_FUNCTION__;
        static const char*  type_head   = full_name + funcsig_head_size_;
        static const u32    type_size   = u32(sizeof(__PRETTY_FUNCTION__)) - 1 - funcsig_head_size_ - funcsig_tail_size_;
        static const str    type_name   = {type_head, type_size};
        return type_name;
    }
};

/*! get compile-time typeid of type `T` */
template<typename T>
constexpr TypeId $typeid = TypeId::make<T>();

#pragma endregion


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
        return *reinterpret_cast<const T*>(reinterpret_cast<const byte*>(&object) + offset);
    }

    template<typename U>
    static T& value(U& object) {
        return *reinterpret_cast<T*>(reinterpret_cast<byte*>(&object) + offset);
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
