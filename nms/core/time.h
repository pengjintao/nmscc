#pragma once

#include <nms/core/base.h>
#include <nms/core/string.h>

namespace nms
{

struct DateTime;
using  time = DateTime;

/* wall time */
struct DateTime
{
    u16 year    = 0;    // year         [0~65535]
    u16 month   = 0;    // month        [1~12]
    u16 day     = 0;    // day of month [1~31]

    u16 hour    = 0;    // hour         [0~23]
    u16 minute  = 0;    // minute       [0~59]
    u16 second  = 0;    // second       [0~59]

    constexpr DateTime() = default;

    constexpr DateTime(u32 year, u32 month, u32 day)
        : year(u16(year)), month(u16(month)), day(u16(day))
    {}

    constexpr DateTime(u32 year, u32 month, u32 day, u32 hour, u32 minute, u32 second)
        : year(u16(year)), month(u16(month)), day(u16(day))
        , hour(u16(hour)), minute(u16(minute)), second(u16(second))
    {}

#pragma region property
    using Tstamp = i64;
    NMS_API Tstamp get_stamp() const;

    __declspec(property(get=get_stamp)) Tstamp stamp;   // time stamp
#pragma endregion

#pragma region method
    NMS_API static DateTime from_stamp(i64 stamp);
    NMS_API static DateTime now();
#pragma endregion

#pragma region operator
    friend i64 operator-(const DateTime& a, const DateTime& b) {
        const auto ta = a.stamp;
        const auto tb = b.stamp;
        return ta - tb;
    }
#pragma endregion

#pragma region format/parse
    static DateTime from_str(str text) {
        DateTime ret;
        DateTime::parse(text, {}, ret);
        return ret;
    }

    NMS_API static bool parse(str text, str fmt, DateTime& datetime);
    NMS_API void        sformat(IString& buff, const FormatStyle& fmt) const;
#pragma endregion

};

/*!
 * process clock
 */
NMS_API f64 clock();

}
