#pragma once

#include <nms/core.h>
#include <nms/thread/semaphore.h>

namespace nms::thread
{

#ifdef NMS_OS_UNIX
using   thrd_ret_t = void*;
#else
using   thrd_ret_t = void;
#endif

struct Ithread
{
    thrd_t  _thrd = thrd_t(0);

    NMS_API int join();
    NMS_API int detach();

    NMS_API static void yield();
    NMS_API static int  sleep(double duration);

protected:
    NMS_API void _start(thrd_ret_t(*pfun)(void*), void* pobj);
};

class Thread final: public Ithread
{
public:
    constexpr static auto $buff_size = 64;

    template<class Tfunc>
    explicit Thread(Tfunc&& func) {
        start(fwd<Tfunc>(func));
    }

    virtual ~Thread() {
        detach();
    }

    Thread(Thread&&)                    = delete;
    Thread(const Thread&)               = delete;
    Thread& operator=(Thread&&)         = delete;
    Thread& operator=(const Thread&)    = delete;

private:
    u64     buff_[$buff_size/sizeof(u64)];

    template<class Tfunc>
    void start(Tfunc&& func) {
        using Tobj = Tvalue<Tmutable<Tfunc>>;
        static_assert(sizeof(Tobj) <= sizeof(buff_), "nms::thread::Thread: `func` size is to large");

        auto pobj = new(buff_)Tobj(fwd<Tobj>(func));

        auto pfun = [](void* ptr) -> thrd_ret_t {
            Tobj*   pobj = static_cast<Tobj*>(ptr);
            Tobj    obj(static_cast<Tobj&&>(*pobj));
            obj();

        #ifdef NMS_OS_UNIX
            return nullptr;
        #endif
        };

        Ithread::_start(pfun, pobj);
    }
};

inline void yield() {
    Ithread::yield();
}

inline void sleep(double seconds) {
    Ithread::sleep(seconds);
}

}
