#include <nms/test.h>
#include <nms/serialization/dom.h>

namespace nms::serialization
{

#pragma region format:json
NMS_API void DOM::_format_json(IString& buf, u32 level) const {
    static const auto $indent = 4;

    auto& v = this->node;

    switch (v.type) {
    case Type::$null:   nms::_sformat_val(buf, {}, "");         break;
    case Type::$bool:   nms::_sformat_val(buf, {}, v.$bool);    break;
    case Type::$u8:     nms::_sformat_val(buf, {}, v.$u8);      break;
    case Type::$i8:     nms::_sformat_val(buf, {}, v.$i8);      break;
    case Type::$u16:    nms::_sformat_val(buf, {}, v.$u16);     break;
    case Type::$i16:    nms::_sformat_val(buf, {}, v.$i16);     break;
    case Type::$u32:    nms::_sformat_val(buf, {}, v.$u32);     break;
    case Type::$i32:    nms::_sformat_val(buf, {}, v.$i32);     break;
    case Type::$u64:    nms::_sformat_val(buf, {}, v.$u64);     break;
    case Type::$i64:    nms::_sformat_val(buf, {}, v.$i64);     break;
    case Type::$f32:    nms::_sformat_val(buf, {}, v.$f32);     break;
    case Type::$f64:    nms::_sformat_val(buf, {}, v.$f64);     break;

    case Type::$time: {
        buf += "\"";
        time::from_stamp(v.$time).sformat(buf, {});
        buf += "\"";
        break;
    }
    case Type::$num: {
        nms::_sformat_val(buf, {}, v.text);
        break;
    }

    case Type::$key: case Type::$str: {
        buf += "\"";
        nms::_sformat_val(buf, {}, v.text);
        buf += "\"";
        break;
    }

    case Type::$array: {
        buf += "[\n";

        for (auto itr = begin(); itr != end(); ) {
            buf.appends((level+1)*$indent, ' ');
            (*itr)._format_json(buf, level + 1);

            ++itr;
            (itr == end()) ? buf += "\n" : buf += ",\n";
        }

        buf.appends(level * $indent, ' ');
        buf += "]";
        break;
    }
    case Type::$object: {
        buf += "{\n";

        for (auto itr = begin(); itr != end();) {
            buf.appends((level + 1)*$indent, ' ');
            buf += "\"";
            buf += itr.key();
            buf += "\": ";
            (*itr)._format_json(buf, level + 1);
            ++itr;

            (itr == end()) ? str{ "\n" } : str{ ",\n" };
        }
        buf.appends(level * $indent, ' ');
        buf += "}";
        break;
    }
    default:
        break;
    }
}
#pragma endregion

#pragma region parse:json
// parse
static bool expect(const str& expect, const str& text) {
    for (u32 i = 1; i < expect.count; ++i) {
        if (expect[i] != text[i]) {
            return false;
        }
    }
    return true;
}

// test if blank
static bool isBlank(char c) {
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') return true;
    return false;
}

static char peekChar(str& text) {
    u32 pos = 0;
    u32 len = u32(text.count);
    while (pos < len && isBlank(text[pos])) pos++;

    auto c = text[pos];
    text = text.slice(pos, u32(text.count - 1));
    return c;
}

static char parseChar(str& text) {
    auto c = peekChar(text);
    text=text.slice(1u, u32(text.count) - 1);
    return c;
}

static i32 parseAny(str& text, DOM* nodes, i32 proot, i32 pleft);

static i32 parseNum(str& text, DOM* nodes, i32 proot, i32 pleft) {
    // 0123456789,
    // ^         ^
    // s         p
    auto s = text.data;
    auto p = s;
    while (*p != ',' && *p != '}' && *p != ']' && *p != '\n') {
        ++p;
    }

    auto ret = nodes->add(proot, pleft, Node::from_num(str{ s, u32(p - s) }));
    text     = str{ p, u32(text.count - 1) };
    return ret;
}

static i32 parseStr(str& text, DOM* nodes, i32 proot, i32 pleft) {
    // "abcdefg"
    // ^ ......^
    // b       e
    auto ptr    = text.data;
    auto pos    = text.data + 1;

    while (true) {
        auto c = *pos;

        if (c == '\\') {
            pos += 2;
        }
        else if (c == '"') {
            break;
        }
        else {
            pos += 1;
        }
    }

    const auto b = 0;
    const auto e = u32(pos - ptr);
    const auto ret = nodes->add(proot, pleft, Node::from_str({ text.data + b + 1, e - b - 1 }));
    text = text.slice(e + 1, u32(text.count) - 1);
    return ret;
}

static i32 parseKey(str& text, DOM* nodes, i32 proot, i32 pleft) {
    // "abcdefg"
    // ^ ......^
    // b       e

    u32 b = 0;
    u32 e = 1;
    while (text[e] != '"' && text[e - 1] != '\\') ++e;
    auto ret = nodes->add(proot, pleft, Node::from_key({ text.data + b + 1,  e - b - 1 }));
    text = text.slice(e + 1, u32(text.count) - 1);
    return ret;
}

static i32 parseArray(str& text, DOM* nodes, i32 proot, i32 pleft) {
    auto parr = nodes->add(proot, pleft, Node::make_array());

    text = text.slice(1u, u32(text.count) - 1);
    if (peekChar(text) == ']') {
        parseChar(text);
        return parr;
    }

    auto prev_val = -1;
    while (true) {
        auto this_val = parseAny(text, nodes, parr, prev_val);
        prev_val = this_val;

        auto next_char = parseChar(text);
        if (next_char == ']') {
            return parr;
        }
        if (next_char == ',') {
            continue;
        }
        return -1;
    }
}

static i32 parseObject(str& text, DOM* nodes, i32 proot, i32 pleft) {
    // add new node
    auto pobj = nodes->add(proot, pleft, Node::make_object());

    text = text.slice(1, -1);
    if (peekChar(text) == '}') {
        return pobj;
    }

    auto prev_key = -1;
    auto prev_val = -1;

    while (true) {
        // key
        const auto next_quota  = peekChar(text);
        if (next_quota != '"') {
            return -1;
        }
        const auto this_key  = parseKey(text, nodes, pobj, prev_key);

        const auto next_colon = parseChar(text);
        if (next_colon != ':') {
            io::log::error("nms.serialization.json.parse_object: expect ':', but '{:c}' ", next_colon);
            return -1;
        }
        const auto this_val  = parseAny(text, nodes, -1, prev_val);
        (void)this_val;

        prev_key = this_key;
        prev_val = this_val;

        const auto next_char = parseChar(text);
        if (next_char == '}') {
            break;
        }
        if (next_char == ',') {
            continue;
        }
        io::log::error("nms.serialization.json.parse_object: expect ',' or '}', but '{:c}' ", next_char);
        return -1;
    }
    return pobj;
}

static i32 parseAny(str& text, DOM* nodes, i32 proot, i32 pleft) {
    auto result = -1;

    const auto text_len    = text.count;

    // peek next char
    const auto next_char = peekChar(text);

    // test char
    switch (next_char) {
    case 'n':
    {
        if (expect("null", text)) {
            text    = text.slice(4u, text_len - 1);
            result  = nodes->add(proot, pleft, Node::make_null() );
        }
        break;
    }
    case 't':
    {
        if (expect("true", text)) {
            text    = text.slice(4u, text_len - 1);
            result  = nodes->add(proot, pleft, Node::from_bool(true));
        }
        break;
    }
    case 'f':
    {
        if (expect("false", text)) {
            text    = text.slice(5u, text_len - 1);
            result  = nodes->add(proot, pleft, Node::from_bool(false));
        }
        break;
    }

    case '[':
    {
        result = parseArray(text, nodes, proot, pleft);
        break;
    }

    case '{':
    {
        result = parseObject(text, nodes, proot, pleft);
        break;
    }

    case '"':
    {
        result = parseStr(text, nodes, proot, pleft);
        break;
    }

    case '+':
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        result = parseNum(text, nodes, proot, pleft);
        break;
    }

    default:
        break;
    }

