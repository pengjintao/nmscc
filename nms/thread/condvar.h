#pragma once

#include <nms/core.h>

namespace nms::thread
{

class Mutex;

class CondVar
{
public:
    CondVar()
        : _fobj{}
    {
        init();
    }

    ~CondVar() {
        destroy();
    }

    CondVar(CondVar&& rhs)  = delete;
    CondVar(const CondVar&) = delete;

    CondVar& operator= (CondVar&& rhs)  = delete;
    CondVar& operator= (const CondVar&) = delete;

    NMS_API int init();
    NMS_API int destroy();
    NMS_API int signal()       noexcept;
    NMS_API int broadcast()    noexcept;
    NMS_API int wait(Mutex& lock);

private:
    cnd_t _fobj;
};

}

