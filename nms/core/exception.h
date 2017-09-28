#pragma once

#include <nms/core/base.h>
#include <nms/core/view.h>

namespace nms
{

struct StackInfo;

template<u32 N, class ...Targs>
void sformat(IString& outbuf, const char(&fmt)[N], const Targs& ...args);

NMS_API void        set_exception_stackinfo() noexcept;
NMS_API StackInfo&  get_exception_stackinfo() noexcept;

#define NMS_THROW(...)  set_exception_stackinfo(); throw(__VA_ARGS__)

struct Esystem: public Iexception
{
    using Teid = i32;
    Teid eid;

    Esystem(const Teid& eid): eid(eid) {}

    void sformat(IString& buf) const override {
        char msg[256] = "";
        _get_errstr(eid, msg, u32(sizeof(msg)));
        nms::sformat(buf, "eid=`{}`, msg=`{}`", eid, static_cast<const char*>(msg) );
    }
private:
    NMS_API static i32  _get_errno();
    NMS_API static void _get_errstr(Teid eid, char* outbuf, u32 max_cnt);
};

struct IEoutofrange: public Iexception
{};

template<class T>
struct Eoutofrange: IEoutofrange
{
    T   min;
    T   max;
    T   val;

    Eoutofrange(const T& min, const T& max, const T& val): min(min), max(max), val(val)
    {}

    void sformat(IString& buf) const override {
        nms::sformat(buf, "range=[{}, {}], value=`{}`", min, max, val);
    }

};

struct IEunexpect: public Iexception
{};

template<class T>
struct Eunexpect: public IEunexpect
{
    T   expect;
    T   value;

    Eunexpect(const T& expect, const T& value): expect(expect), value(value)
    {}

    void sformat(IString& buf) const override {
        nms::sformat(buf, "expect=`{}`, value=`{}`", expect, value);
    }
};

template<class T>
struct Einvalid: public Iexception
{
    T value;

    Einvalid(const T& value): value(value)
    {}

    void sformat(IString& buf) const override {
        nms::sformat(buf, "value = {}", value);
    }
};

template<class T>
struct Enotfound: public Iexception
{
    T value;

    Enotfound(const T& value): value(value) {
    }

    void sformat(IString& buf) const override {
        nms::sformat(buf, "value = {}", value);
    }
};



}
