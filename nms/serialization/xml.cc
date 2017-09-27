#include <nms/test.h>
#include <nms/serialization/dom.h>

namespace nms::serialization
{

#pragma region format: xml

NMS_API void DOM::_format_xml_begin(IString& buf) const {
    const auto& v = node;
    buf += "<xml? version=\"1.0\" encoding=\"utf-8\" ?>\n";
    nms::sformat(buf, "<root type={}>", v.type);
}

NMS_API void DOM::_format_xml_end(IString& buf) const {
    buf += "</root>";
}

NMS_API void DOM::_format_xml_body(IString& buf, u32 level) const {
    static const auto $indent = 2;
    const auto& v = this->node;

    switch (v.type) {
    case Type::$null:   nms::sformat(buf, FormatStyle{}, "");        break;
    case Type::$bool:   nms::sformat(buf, FormatStyle{}, v.$bool);   break;
    case Type::$u8:     nms::sformat(buf, FormatStyle{}, v.$u8 );    break;
    case Type::$i8:     nms::sformat(buf, FormatStyle{}, v.$i8 );    break;
    case Type::$u16:    nms::sformat(buf, FormatStyle{}, v.$u16);    break;
    case Type::$i16:    nms::sformat(buf, FormatStyle{}, v.$i16);    break;
    case Type::$u32:    nms::sformat(buf, FormatStyle{}, v.$u32);    break;
    case Type::$i32:    nms::sformat(buf, FormatStyle{}, v.$i32);    break;
    case Type::$u64:    nms::sformat(buf, FormatStyle{}, v.$u64);    break;
    case Type::$i64:    nms::sformat(buf, FormatStyle{}, v.$i64);    break;
    case Type::$f32:    nms::sformat(buf, FormatStyle{}, v.$f32);    break;
    case Type::$f64:    nms::sformat(buf, FormatStyle{}, v.$f64);    break;

    case Type::$time:
        DateTime::from_stamp(v.$time).sformat(buf, {});
        break;

    case Type::$num: {
        nms::sformat(buf, FormatStyle{}, str{ v.$str, v.size });
        break;
    }

    case Type::$key: case Type::$str: {
        buf += "\"";
        nms::sformat(buf, FormatStyle{}, str{ v.$str, v.size });
        buf += "\"";
        break;
    }

    case Type::$array: {
        buf += "\n";
        buf.appends(level*$indent, ' ');

        auto k = 0u;
        for (auto itr = begin(); itr != end(); ++k, ++itr) {
            const auto element = *itr;
            nms::sformat(buf, "{ }<{} type=\"{}\">", (level + 1) * $indent, k, element.type);
            element._format_xml_body(buf, level + 1);
            nms::sformat(buf, "</{}>", k);
            buf += "\n";
            buf.appends(level*$indent, ' ');
        }
        break;
    }
    case Type::$object: {
        buf += "\n";
        buf.appends(level*$indent, ' ');

        auto k = 0u;
        for (auto itr = begin(); itr != end(); ++k, ++itr) {
            const auto element = *itr;
            const auto key     = itr.key();
            nms::sformat(buf, "{ }<{} type=\"{}\">", $indent, key, element.type);
            element._format_xml_body(buf, level + 1);
            nms::sformat(buf, "</{}>", key);
            buf += "\n";
            buf.appends(level*$indent, ' ');
        }
        break;
    }
    default:
        break;
    }
}
#pragma endregion

#pragma region unittest
struct TestObject
{
    NMS_REFLECT_BEGIN;
    typedef String<32>  NMS_MEMBER(a);
    typedef f32x4       NMS_MEMBER(b);
    typedef DateTime    NMS_MEMBER(c);
    NMS_REFLECT_END;
};

nms_test(xml_serialize) {
    TestObject obj;
    obj.a = "hello";
    obj.b ={ 1.1f, +2.2f, -3.3f, 4.4e2f };
    obj.c = DateTime(2017, 9, 3, 8, 30, 12);

    Tree<64> node;
    //node << obj;

    io::log::info("obj = {}", obj);
    io::log::info("xml = {:xml}", node);
}

#pragma endregion

}