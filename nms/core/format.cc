#include <nms/core.h>

namespace nms
{

#pragma region format: int

template<class T, int E=10>
static uint _uint_to_str(char buff[], const FormatStyle& style, const T& val) {
    (void)style;

    if (E == 10) {
        if (sizeof(T) <= sizeof(short)) {
            return uint(::snprintf(buff, 256, "%hu", ushort(val)));
        }
        if (sizeof(T) == sizeof(unsigned)) {
            return uint(::snprintf(buff, 256, "%u", uint(val)));
        }
        if (sizeof(T) == sizeof(long long)) {
            return uint(::snprintf(buff, 256, "%llu", ullong(val)));
        }
    }
    else {

    }

    return 0u;
}

template<class T, int E=10>
static uint _int_to_str(char buff[], const FormatStyle& style, const T& val) {
    uint cnt = 0u;

    if (val < 0) {
        buff[cnt++] = '-';
    }
    else {
        if (style.sign == '+') { buff[cnt++] = '+'; }
        if (style.sign == '-') { buff[cnt++] = ' '; }
    }

    cnt += _uint_to_str(buff + cnt, style, val >= 0 ? (0+val) : (0-val));

    return cnt;
}

// [align:<>=][width:number].[prec:number][type:cbBnoxX*]
template<class T>
static void _sformat_int(IString& str_out, const FormatStyle& style, const T& val) {
    // repeat ?
    if (style.type == '*') {
        if (val > 0 && val < 256) {
            str_out.appends(u32(val), style._spec[1] == '\0' ? ' ' : style._spec[1]);
        }
        return;
    }

    // char ?
    if (style.type == 'c') {
        str_out.append(char(val));
        return;
    }

    // width == 0 ?
    if (style.width == 0) {
        auto ptr = str_out.data + str_out.count;
        auto cnt = _int_to_str(ptr, style, val);
        str_out._resize(str_out.count + cnt);
        return;
    }

    char str_tmp[512];
    auto str_len = _int_to_str(str_tmp, style, val);

    if (style.width <= str_len) {
        str_out.appends(str_tmp, str_len);
    }
    else {
        switch (style.align) {
        case '<':
            str_out.appends(str_tmp, str_len);
            str_out.appends(style.width - str_len, ' ');
            break;
        case '>': default:
            str_out.appends(style.width - str_len, ' ');
            str_out.appends(str_tmp, str_len);
            break;
        case '^':
            str_out.appends((style.width - str_len + 0) / 2, ' ');
            str_out.appends(str_tmp, str_len);
            str_out.appends((style.width - str_len + 1) / 2, ' ');
            break;
        }
    }
}

NMS_API void _sformat_val(IString& buf, const FormatStyle& style, i8  val) { _sformat_int(buf, style, val); }
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, u8  val) { _sformat_int(buf, style, val); }
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, i16 val) { _sformat_int(buf, style, val); }
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, u16 val) { _sformat_int(buf, style, val); }
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, i32 val) { _sformat_int(buf, style, val); }
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, u32 val) { _sformat_int(buf, style, val); }
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, i64 val) { _sformat_int(buf, style, val); }
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, u64 val) { _sformat_int(buf, style, val); }
#pragma endregion

#pragma region format: float

template<class T, int E=10>
static uint _ufloat_to_str(char buff[], const FormatStyle& style, const T& val) {
    if (E == 10) {
        if (sizeof(T) <= sizeof(f32)) {
            const auto prec = style.prec == 0 ? 3 : style.prec;
            return uint(::snprintf(buff, 256, "%.*f", prec, f32(val)));
        }
        if (sizeof(T) == sizeof(f64)) {
            const auto prec = style.prec == 0 ? 6 : style.prec;
            return uint(::snprintf(buff, 256, "%.*f", prec, f64(val)));
        }
    }
    else {

    }

    return 0u;
}

template<class T, int E=10>
static uint _float_to_str(char buff[], const FormatStyle& style, const T& val) {
    uint cnt = 0u;

    if (val < 0) {
        buff[cnt++] = '-';
    }
    else {
        if (style.sign == '+') { buff[cnt++] = '+'; }
        if (style.sign == '-') { buff[cnt++] = ' '; }
    }

    cnt += _ufloat_to_str(buff + cnt, style, val >= 0 ? +val : -val);

    return cnt;
}

// [align:<>=][sign:+-][width:number].[prec:number]
template<class T>
static void _sformat_float(IString& str_out, const FormatStyle& style, const T& val) {
    // match: width == 0 ?
    if (style.width == 0) {
        auto ptr = str_out.data + str_out.count;
        auto cnt = _float_to_str(ptr, style, val);
        str_out._resize(str_out.count + cnt);
        return;
    }

    char str_tmp[512];
    auto str_len = _float_to_str(str_tmp, style, val);

    if (str_len >= style.width) {
        str_out.appends(str_tmp, str_len);
    }
    else {
        switch (style.align) {
        case '<':
            str_out.appends(str_tmp, str_len);
            str_out.appends(style.width - str_len, ' ');
            break;
        case '>': default:
            str_out.appends(style.width - str_len, ' ');
            str_out.appends(str_tmp, str_len);
            break;
        case '^':
            str_out.appends((style.width - str_len + 0) / 2, ' ');
            str_out.appends(str_tmp, str_len);
            str_out.appends((style.width - str_len + 1) / 2, ' ');
            break;
        }
    }
}

