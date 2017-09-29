#include <nms/core.h>

namespace nms
{

#ifdef NMS_OS_WINDOWS
extern "C" {
    int QueryPerformanceFrequency(u64* freq);
    int QueryPerformanceCounter(u64* counter);
}

NMS_API f64 system_clock() {
    static const auto freq = [] {
        u64 val;
        QueryPerformanceFrequency(&val);
        return val;
    }();

    u64 clock;
    QueryPerformanceCounter(&clock);

    return static_cast<f64>(clock) / static_cast<f64>(freq);
}

static const f64 g_clock_start = system_clock();

NMS_API f64 clock() {
    const auto clock_now = system_clock();
    return clock_now - g_clock_start;
}
#endif

#ifdef NMS_OS_UNIX
NMS_API f64 system_clock() {
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    auto sec = spec.tv_sec;
    auto nsec = spec.tv_nsec;
    return f64(sec) + f64(nsec*1e-9);
};

static const auto g_clock_start = system_clock();

NMS_API f64 clock() {
    auto clock_now = system_clock();
    return clock_now - g_clock_start;
}
#endif

// UTC
NMS_API DateTime DateTime::from_stamp(i64 stamp) {
    const time_t clock = stamp;

    struct tm tm = {};
#ifdef NMS_OS_WINDOWS
    ::gmtime_s(&tm, &clock);
#else
    ::gmtime_r(&clock, &tm);
#endif
    tm.tm_year += 1900;

    const DateTime ret = {
        u16(tm.tm_year),
        u16(tm.tm_mon + 1),
        u16(tm.tm_mday),
        u16(tm.tm_hour),
        u16(tm.tm_min),
        u16(tm.tm_sec)
    };

    return ret;
}

// UTC
NMS_API DateTime::Tstamp DateTime::get_stamp() const {
    tm tm = {};
    tm.tm_year  = year;
    tm.tm_mon   = month- 1;
    tm.tm_mday  = day;
    tm.tm_hour  = hour;
    tm.tm_min   = minute;
    tm.tm_sec   = second;

    tm.tm_year -= 1900;
#ifdef NMS_OS_WINDOWS
    const auto result = ::_mkgmtime(&tm);
#else
    const auto result = ::timegm(&tm);
#endif
    return Tstamp(result);
}

NMS_API DateTime DateTime::now() {
    const auto  time_stamp = ::time(nullptr);
    struct tm   time_struct;
#ifdef NMS_OS_WINDOWS
    ::localtime_s(&time_struct, &time_stamp);
#else
    ::localtime_r(&time_stamp, &time_struct);
#endif
    return {
        u32(time_struct.tm_year) + 1900,
        u32(time_struct.tm_mon)+1,
        u32(time_struct.tm_mday),
        u32(time_struct.tm_hour),
        u32(time_struct.tm_min),
        u32(time_struct.tm_sec)
    };
}

NMS_API bool DateTime::parse(str text, str /*fmt*/, DateTime& dt) {
    auto cnt  = sscanf(text.data, "%hu-%hu-%huT%hu:%hu:%hu", &dt.year, &dt.month, &dt.day, &dt.hour, &dt.minute, &dt.second);
    return (cnt == 6);
}

NMS_API void DateTime::sformat(IString& buf, const FormatStyle& /*fmt*/) const {
    const auto len = buf.count;
    buf.reserve(len+32);
    const auto dat = buf.data;
    const auto ret = ::snprintf(dat+len, 32, "%hu-%02hu-%02huT%02hu:%02hu:%02hu", year, month, day, hour, minute, second);
    if (ret > 0) {
        buf._resize(len + u32(ret));
    }
}

}
