#include <nms/test.h>
#include <nms/io/file.h>
#include <nms/util/stackinfo.h>

namespace nms::io::log
{

Level   gLevel = Level::None;

NMS_API Level get_level() {
    return gLevel;
}

NMS_API void set_level(Level level) {
    gLevel = level;
}

NMS_API TxtFile& _get_log_file() {
    static TxtFile file;
    return file;
}

NMS_API void set_file_path(const Path& path) {
    _get_log_file().reopen(path, FileMode::Read);
}

static str _get_level_name(Level level) {
    static const str names[] ={ "  ", "--", "**", "??", "!!", "XX", "XX" };
    return names[u32(level)];
}

static str get_console_color_of_level(Level type) {
    switch (type) {
        case Level::None:
            return console::$rst;

        case Level::Debug:
            return "";

        case Level::Info:
            return console::$fg_grn;

        case Level::Warn:
            return console::$fg_yel;

        case Level::Alert:
            return console::$bg_grn;

        case Level::Error:
            return console::$fg_red;

        case Level::Fatal:
            return console::$bg_red;

        default:
            return "";
    }
}


NMS_API void logging_message(Level level, IString& msg) {
    if (level < gLevel) {
        return;
    }
    msg += '\n';

    // current process time
    const auto time = clock();
    const auto name = _get_level_name(level);

    // 1. terminal
    {
        const auto color    =  get_console_color_of_level(level);
        auto& console_buff  =  console::_tls_buf_for_write();

        console_buff._resize(0);
        const auto cnt = snprintf(
            console_buff.data, console_buff.capacity, "%s[%s] %6.3f\033[0m %.*s",
            color.data, name.data,time, msg.count, msg.data);

        if (cnt > 0 ) {
            console_buff._resize(u32(cnt));
            console::write(console_buff);
        }

        if (level >= Level::Fatal) {
            auto stack_info = StackInfo::backtrace();
            console::writeln("{}", stack_info);
        }
    }

    // 2. file
    static auto& log_file = _get_log_file();

    if (log_file) {
        static thread_local U8String<4 * 1024 * 1024> file_buff;
        const auto cnt = snprintf(file_buff.data, file_buff.capacity, "[%s] %6.3f %.*s",
                                       name.data, time, msg.count, msg.data);

        if (cnt > 0) {
            file_buff._resize(u32(cnt));
            log_file.write(file_buff);
        }
    }
}

}
