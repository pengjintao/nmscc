#include <nms/test.h>
#include <nms/util/stackinfo.h>
#include <nms/util/library.h>

#ifdef NMS_CC_MSVC
using namespace nms;

extern "C" {
    void*   GetCurrentProcess();
    ushort  RtlCaptureStackBackTrace(u32 FramesToSkip, u32 FramesToCapture, void** BackTrace, u32* BackTraceHash);

    int     SymInitialize(void* hProcess, const char* path, int fInvadeProcess);
    int     SymFromAddr(void* hProcess, u64 Address, u64* Displacement, struct _SYMBOL_INFO* Symbol);
}

struct _SYMBOL_INFO
{
    u32     SizeOfStruct;
    u32     TypeIndex;        // Type Index of symbol
    u64     Reserved[2];
    u32     Index;
    u32     Size;
    u64     pModBase;          // Base Address of module comtaining this symbol
    u32     Flags;
    u64     Value;            // Value of symbol, ValuePresent should be 1
    u64     Address;          // Address of symbol including base address of module
    u32     Register;         // register holding value or pointer to value
    u32     Scope;            // scope of the symbol
    u32     Tag;              // pdb classification
    u32     NameLen;          // Actual length of name
    u32     MaxNameLen;
    union {
        char    Name[1];      // Name of symbol
        char    dli_sname[1]; // Name of symbol
    };
};

using Dl_info = _SYMBOL_INFO;

static auto getCurrentProcess() {
    static auto proc = GetCurrentProcess();
    return proc;
}

static auto gDbgHelpLibrary(str name) {
    static Library dbghelp_lib("DbgHelp.dll");
    auto func = dbghelp_lib[name];
    return func;
}

#define NMS_DBGHELP_FUNC(name) static_cast<decltype(name)*>(gDbgHelpLibrary(#name))

static __forceinline auto backtrace(void** stacks, int count) {
    static auto proc        = getCurrentProcess();
    static auto sym_init    = NMS_DBGHELP_FUNC(SymInitialize);
    static auto init        = sym_init(proc, nullptr, true);  (void)init;

    auto ret = RtlCaptureStackBackTrace(1u, u32(count), stacks, nullptr);
    return ret;
}

static auto dladdr(void* handle, Dl_info* info) {
    static auto proc            = getCurrentProcess();
    info->MaxNameLen            = 512;
    info->SizeOfStruct          = sizeof(_SYMBOL_INFO);
    static auto pSymFromAddr    = NMS_DBGHELP_FUNC(SymFromAddr);
    auto ret = pSymFromAddr(proc, reinterpret_cast<u64>(handle), nullptr, info);

    return ret;
}
#endif

namespace nms
{

NMS_API void StackInfo::_backtrace() {
    const auto cnt = this->$capacity;
    const auto ret = ::backtrace(_stacks, i32(cnt));
    this->_count   = u32(ret);
}

NMS_API void StackInfo::Frame::sformat(IString& outbuf) const {
    if (this->ptr == nullptr) {
        outbuf += str("<null>");
        return;
    }

    struct {
        Dl_info info;
        char    buff[512];
    } info_ext;

    const auto ret = dladdr(this->ptr, &info_ext.info);
    if (ret == 0) {
        outbuf += str("<unknow>");
        return;
    }
    const auto name = str{ info_ext.info.dli_sname, u32(strlen(info_ext.info.dli_sname)) };

#ifdef NMS_CC_MSVC
    outbuf += name;
#else
    char out_name[4096];
    size_t length = sizeof(out_name);
    int status = 0;
    auto cxx_buff = abi::__cxa_demangle(name.data(), out_name, &length, &status);

    if (status == 0) {
        outbuf += str{ cxx_buff, strlen(cxx_buff) };

        if (cxx_buff != out_name) {
            ::free(cxx_buff);
        }
    }
    else if (cxx_buff != nullptr) {
        outbuf += str{ cxx_buff, strlen(cxx_buff) };
    }
    else {
        outbuf += str("<empty>");
    }
#endif
}

NMS_API void StackInfo::sformat(IString& outbuf) const {
    const auto frame_cnt = this->count;

    for (u32 frame_idx = 0; frame_idx < frame_cnt; ++frame_idx) {
        const auto frame_info = (*this)[frame_idx];
        nms::sformat(outbuf, "\t{2}| {}", frame_idx, frame_info);
        if (frame_idx + 1 != frame_cnt) {
            outbuf += "\n";
        }
    }
}

#pragma region unittest
nms_test(stacktrace) {
    auto stack_info = StackInfo::backtrace();
    io::log::info("stacks = \n{}", stack_info);
}
#pragma endregion

}