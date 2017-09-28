#pragma once

#include <nms/core/list.h>

namespace nms
{

template<u32 Icapacity=0 > using U8String   = List<char,     Icapacity>;
template<u32 Icapacity=0 > using U16String  = List<char16_t, Icapacity>;
template<u32 Icapacity=0 > using Tu32String = List<char32_t, Icapacity>;
template<u32 Icapacity=0 > using String     = U8String<Icapacity>;

NMS_API u32 strlen(const char* s);
NMS_API u32 strlen(const char* s, u32 n);

template<u32 N>
u32 strlen(const char(&s)[N]) {
    return strlen(s, N);
}

inline str make_str(const char* s) {
    return str(s, strlen(s));
}

}
