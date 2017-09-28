#pragma once

#include <nms/core.h>

namespace nms::io
{

class ENotExists: public Iexception
{};

class Path final
{
public:
    Path()
        : str_{}
    {}

    Path(const str& s)
        : str_{s}
    {}

    Path(const IString& s)
        : str_{s}
    {}

    template<u32 N>
    Path(const char(&s)[N])
        : str_{s}
    {}

    template<class U, class ...Us>
    Path(str fmt, const U& u, const Us& ...us) {
        sformat(str_, fmt, u, us...);
    }

    NMS_API Path& operator/=(const str& rhs);
    NMS_API Path& operator+=(const str& rhs);

    NMS_API str ext()  const;
    NMS_API str base() const;
    NMS_API str path() const;

    operator str() const {
        return str_;
    }

    template<class Text>
    Path operator+(const Text& ext) const {
        auto tmp(*this);
        tmp += str{ ext };
        return tmp;
    }

    template<class Text>
    Path operator/(const Text& ext) const {
        auto tmp(*this);
        tmp /= str{ ext };
        return tmp;
    }

private:
    U8String<256> str_;
};

NMS_API Path  cwd();
NMS_API void  chdir(const Path& path);
NMS_API bool  exists(const Path& path);
NMS_API void  mkdir(const Path& path);
NMS_API void  remove(const Path& path);
NMS_API void  rename(const Path& oldpath, const Path& newpath);
}