    return result;
}

// wraper
NMS_API void DOM::_parse_json(const str& text) {
    auto str = text;
    parseAny(str, this, -1, -1);
}

#pragma endregion

#pragma region unittest

struct TestObject
{
    NMS_REFLECT_BEGIN;
    typedef String<32>    NMS_MEMBER(a);
    typedef f32x4         NMS_MEMBER(b);
    typedef DateTime      NMS_MEMBER(c);
    NMS_REFLECT_END;
};

nms_test(json_serialize) {
    TestObject obj;
    obj.a = "hello";
    obj.b ={ 1.1f, +2.2f, -3.3f, 4.4e2f };
    obj.c = DateTime(2017, 9, 3, 8, 30, 12);

    auto tree = Tree<64>::from_object(obj);

    io::log::info("obj  = {}", obj);
    io::log::info("json = {:json}", tree);
}

nms_test(json_deserialize) {
    const char text[] = R"(
{
    "a": "hello",
    "b": [ 1.1, +2.2, -3.3, 4.4e2],
    "c": "2017-09-03T08:30:12"
}
)";
    // json_str -> json_tree
    auto dom = Tree<32>::from_json(text);

    TestObject val;
    dom.deserialize(val);
    io::log::debug("obj = {}", val);
}

#pragma endregion

}
