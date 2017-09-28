#include <nms/test.h>
#include <nms/thread.h>
#include <nms/util/system.h>

#ifdef NMS_OS_WINDOWS
extern "C" {
    using namespace nms;
    using thrd_start_t = _beginthread_proc_type;

    int CloseHandle(void* handle);
    int WaitForSingleObjectEx(void* hHandle, u32 dwMilliseconds, int bAlertable);
    int SwitchToThread();

    static int thrd_create(thrd_t* thr, thrd_start_t func, void* arg) {
        u32  tid = 0;
        auto obj = _beginthread(func, 0, arg);
        *thr = reinterpret_cast<thrd_t>(obj);
        return int(tid);
    }

    static int thrd_detach(thrd_t thr) {
        ::CloseHandle(thr);
        return 0;
    }

    static int thrd_join(thrd_t thr, int* /*res*/) {
        const auto ret = ::WaitForSingleObjectEx(thr, 0xFFFFFFFFu, 0);
        return ret;
    }

    static void thrd_yeild() {
        ::SwitchToThread();
    }
}
#else

extern "C" {
    typedef void*(thrd_start_t)(void*);

    static int thrd_create(thrd_t* thr, thrd_start_t func, void* arg) {
        return pthread_create(thr, nullptr, func, arg);
    }

    static int thrd_detach(thrd_t thr) {
        return pthread_detach(thr);
    }

    static int thrd_join(thrd_t thr, int* res) {
        (void)res;
        return pthread_join(thr, nullptr);
    }

    static void thrd_yeild() {
        sched_yield();
    }
}
#endif


namespace nms::thread
{

NMS_API void Ithread::_start(thrd_ret_t(*pfun)(void*), void* pobj) {
    if (_thrd != thrd_t(0)) {
        return;
    }
    auto ret = thrd_create(&_thrd, pfun, pobj);

    if (_thrd == thrd_t(0) && ret == 0) {
        io::log::error("nms.thread.Thread.start : start thread failed.");
    } else {
        io::log::debug("nms.thread.Thread.start : {}", u64(_thrd) );
    }
}

NMS_API int Ithread::detach() {
    if (_thrd == thrd_t(0) ) {
        return 0;
    }

    io::log::debug("nms.thread.Thread.detach: {}", u64(_thrd) );
    auto ret = thrd_detach(_thrd);
    _thrd = thrd_t(0);
    return ret;
}

NMS_API int Ithread::join() {
    if (_thrd == thrd_t(0) ) {
        return 0;
    }

    io::log::debug("nms.thread.Thread.join  : {}", u64(_thrd) );
    auto ret = thrd_join(_thrd, nullptr);
    _thrd = thrd_t(0);
    return ret;
}

NMS_API void Ithread::yield() {
    thrd_yeild();
}

NMS_API int Ithread::sleep(double duration) {
    system::sleep(duration);
    return 0;
}

}

