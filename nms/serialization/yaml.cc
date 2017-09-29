
namespace nms::serialization
{

#pragma region format:json
NMS_API void XDOM::_format_yaml(IString& buf, u32 level) const {
    static const auto $indent = 4;

    auto& v = val();

    switch (v.type()) {
    case Type::null:    nms::_format(buf, {}, "");              break;
    case Type::boolean: nms::_format(buf, {}, v.bool_val_);     break;
    case Type::u8:      nms::_format(buf, {}, v.u8_val_);       break;
    case Type::i8:      nms::_format(buf, {}, v.i8_val_);       break;
    case Type::u16:     nms::_format(buf, {}, v.u16_val_);      break;
    case Type::i16:     nms::_format(buf, {}, v.i16_val_);      break;
    case Type::u32:     nms::_format(buf, {}, v.u32_val_);      break;
    case Type::i32:     nms::_format(buf, {}, v.i32_val_);      break;
    case Type::u64:     nms::_format(buf, {}, v.u64_val_);      break;
    case Type::i64:     nms::_format(buf, {}, v.i64_val_);      break;
    case Type::f32:     nms::_format(buf, {}, v.f32_val_);      break;
    case Type::f64:     nms::_format(buf, {}, v.f64_val_);      break;

    case Type::datetime: {
        nms::_format(buf, {}, DateTime(v.i64_val_));
        break;
    }
    case Type::number: {
        nms::_format(buf, {}, v.str());
        break;
    }

    case Type::key: case Type::string: {
        buf += "\"";
        nms::_format(buf, {}, v.str());
        buf += "\"";
        break;
    }

    case Type::array: {
        buf += "[\n";

        for (auto itr = begin(); itr != end(); ) {
            buf.appends((level+1)*$indent, ' ');
            (*itr)._format_json(buf, level + 1);
            _format(buf, {}, (++itr == end()) ? StrView{ "\n" } : StrView{ ",\n" });
        }

        buf.appends(level * $indent, ' ');
        buf += "]";
        break;
    }
    case Type::object: {
        buf += "{\n";

        for (auto itr = begin(); itr != end();) {
            buf.appends((level + 1)*$indent, ' ');
            buf += "\"";
            buf += itr.key();
            buf += "\": ";
            (*itr)._format_json(buf, level + 1);
            _format(buf, {}, (++itr == end()) ? StrView{ "\n" } : StrView{ ",\n" });
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

}
