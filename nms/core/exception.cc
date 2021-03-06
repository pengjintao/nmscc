#include <nms/test.h>
#include <nms/util/stackinfo.h>

namespace nms
{

NMS_API StackInfo& get_exception_stackinfo() noexcept {
    static thread_local StackInfo stack_info;
    return stack_info;
}

NMS_API void set_exception_stackinfo() noexcept {
    auto& global_info   = get_exception_stackinfo();
    auto  current_info  = StackInfo::backtrace(3);
    
#ifdef NMS_OS_WINDOWS
    if (current_info._count > 6) {
        current_info._count -= 6;
    }
#endif
    global_info = current_info;
}

#pragma region exception: system

NMS_API Esystem::Teid Esystem::_get_errno() {
    return errno;
}

NMS_API void Esystem::_get_errstr(Teid eid, char* outbuf, u32 max_cnt) {
#if defined(NMS_OS_WINDOWS)
    strerror_s(outbuf, max_cnt, eid);
#elif defined(NMS_OS_APPLE)
    // XSI Tver
    strerror_r(eid, outbuf, max_cnt);
#elif defined(NMS_OS_UNIX)
    // GNU Tver
    auto ptr = strerror_r(eid, outbuf, max_cnt);
    if (ptr != outbuf) {
        ::free(ptr);
    }
#endif
}

#pragma endregion

}
