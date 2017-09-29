#pragma once

#include <nms/core/string.h>

namespace nms
{

#pragma region format: pre-define
struct FormatStyle;

NMS_API void _sformat_val(IString& outbuf, const FormatStyle& style,str value);
#pragma endregion

template<class T>
struct Enum
{
    T value;

#pragma region property
    /*! the name of the enum */
    using Tname = str;
    __declspec(property(get=get_name)) Tname name;

    Tname get_name() const {
        static const auto& names= _make_enum_names(static_cast<T>(0));

        const auto  idx = static_cast<u32>(value);
        if (idx >= numel(names)) {
            return {};
        }
        const auto& val = names[idx];
        return val;
    }

#pragma endregion

#pragma region format/parse
    void sformat(IString& buf, const FormatStyle& style) const {
        auto& name = this->name;
        nms::_sformat_val(buf, style, name);
    }

    static bool parse(str text, str /*fmt*/, Enum& val) {
        const auto& names_arr = _make_enum_names(static_cast<T>(0));
        const auto  names_cnt = nms::numel(names_arr);

        for (u32 i = 0; i < names_cnt; ++i) {
            const auto& name = names_arr[i];
            if (name == text) {
                val = T(i);
                return true;
            }
        }
        return false;
    }
#pragma endregion

};

#define _NMS_ENUM_NAME(value)   #value

#if defined(NMS_CC_MSVC) && !defined(NMS_CC_CLANG)
#define  NMS_ENUM_NAMES(...)    _NMS_ENUM_NAMES __VA_ARGS__
#define _NMS_ENUM_NAMES(...)    NMSCPP_FOR(_NMS_ENUM_NAME, __VA_ARGS__)
#else
#define  NMS_ENUM_NAMES(...)    NMSCPP_FOR(_NMS_ENUM_NAME, __VA_ARGS__)
#endif

#define NMS_ENUM(type, ...)                                                         \
enum class type { __VA_ARGS__ };                                                    \
inline auto&  _make_enum_names(type) {                                              \
    static const ::nms::str names[] ={ NMS_ENUM_NAMES((__VA_ARGS__)) };             \
    return names;                                                                   \
}

#define NMS_ENUM_TYPE(type, alias,...)                                              \
enum class type: alias { __VA_ARGS__ };                                             \
inline auto& _make_enum_names(type) {                                               \
    static const auto names[] ={ NMS_ENUM_NAMES((__VA_ARGS__)) };                   \
    return names;                                                                   \
}

}