NMS_API void _sformat_val(IString& buf, const FormatStyle& style, f32 val) { _sformat_float(buf, style, val); }
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, f64 val) { _sformat_float(buf, style, val); }
#pragma endregion

#pragma region format: bool
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, bool val) {
    _sformat_val(buf, style, val ? str("true") : str("false") );
}
#pragma endregion

#pragma region format: string
// [align:<>=][width][type:Aa]
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, str val) {
    const auto cnt = val.count;

    if (style.width == 0) {
        buf += val;
    }
    else {
        if (style.width <= cnt) {
            buf += val;
        }
        else {
            switch (style.align) {
            case '<': default:
                buf += val;
                buf.appends(style.width - cnt, ' ');
                break;
            case '>':
                buf.appends(style.width - cnt, ' ');
                buf += val;
                break;
            case '^':
                buf.appends((style.width - cnt + 0) / 2, ' ');
                buf += val;
                buf.appends((style.width - cnt + 1) / 2, ' ');
                break;
            }
        }
    }
}
#pragma endregion

#pragma region format: type
NMS_API void _sformat_val(IString& buf, const FormatStyle& style, const std::type_info& val) {
    auto raw_name = val.name();

#ifdef NMS_CC_MSVC
    _sformat_val(buf, style, str( raw_name, strlen(raw_name)) );
#else
    char    out_name[4096];
    size_t  length = 4096;
    int     status = 0;

    auto cxx_name = abi::__cxa_demangle(raw_name, out_name, &length, &status);

    _sformat_val(buf, style, make_str(cxx_name));

    if (cxx_name != out_name) {
        ::free(cxx_name);
    }
#endif
}
#pragma endregion

NMS_API FormatStyle FormatStyle::from_fmt_str(const str& fmt) {
    FormatStyle ret;

    if (fmt.count == 0) {
        return ret;
    }

    const auto n = fmt.count;
    auto       i = 0u;

    // parse: align
    if (i < n && (fmt[i] == '<' || fmt[i] == '>' || fmt[i] == '^')) {
        ret.align = fmt[i++];
    }

    // parse: sign
    if (i < n && (fmt[i] == '+' || fmt[i] == '-')) {
        ret.sign = fmt[i++];
    }

    // parse: width
    while (i < n && ('0' <= fmt[i] && fmt[i] <= '9')) {
        ret.width *= 10;
        ret.width += fmt[i] - '0';
        ++i;
    }

    // parse: spec
    if (i < n && (fmt[i] == '.')) {
        ++i;

        ret.prec = 0;
        while (i < n && ('0' <= fmt[i] && fmt[i] <= '9')) {
            ret.prec *= 10;
            ret.prec += fmt[i] - '0';
            ++i;
        }
    }

    // parse: type
    if (i < n && fmt[i] != ':') {
        ret.type = fmt[i];
        ++i;
    }

    // parse: spec
    if (i < n && fmt[i] == ':') {
        i++;

        const auto capicity = u32(sizeof(ret._spec));
        for (auto k = 0u; k < capicity && i < n; ++k, ++i) {
            ret._spec[k] = fmt[i];
        }
    }
    return ret;
}

namespace _ns_format
{
NMS_API bool next_value(IString& outbuf, str& strfmt, str& valfmt) {
    //! ----------------------
    //! ... {..........}
    //!     ^          ^
    //!     |          |
    //!  pos_beg       |
    //!             pos_end
    //! ---------------------
    const auto fmt_len = strfmt.count;
    const auto fmt_ptr = strfmt.data;

    //1. find {
    u32 pos_beg = 0;

    while (pos_beg < fmt_len) {
        const auto c = strfmt[pos_beg];

        if (c == '{') {
            if (pos_beg + 1 < fmt_len && strfmt[pos_beg + 1] == '{') {
                outbuf += '{';
                ++pos_beg;
            }
            else {
                break;
            }
        }
        else {
            outbuf += c;
        }
        ++pos_beg;
    }

    if (pos_beg == fmt_len) {
        return false;
    }

    //2. find }
    u32 pos_end =pos_beg + 1;
    while (pos_end < fmt_len) {
        const auto c = strfmt[pos_end];

        if (c == '}') {
            if (pos_end + 1 < fmt_len && strfmt[pos_end + 1] == '}') {
                ++pos_end;
            }
            else {
                break;
            }
        }
        ++pos_end;
    }

    if (pos_end == fmt_len) {
        return false;
    }

    //3. {}
    valfmt = str(fmt_ptr + pos_beg + 1, pos_end - pos_beg - 1);
    strfmt = str(fmt_ptr + pos_end + 1, fmt_len - pos_end - 1);

    return true;
}
}

}