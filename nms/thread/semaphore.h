#pragma once

#include <nms/core.h>

namespace nms::thread
{

class  Semaphore final
{
public:
    Semaphore() {
        open(0);
    }

    explicit Semaphore(u32 value) {
        open(value);
    }

    ~Semaphore() {
        close();
    }

    Semaphore(Semaphore&& rhs) noexcept
        : _thrd(rhs._thrd)
    {
        rhs._thrd = nullptr;
    }

    Semaphore& operator=(Semaphore&& rhs) {
        if (&rhs != this) {
            close();
            _thrd = rhs._thrd;
            rhs._thrd = nullptr;
        }
        return *this;
    }
    Semaphore(const Semaphore&) = delete;


    Semaphore& operator=(const Semaphore&) = delete;

    NMS_API void open(u32 value);
    NMS_API void close();

    NMS_API Semaphore& operator+=(u32 val);
    NMS_API Semaphore& operator++();
    NMS_API Semaphore& operator--();

private:
    sem_t* _thrd = nullptr;
};

}