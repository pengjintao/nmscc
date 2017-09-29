#pragma once

#include <nms/core/base.h>

namespace nms
{

struct StackInfo
{
#pragma region data
    constexpr static u32 $capacity = 64;

    void*   _stacks[$capacity];
    u32     _index = 2;
    u32     _count = 0;
#pragma endregion

#pragma region constructor
private:
    NMS_API void _backtrace();

public:
    static StackInfo backtrace() {
        StackInfo ret;
        ret._backtrace();
        return ret;
    }

    static StackInfo backtrace(u32 frames_to_skip) {
        StackInfo ret;
        ret._index = frames_to_skip;
        ret._backtrace();
        return ret;
    }
#pragma endregion

#pragma region property
    /*! get stack-frame count */
    __declspec(property(get=get_count)) u32 count;

    u32 get_count() const {
        return _count - _index;
    }

#pragma endregion

#pragma region access
    struct Frame
    {
        void* ptr;
        NMS_API void sformat(IString& buf) const;
    };

    /*! get stack-frame */
    Frame operator[](u32 idx) const {
        if (idx+_index >= _count) {
            return Frame{ nullptr };
        }
        return Frame{ _stacks[idx+_index] };
    }
#pragma endregion

#pragma region format
    NMS_API void sformat(IString& buf) const;
#pragma endregion
};

}