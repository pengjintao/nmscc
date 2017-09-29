#pragma once

#include <nms/core/string.h>

namespace nms
{

#pragma region parse impl
NMS_API bool _parse_num(str& text, str style, u8&   val);
NMS_API bool _parse_num(str& text, str style, i8&   val);
NMS_API bool _parse_num(str& text, str style, u16&  val);
NMS_API bool _parse_num(str& text, str style, i16&  val);
NMS_API bool _parse_num(str& text, str style, u32&  val);
NMS_API bool _parse_num(str& text, str style, i32&  val);
NMS_API bool _parse_num(str& text, str style, u64&  val);
NMS_API bool _parse_num(str& text, str style, i64&  val);
NMS_API bool _parse_num(str& text, str style, f32&  val);
NMS_API bool _parse_num(str& text, str style, f64&  val);
NMS_API bool _parse_str(str& text, str style, str&  val);
NMS_API bool _parse_str(str& text, str style, bool& val);
#pragma endregion

namespace ns_parse
{

template<class T>
auto match_version(str text, str style, T& value, Tver<5>) -> decltype(T::parse(text, style, value)) {
    return T::parse(text, style, value);
}

template<class T>
auto match_version(str text, str /*style*/, T& value, Tver<4>) -> decltype(T::parse(text, value)) {
    return T::parse(text, value);
}

template<class T>
auto match_version(str text, str style, T& value, Tver<3>) -> decltype(_parse_num(text, style, value)) {
    return _parse_num(text, style, value);
}

template<class T>
auto match_version(str text, str style, T& value, Tver<2>) -> decltype(_parse_str(text, style, value)) {
    return _parse_str(text, style, value);
}

template<class T>
auto match_version(str text, str style, T& value, Tver<1>) -> decltype(Enum<T>(value), true) {
    return Enum<T>::parse(text, style, value);
}

}

template<class T>
bool str_cast(str text, T& value) {
    return ns_parse::match_version(text, {}, value, Tver<9>{});
}

template<class T, class ...U>
void parse(str text, str format, T& arg, U& ... args) {
}


}
