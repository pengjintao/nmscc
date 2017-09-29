#include <nms/test.h>
#include <nms/serialization/dom.h>

namespace nms::serialization
{

#pragma region format: xml

NMS_API void DOM::_format_xml_begin(IString& outbuf) const {
    const auto& this_node = node;
    outbuf += "<xml? version=\"1.0\" encoding=\"utf-8\" ?>\n";
    nms::sformat(outbuf, "<root type={}>", this_node.type);
}

NMS_API void DOM::_format_xml_end(IString& outbuf) const {
    outbuf += "</root>";
}

NMS_API void DOM::_format_xml_body(IString& outbuf, u32 indent_level) const {
    static const auto $indent_size = 4;
    const auto& this_node = this->node;

    switch (this_node.type) {
    case NodeType::$null:   nms::sformat(outbuf, FormatStyle{}, "");                break;
    case NodeType::$bool:   nms::sformat(outbuf, FormatStyle{}, this_node.$bool);   break;
    case NodeType::$u8:     nms::sformat(outbuf, FormatStyle{}, this_node.$u8 );    break;
    case NodeType::$i8:     nms::sformat(outbuf, FormatStyle{}, this_node.$i8 );    break;
    case NodeType::$u16:    nms::sformat(outbuf, FormatStyle{}, this_node.$u16);    break;
    case NodeType::$i16:    nms::sformat(outbuf, FormatStyle{}, this_node.$i16);    break;
    case NodeType::$u32:    nms::sformat(outbuf, FormatStyle{}, this_node.$u32);    break;
    case NodeType::$i32:    nms::sformat(outbuf, FormatStyle{}, this_node.$i32);    break;
    case NodeType::$u64:    nms::sformat(outbuf, FormatStyle{}, this_node.$u64);    break;
    case NodeType::$i64:    nms::sformat(outbuf, FormatStyle{}, this_node.$i64);    break;
    case NodeType::$f32:    nms::sformat(outbuf, FormatStyle{}, this_node.$f32);    break;
    case NodeType::$f64:    nms::sformat(outbuf, FormatStyle{}, this_node.$f64);    break;

    case NodeType::$time:
        DateTime::from_stamp(this_node.$time).sformat(outbuf, {});
        break;

    case NodeType::$num: {
        nms::sformat(outbuf, FormatStyle{}, str{ this_node.$str, this_node.size });
        break;
    }

    case NodeType::$key: case NodeType::$str: {
        outbuf += "\"";
        nms::sformat(outbuf, FormatStyle{}, str{ this_node.$str, this_node.size });
        outbuf += "\"";
        break;
    }

    case NodeType::$array: {
        outbuf += "\n";
        auto element_idx = 0;
        auto element_cnt = this_node.size;
        for (auto itr = begin(); element_idx < element_cnt; ++element_idx, ++itr) {
            const auto element = *itr;
            nms::sformat(outbuf, "{*: }<{} type=\"{}\">", (indent_level + 1)*$indent_size, element_idx, element.type);
            element._format_xml_body(outbuf, indent_level + 1);

            if (element.type == NodeType::$array || element.type == NodeType::$object) {
                outbuf.appends((indent_level + 1)*$indent_size, ' ');
            }
            nms::sformat(outbuf, "</{}>\n", element_idx);
        }
        break;
    }
    case NodeType::$object: {
        outbuf += "\n";
        auto element_idx = 0u;
        auto element_cnt = this_node.size;
        for (auto itr = begin(); element_idx < element_cnt; ++element_idx, ++itr) {
            const auto element = *itr;
            const auto key     = itr.key();
            nms::sformat(outbuf, "{*: }<{} type=\"{}\">", (indent_level + 1)*$indent_size, key, element.type);
            element._format_xml_body(outbuf, indent_level + 1);
            if (element.type == NodeType::$array || element.type == NodeType::$object) {
                outbuf.appends((indent_level + 1)*$indent_size, ' ');
            }
            nms::sformat(outbuf, "</{}>\n", (indent_level + 1)*$indent_size, element_idx);
        }
        break;
    }
    default:
        break;
    }
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
    typedef Vec<Item, 2> NMS_MEMBER(items);
    NMS_REFLECT_END;
};

nms_test(xml_serialize) {
    Items items ={
        2,
        time{ 1945, 8, 9, 11, 2, 30 },
        { { { "samll", f32x3{ 1, 1, 1 } },
            { "big",   f32x3{ 2, 2, 2 } } } }
    };

    auto tree = Tree<64>::from_object(items);

    io::log::info("obj = \n{}", items);
    io::log::info("xml = \n{:xml}", tree);
}

#pragma endregion

}