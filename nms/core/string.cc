#include <nms/core.h>
#include <nms/test.h>

namespace nms
{

NMS_API u32 strlen(const char* s) {
    if (s == nullptr) {
        return 0u;
    }
    auto len = u32(::strlen(s));
    return len;
}

NMS_API u32 strlen(const char* s, u32 n) {
    if (s == nullptr) {
        return 0u;
    }
    auto len = u32(::strnlen(s, n));
    return len;
}

}
