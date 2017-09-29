#include <nms/test.h>
#include <nms/math/base.h>

namespace nms
{

#pragma region parse: number

// fmt:
//    {[cnbBoxX]?:[0-9]*}
template<class T>
static bool _parse_int(str& text, const str& fmt, T& val) {

    // .....([+-]?)([0-9]*)....
    //      ^              ^
    //      |              |
    //   pos_beg           |
    //                 pos_end
    auto str_ptr = text.data;
    auto str_len = text.count;
    auto pos_beg = 0u;

    //1. ignore space
    while (pos_beg < str_len && isblank(str_ptr[pos_beg])) {
        ++pos_beg;
    }
    if (pos_beg == str_len) {
        return false;
    }

    auto pos_end = pos_beg;

    //2. parse sign
    const auto sign = str_ptr[pos_end];
    switch (sign) {
    case '-':
        ++pos_end;
        if ($is<$uint, T>) {
            return false;
        }
    case '+':
        ++pos_end;
        break;
    default:
        break;
    }

    //3. parse body
    auto abs_val = T(0);
    if (fmt.count > 0) {
        const auto type = fmt[0];
        if (type == 'n' || type == 'd') {
            goto RADIX_10;
        }
        else if (type == 'b' || type == 'B') {   // 2
            while (pos_end < str_len) {
                const auto c = str_ptr[pos_end];

                if (c == '0') {
                    abs_val *= 2;
                }
                else if (c == '1') {
                    abs_val *= 2;
                    abs_val += 1;
                }
                else {
                    break;
                }
            }
        }
        else if (type == 'o' || type == 'O') {   // 8
            while (pos_end < str_len) {
                const auto c = str_ptr[pos_end];

                if (c >= '0' && c <= '7') {
                    abs_val *= 8;
                    abs_val += T(c - '0');
                }
                else {
                    break;
                }
            }
        }
        else if (type == 'x' || type == 'X') {   // 16
            while (pos_end < str_len) {
                const auto c = str_ptr[pos_end];

                if (c >= '0' && c <= '9') {
                    abs_val *= 16;
                    abs_val += T(c - '0');
                }
                else if (c >= 'a' && c <= 'f') {
                    abs_val *= 16;
                    abs_val += T(c - 'a');
                }
                else if (c >= 'A' && c <= 'F') {
                    abs_val *= 16;
                    abs_val += T(c - 'A');
                }
                else {
                    break;
                }
            }
        }
    }
    else {
    RADIX_10:
        while (pos_end < str_len) {
            const auto c = str_ptr[pos_end];

            if (c >= '0' && c <= '9') {
                abs_val *= 10;
                abs_val += T(c - '0');
                ++pos_end;
            }
            else {
                break;
            }
        }
    }

    if (sign == '+') {
        if (pos_end == pos_beg + 1) {
            return false;
        }
        val = abs_val;
    }
    else if (sign == '-') {
        if (pos_end == pos_beg + 1) {
            return false;
        }
        val = T(0 - abs_val);
    }
    else {
        if (pos_end == pos_beg) {
            return false;
        }
        val = abs_val;
    }

    text = str(str_ptr + pos_end, str_len - pos_end + 1);
    return true;
}

template<class T>
static bool _parse_float(str& text, str /*fmt*/, T& val) {
    int int_part        = 0;
    int decimal_part    = 0;
    int exponent_part   = 0;

    //1. int part
    {
        const auto stat = _parse_int(text, {}, int_part);
        if (stat == false) {
            return false;
        }
    }

    //2. decimal part
    if (text.count != 0 && text[0] == '.') {
        auto tmp = str(text.data + 1, text.count - 1);
        auto ret = _parse_int(tmp, {}, decimal_part);
        if (ret) {
            text = tmp;
        }
    }

    //3. exponent part
    if (text.count != 0 && (text[0] == 'E' || text[0] == 'e')) {
        auto tmp = str(text.data + 1, text.count - 1);
        auto ret = _parse_int(tmp, {}, exponent_part);
        if (ret) {
            text = tmp;
        }
    }

    //4. make val
    val = T(int_part);
    if (decimal_part == 0) {}
    else if (decimal_part < 10      ) { val += decimal_part / T(10);        }
    else if (decimal_part < 100     ) { val += decimal_part / T(100);       }
    else if (decimal_part < 1000    ) { val += decimal_part / T(1000);      }
    else if (decimal_part < 10000   ) { val += decimal_part / T(10000);     }
    else if (decimal_part < 100000  ) { val += decimal_part / T(100000);    }
    else if (decimal_part < 1000000 ) { val += decimal_part / T(1000000);   }
    else if (decimal_part < 10000000) { val += decimal_part / T(10000000);  }
    else {}

    // 5. set exponent
    switch (exponent_part) {
    case 0:                     break;
    case +1:    val *= T(1E+1); break;
    case +2:    val *= T(1E+2); break;
    case +3:    val *= T(1E+3); break;
    case +4:    val *= T(1E+4); break;
    case +5:    val *= T(1E+5); break;
    case +6:    val *= T(1E+6); break;
    case +7:    val *= T(1E+7); break;
    case +8:    val *= T(1E+8); break;
    case +9:    val *= T(1E+9); break;
    case -1:    val *= T(1E-1); break;
    case -2:    val *= T(1E-2); break;
    case -3:    val *= T(1E-3); break;
    case -4:    val *= T(1E-4); break;
    case -5:    val *= T(1E-5); break;
    case -6:    val *= T(1E-6); break;
    case -7:    val *= T(1E-7); break;
    case -8:    val *= T(1E-8); break;
    case -9:    val *= T(1E-9); break;
    default:    val *= math::exp(T(exponent_part));
    }

    return true;
}

NMS_API bool _parse_num(str& text, str style, u8&  val) { return _parse_int(text, style, val); }
NMS_API bool _parse_num(str& text, str style, i8&  val) { return _parse_int(text, style, val); }
NMS_API bool _parse_num(str& text, str style, u16& val) { return _parse_int(text, style, val); }
NMS_API bool _parse_num(str& text, str style, i16& val) { return _parse_int(text, style, val); }
NMS_API bool _parse_num(str& text, str style, u32& val) { return _parse_int(text, style, val); }
NMS_API bool _parse_num(str& text, str style, i32& val) { return _parse_int(text, style, val); }
NMS_API bool _parse_num(str& text, str style, u64& val) { return _parse_int(text, style, val); }
NMS_API bool _parse_num(str& text, str style, i64& val) { return _parse_int(text, style, val); }
NMS_API bool _parse_num(str& text, str style, f32& val) { return _parse_float(text, style, val); }
NMS_API bool _parse_num(str& text, str style, f64& val) { return _parse_float(text, style, val); }
#pragma endregion

#pragma region parse: string
NMS_API bool _parse_str(str& text, str style, str& val) {
    (void)style;
    val = text;
    text = str{};
    return true;
}

NMS_API bool _parse_str(str& text, str style, bool& val) {
    str str_val;
    auto ret = _parse_str(text, style, str_val);

    if (!ret) {
        return false;
    }

    if (str_val == str("true") || str_val == str("True") || str_val == str("TRUE")) {
        val = true;
        return true;
    }
    if (str_val == str("false") || str_val == str("False") || str_val == str("FALSE")) {
        val = false;
        return true;
    }

    return false;
}

#pragma endregion

}
