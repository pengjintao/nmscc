
#include <nms/test.h>
#include <nms/io/console.h>
#include <nms/util/system.h>
#include <nms/thread/thread.h>

#ifdef NMS_OS_WINDOWS
extern "C"
{
    using namespace nms;

    void* GetStdHandle(long fildno);
    int   GetConsoleMode(void* handle, ulong*mode);
    int   SetConsoleMode(void* handle, ulong mode);

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2
}
#endif

namespace nms::io::console
{

NMS_API bool _init() {
    auto has_init = false;

    if (has_init) {
        return true;
    }

#ifdef NMS_OS_WINDOWS
    const auto std_output_handle = -11;
    const auto hout = ::GetStdHandle(std_output_handle);
    ulong mode;
    ::GetConsoleMode(hout, &mode);
    mode |= 0x4;
    ::SetConsoleMode(hout, mode);
#endif
    return has_init;
}

NMS_API void writes(const str text[], u32 n) {
    static auto init = _init();
    (void)init;

    if (n == 0) {
        return;
    }
    else if (n == 1) {
        if (text[0].count > 0) {
            ::write(STDOUT_FILENO, text[0].data, text[0].count);
        }
    }
    else {
        static thread_local U8String<4 *1024 * 1024> tls_buff;  // 4MB
        tls_buff._resize(0u);

        for (u32 i = 0; i < n; ++i) {
            tls_buff += text[i];
        }

        if (tls_buff.count > 0) {
            ::write(STDOUT_FILENO, tls_buff.data, tls_buff.count);
        }
    }
}

NMS_API bool isterm() {
    static const auto cond = ::isatty(STDOUT_FILENO);
    return cond != 0;
}

NMS_API void goto_line(i32 line) {
    if (line == 0 || !isterm() ) {
        return;
    }

    char cmd_buf[32];
    const auto cmd_len = line > 0
        ? ::snprintf(cmd_buf, sizeof(cmd_buf), "\033[%dE", +line)
        : ::snprintf(cmd_buf, sizeof(cmd_buf), "\033[%dF", -line);

    str cmd_str = { cmd_buf, u32(cmd_len)};
    write(cmd_str);
}

NMS_API u32 columns() {
#ifdef NMS_OS_UNIX
    struct winsize w;
    ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
#else
	return 80;
#endif
}

NMS_API void show_cursor(bool value) {
    static thread_local auto cond = true;
    if (cond == value || !isterm()) {
        return;
    }
    cond = value;

    static const str cmd_show = "\033[?25h";
    static const str cmd_hide = "\033[?25l";
    write(cond ? cmd_show : cmd_hide);
}

NMS_API void hide_cursor(bool value) {
    show_cursor(!value);
}

NMS_API void progress_bar(f64 percent, const View<const str>& digits, const View<const str>& circles, const str& color) {
    if (!isterm()) {
        return;
    }

    static const auto bar_len = columns() - 20;

    const auto bar_pos = (percent+0.001)*bar_len;
    const auto bar_idx = u32(bar_pos);

    if (bar_idx < bar_len) {
        // hide cursor
        show_cursor(false);
    }
    else {
        // show cursor
        show_cursor(true);
    }

    U8String<1024> prog_bar;

    if (color.count!=0) {
        prog_bar += color;
    }

    prog_bar += "\033[512D";
    for (auto i = 0u; i < bar_idx; ++i) {
        prog_bar +=digits[digits.count - 1];
    }

    if (bar_idx < bar_len) {
        const auto digits_idx = u32((bar_pos - bar_idx)*digits.count);
        prog_bar +=digits[digits_idx];
    }

    for (auto i = bar_idx+1; i < bar_len; ++i) {
        prog_bar += digits[0];
    }

    if (color.count!=0) {
        prog_bar += $rst;
    }


    if (circles.count != 0) {
        static thread_local auto circles_idx = 0u;
        circles_idx = (circles_idx+1)%circles.count;

        if (bar_idx == bar_len) {
            circles_idx = 0;
        }

        prog_bar += ' ';
        prog_bar += circles[circles_idx];
        prog_bar += ' ';
    }

    sformat(prog_bar, " {.2}%", percent*100.0);
    write(prog_bar);
}

NMS_API str unicode_blocks[] = { u8"░", u8"▏", u8"▎", u8"▍", u8"▌", u8"▋", u8"▊", u8"▉", u8"█"};
NMS_API str unicode_clocks[] = { u8"🕛", u8"🕐", u8"🕑", u8"🕒", u8"🕓", u8"🕔", u8"🕕", u8"🕖", u8"🕗", u8"🕘", u8"🕙", u8"🕚" };

NMS_API str ascii_blocks[]   = { " ", "-", "="};
NMS_API str ascii_clocks[]   = { "-", "/", "|", "\\" };

NMS_API void progress_bar(f64 percent, const str& color) {
#ifdef NMS_OS_UNIX
    progress_bar(percent, unicode_blocks, unicode_clocks, color);
#else
    progress_bar(percent, ascii_blocks, ascii_clocks, color);
#endif
}

nms_test(progress_bar) {
    const auto cnt = 100;
    for (auto i = 0; i < cnt; ++i) {
        const auto percent = f32(i) / f32(cnt);
        progress_bar(percent);
        thread::sleep(0.02);
    }
    progress_bar(1.0f);
    printf("\n");
}

}

