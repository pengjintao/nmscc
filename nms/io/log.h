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

extern Level gLevel;

/* set log level*/
NMS_API void    set_level(Level level);

/* get log level */
NMS_API Level   get_level();

/**
 * set log file
 * @param log_path:  the file path
 *
 * if path is null, will close log file.
 */
NMS_API void    set_file_path(const Path& log_path);


/* show log message */
NMS_API void    logging_message(Level level, IString& s);

/* log buffer */
inline IString& _tls_buf_for_logging() {
    static thread_local U8String<4 * 1024> buf;
    return buf;
}

template<class ...T>
void logging(Level level, const str& fmt, const T& ...args) {
    IString& buf = _tls_buf_for_logging();
    buf._resize(0);

    sformat(buf, fmt, args...);
    logging_message(level, buf);
}

/* nms.io.log: debug message */
template<class ...T>
__forceinline void debug(const str& fmt, const T& ...args) {
    logging(Level::Debug, fmt, args...);
}

/* nms.io.log: info message */
template<class ...T>
__forceinline void info (const str& fmt, const T& ...args) {
    logging(Level::Info,  fmt, args...);
}

/* nms.io.log: warning message */
template<class ...T>
__forceinline void warn (const str& fmt, const T& ...args) {
    logging(Level::Warn,  fmt, args...);
}

/* nms.io.log: alert message */
template<class ...T>
__forceinline void alert(const str& fmt, const T& ...args) {
    logging(Level::Alert, fmt, args...);
}

/* nms.io.log: error message */
template<class ...T>
__forceinline void error(const str& fmt, const T& ...args) {
    logging(Level::Error, fmt, args...); }

/* nms.io.log: fatal message */
template<class ...T>
__forceinline void fatal(const str& fmt, const T& ...args) {
    logging(Level::Fatal, fmt, args...);
}

}
