#include <nms/test.h>
#include <nms/serialization/dom.h>

namespace nms::serialization
{

#pragma region format:json
NMS_API void DOM::_format_json(IString& outbuf, u32 indent_level) const {
    constexpr static auto $indent_size = 4;

    auto& this_node = this->node;

    switch (this_node.type) {
    case NodeType::$null:   nms::_sformat_val(outbuf, {}, "null");            break;
    case NodeType::$bool:   nms::_sformat_val(outbuf, {}, this_node.$bool);   break;
    case NodeType::$u8:     nms::_sformat_val(outbuf, {}, this_node.$u8);     break;
    case NodeType::$i8:     nms::_sformat_val(outbuf, {}, this_node.$i8);     break;
    case NodeType::$u16:    nms::_sformat_val(outbuf, {}, this_node.$u16);    break;
    case NodeType::$i16:    nms::_sformat_val(outbuf, {}, this_node.$i16);    break;
    case NodeType::$u32:    nms::_sformat_val(outbuf, {}, this_node.$u32);    break;
    case NodeType::$i32:    nms::_sformat_val(outbuf, {}, this_node.$i32);    break;
    case NodeType::$u64:    nms::_sformat_val(outbuf, {}, this_node.$u64);    break;
    case NodeType::$i64:    nms::_sformat_val(outbuf, {}, this_node.$i64);    break;
    case NodeType::$f32:    nms::_sformat_val(outbuf, {}, this_node.$f32);    break;
    case NodeType::$f64:    nms::_sformat_val(outbuf, {}, this_node.$f64);    break;

    case NodeType::$time: {
        outbuf += "\"";
        time::from_stamp(this_node.$time).sformat(outbuf, {});
        outbuf += "\"";
        break;
    }
    case NodeType::$num: {
        nms::_sformat_val(outbuf, {}, this_node.text);
        break;
    }

    case NodeType::$key: case NodeType::$str: {
        outbuf += "\"";
        nms::_sformat_val(outbuf, {}, this_node.text);
        outbuf += "\"";
        break;
    }

    case NodeType::$array: {
        outbuf += "[\n";

        auto element_idx = 0;
        auto element_cnt = this_node.size;

        for (auto itr = begin(); element_idx < element_cnt; ++element_idx, ++itr) {
            outbuf.appends((indent_level+1)*$indent_size, ' ');
            (*itr)._format_json(outbuf, indent_level + 1);

            element_idx + 1 < element_cnt
                ? outbuf += ",\n"
                : outbuf += "\n";
        }

        outbuf.appends(indent_level * $indent_size, ' ');
        outbuf += "]";
        break;
    }
    case NodeType::$object: {
        outbuf += "{\n";

        auto element_idx = 0;
        auto element_cnt = this_node.size;
        for (auto itr = begin(); element_idx < element_cnt; ++element_idx, ++itr) {
            outbuf.appends((indent_level + 1)*$indent_size, ' ');
            outbuf += "\"";
            outbuf += itr.key();
            outbuf += "\": ";
            (*itr)._format_json(outbuf, indent_level + 1);

            element_idx+1 < element_cnt 
                ? outbuf += ",\n"
                : outbuf += "\n";
        }
        outbuf.appends(indent_level * $indent_size, ' ');
        outbuf += "}";
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
static bool is_blank(char c) {
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') return true;
    return false;
}

static char json_peek_char(str& text) {
    u32 pos = 0;
    u32 len = u32(text.count);
    while (pos < len && is_blank(text[pos])) pos++;

    auto c = text[pos];
    text = text.slice(pos, u32(text.count - 1));
    return c;
}

static char json_parse_char(str& text) {
    auto c = json_peek_char(text);
    text=text.slice(1u, u32(text.count) - 1);
    return c;
}

static i32 json_parse_any(str& text, DOM* nodes, i32 proot, i32 pleft);

static i32 json_parse_number(str& text, DOM* nodes, i32 proot, i32 pleft) {
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

static i32 json_parse_string(str& text, DOM* nodes, i32 proot, i32 pleft) {
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

static i32 json_parse_key(str& text, DOM* nodes, i32 proot, i32 pleft) {
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

static i32 json_parse_array(str& text, DOM* nodes, i32 proot, i32 pleft) {
    auto parr = nodes->add(proot, pleft, Node::make_array());

    text = text.slice(1u, u32(text.count) - 1);
    if (json_peek_char(text) == ']') {
        json_parse_char(text);
        return parr;
    }

    auto prev_val = -1;
    while (true) {
        auto this_val = json_parse_any(text, nodes, parr, prev_val);
        prev_val = this_val;

        auto next_char = json_parse_char(text);
        if (next_char == ']') {
            return parr;
        }
        if (next_char == ',') {
            continue;
        }
        return -1;
    }
}

static i32 json_parse_object(str& text, DOM* nodes, i32 proot, i32 pleft) {
    // add new node
    auto pobj = nodes->add(proot, pleft, Node::make_object());

    text = text.slice(1, -1);
    if (json_peek_char(text) == '}') {
        return pobj;
    }

    auto prev_key = -1;
    auto prev_val = -1;

    while (true) {
        // key
        const auto next_quota  = json_peek_char(text);
        if (next_quota != '"') {
            return -1;
        }
        const auto this_key  = json_parse_key(text, nodes, pobj, prev_key);

        const auto next_colon = json_parse_char(text);
        if (next_colon != ':') {
            io::log::error("nms.serialization.json.parse_object: expect ':', but '{:c}' ", next_colon);
            return -1;
        }
        const auto this_val  = json_parse_any(text, nodes, -1, prev_val);
        (void)this_val;

        prev_key = this_key;
        prev_val = this_val;

        const auto next_char = json_parse_char(text);
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

static i32 json_parse_any(str& text, DOM* nodes, i32 proot, i32 pleft) {
    auto result = -1;

    const auto text_len    = text.count;

    // peek next char
    const auto next_char = json_peek_char(text);

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
        result = json_parse_array(text, nodes, proot, pleft);
        break;
    }

    case '{':
    {
        result = json_parse_object(text, nodes, proot, pleft);
        break;
    }

    case '"':
    {
        result = json_parse_string(text, nodes, proot, pleft);
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
        result = json_parse_number(text, nodes, proot, pleft);
        break;
    }

    default:
        break;
    }

    return result;
}

// wraper
NMS_API void DOM::_parse_json(const str& text, i32 root_idx, i32 prev_idx) {
    auto str = text;
    json_parse_any(str, this, root_idx, prev_idx);
}

#pragma endregion

#pragma region unittest

struct Item
{
    NMS_REFLECT_BEGIN;
    typedef String<32>    NMS_MEMBER(name);
    typedef f32x3         NMS_MEMBER(size);
    NMS_REFLECT_END;
};

struct Items
{
    NMS_REFLECT_BEGIN;
    typedef u32         NMS_MEMBER(items_count);
    typedef time        NMS_MEMBER(product_date);
    typedef Vec<Item,2> NMS_MEMBER(items);
    NMS_REFLECT_END;
};

nms_test(json_serialize) {
    Items items = {
        2, 
        time{1945, 8, 9, 11, 2, 30},
        {{ {"samll", f32x3{1, 1, 1}},
           {"big",   f32x3{2, 2, 2}}}}
    };

    auto tree = Tree<64>::from_object(items);

    io::log::info("obj  = \n{}", items);
    io::log::info("json = \n{:json}", tree);
}

nms_test(json_deserialize) {
    const char text[] = R"(
{
    "items_count": 2,
    "product_date": "1945-08-09T11:02:45",
    "items": [
        { "name": "small", "size": [1, 1, 1]},
        { "name": "big",   "size": [2, 2, 2]},
    ]
}
)";
    // json_str -> json_tree
    auto dom = Tree<32>::from_json(text);

    Items items;
    dom.deserialize(items);
    io::log::debug("obj = \n{}", items);
}

#pragma endregion

}
