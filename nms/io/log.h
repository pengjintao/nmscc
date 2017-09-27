#pragma once

#include <nms/core.h>
#include <nms/io/console.h>

namespace nms::io::log
{

enum class Level
{
    None,
    Debug,
    Info,
    Warn,
    Alert,
    Error,
    Fatal,
};

inline auto _make_enum_names(Level) {
    static str names[] = { "  ", "--", "**", "??", "!!", "XX", "XX" };
    return View<const str>{ names};
}

extern Level gLevel;

/* set log level*/
NMS_API void    setLevel(Level level);

/* get log level */
NMS_API Level   getLevel();

/**
 * set log file
 * @param log_path:  the file path
 *
 * if path is null, will close log file.
 */
NMS_API void    setLogPath(const Path& log_path);

/* log buffer */
NMS_API IString& _tls_strbuf();

/* show log message */
NMS_API void    _message(Level level, IString& s);

template<class ...T>
void message(Level level, const str& fmt, const T& ...args) {
    IString& buf = _tls_strbuf();
    sformat(buf, fmt, args...);
    _message(level, buf);
}

/* nms.io.log: debug message */
template<class ...T>
__forceinline void debug(const str& fmt, const T& ...args) {
    message(Level::Debug, fmt, args...);
}

/* nms.io.log: info message */
template<class ...T>
__forceinline void info (const str& fmt, const T& ...args) {
    message(Level::Info,  fmt, args...);
}

/* nms.io.log: warning message */
template<class ...T>
__forceinline void warn (const str& fmt, const T& ...args) {
    message(Level::Warn,  fmt, args...);
}

/* nms.io.log: alert message */
template<class ...T>
__forceinline void alert(const str& fmt, const T& ...args) {
    message(Level::Alert, fmt, args...);
}

/* nms.io.log: error message */
template<class ...T>
__forceinline void error(const str& fmt, const T& ...args) {
    message(Level::Error, fmt, args...); }

/* nms.io.log: fatal message */
template<class ...T>
__forceinline void fatal(const str& fmt, const T& ...args) {
    message(Level::Fatal, fmt, args...);
}

}
