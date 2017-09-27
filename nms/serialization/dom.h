#pragma once

#include <nms/serialization/base.h>

namespace  nms::serialization
{

struct Node
{
    friend struct DOM;
    friend struct NodeIterator;

    using Tsize = u16;
    using Tnext = i32;
    using cstr  = const char*;

public:
    Type    type;           // 2 byte
    Tsize   size;           // 2 byte
    Tnext   next;           // 4 byte

    union                   // 8byte
    {
        void*   $val;
        bool    $bool;
        i8      $i8;
        i16     $i16;
        i32     $i32;
        i64     $i64;

        u8      $u8;
        u16     $u16;
        u32     $u32;
        u64     $u64;

        f32     $f32;
        f64     $f64;
        i64     $time;
        cstr    $num;
        cstr    $str;
        cstr    $key;
        Node*   $arr;
        Node*   $obj;
    };

public:
    static Node from_bool(bool val) { Node ret = { Type::$bool, 0, 0, nullptr }; ret.$bool = val;       return ret; }
    static Node from_num (u8   val) { Node ret = { Type::$u8,   0, 0, nullptr }; ret.$u8   = val;       return ret; }
    static Node from_num (i8   val) { Node ret = { Type::$i8,   0, 0, nullptr }; ret.$i8   = val;       return ret; }
    static Node from_num (u16  val) { Node ret = { Type::$u16,  0, 0, nullptr }; ret.$u16  = val;       return ret; }
    static Node from_num (i16  val) { Node ret = { Type::$i16,  0, 0, nullptr }; ret.$i16  = val;       return ret; }
    static Node from_num (u32  val) { Node ret = { Type::$u32,  0, 0, nullptr }; ret.$u32  = val;       return ret; }
    static Node from_num (i32  val) { Node ret = { Type::$i32,  0, 0, nullptr }; ret.$i32  = val;       return ret; }
    static Node from_num (u64  val) { Node ret = { Type::$u64,  0, 0, nullptr }; ret.$u64  = val;       return ret; }
    static Node from_num (i64  val) { Node ret = { Type::$i64,  0, 0, nullptr }; ret.$i64  = val;       return ret; }
    static Node from_num (f32  val) { Node ret = { Type::$f32,  0, 0, nullptr }; ret.$f32  = val;       return ret; }
    static Node from_num (f64  val) { Node ret = { Type::$f64,  0, 0, nullptr }; ret.$f64  = val;       return ret; }
    static Node from_time(time val) { Node ret = { Type::$time, 0, 0, nullptr }; ret.$i64  = val.stamp; return ret; }

    static Node from_num (str  val) { Node ret = { Type::$num, Tsize(val.count), 0, nullptr }; ret.$num = val.data; return ret; }
    static Node from_str (str  val) { Node ret = { Type::$str, Tsize(val.count), 0, nullptr }; ret.$str = val.data; return ret; }
    static Node from_key (str  val) { Node ret = { Type::$str, Tsize(val.count), 0, nullptr }; ret.$key = val.data; return ret; }

    static Node make_null()   { Node ret = { Type::$null,   0, 0, nullptr }; return ret; }
    static Node make_array()  { Node ret = { Type::$array,  0, 0, nullptr }; return ret; }
    static Node make_object() { Node ret = { Type::$object, 0, 0, nullptr }; return ret; }

    __declspec(property(get=get_text)) str text;
    str get_text() const {
        if (type == Type::$null) {
            return {};
        }

        if ((type != Type::$str) && (type != Type::$key) && (type != Type::$num) ) {
            NMS_THROW(Eunexpect<Type>{Type::$str, type});
        }
        return { $str, size};
    }

    template<class T>
    void get_num(T& val, Type expect) const {
        if (type != expect) {
            if (type == Type::$num) {

            }
            else {
                NMS_THROW(Eunexpect<Type>{Type::$num, type});
            }
        }
        val = *reinterpret_cast<const T*>(&val);
    }

    void get_val(bool&  val) const {
        if (type == Type::$bool) {
            val = $bool;
        }
        NMS_THROW(Eunexpect<Type>{Type::$bool, type});
    }

    void get_val(str&   val) const {
        if (type == Type::$str) {
            val = str{ $str, size };
        }
        NMS_THROW(Eunexpect<Type>{Type::$str, type});
    }

    void get_val(time&  val) const {
        if (type == Type::$time) {
            val = time::from_stamp($time);
        }
        else if (type == Type::$str) {
            auto time_str = str{ $str, size };
            if (time::parse(time_str, {}, val)) {
                const_cast<Type&>(this->type)  = Type::$time;
                const_cast<i64&> (this->$time) = val.stamp;
            }
        }
        NMS_THROW(Eunexpect<Type>{Type::$str, type});
    }
};

struct DOM
{
    using Tsize = typename Node::Tsize;

    IList<Node>*    nodes;
    i32             index;

#pragma region iterator
    struct Iterator
    {
        IList<Node>*    nodes;
        i32             index;

        Iterator& operator++() {
            auto& element = (*nodes)[index];
            if (element.next != 0) {
                index += element.next;
            }
            else {
                index = 0;
            }
            return *this;
        }

        Iterator operator++(int) {
            auto self = *this;
            auto element = (*nodes)[index];
            if (element.next != 0) {
                index += element.next;
            }
            else {
                index = 0;
            }
            return self;
        }

        str key() const {
            auto& element = (*nodes)[index - 1];
            if (element.type != Type::$key) {
                NMS_THROW(Eunexpect<Type>{Type::$key, element.type });
            }
            return { element.$key, element.size };
        }

        DOM operator*() const {
            return { nodes, index };
        }

        DOM operator->() const {
            return { nodes, index };
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.index == rhs.index;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return lhs.index != rhs.index;
        }
    };

    /* iterator: begin */
    Iterator begin() const {
    #ifdef NMS_CC_MSVC
    #pragma warning(push)
    #pragma warning(disable: 4061)
    #endif
        switch (node.type) {
        case Type::$null:
            return { nodes, 0 };

        case Type::$array:
            return { nodes, index + 1 };

        case Type::$object:
            return { nodes, index + 2 };

        default:
            break;

        }
    #ifdef NMS_CC_MSVC
    #pragma warning(pop)
    #endif
        NMS_THROW(Eunexpect<Type>{Type::$array, node.type});
    }

    /* iterator: end */
    Iterator end() const {
        return { nodes, 0 };
    }

#pragma endregion

#pragma region property
    __declspec(property(get=get_node, put=set_node)) Node node;

    const Node& get_node() const {
        return (*nodes)[index];
    }

    void set_node(Node val) {
        (*nodes)[index] = val;
    }

    __declspec(property(get=get_type)) Type type;
    Type get_type() const {
        return node.type;
    }

    __declspec(property(get=get_size)) Tsize size;
    __declspec(property(get=get_size)) Tsize count;
    Tsize get_size() const {
        return node.size;
    }

    /* get key */
    str key() const {
        auto k = (*nodes)[index - 1];
        if (k.type != Type::$key) {
            NMS_THROW(Eunexpect<Type>{Type::$key, k.type });
        }
        str val = { k.$str, k.size };
        return val;
    }
#pragma endregion

#pragma region deserialize
    void deserialize(bool&  val) const { node.get_val(val); }
    void deserialize(str&   val) const { node.get_val(val); }
    void deserialize(time&  val) const { node.get_val(val); }
    void deserialize(i8&    val) const { node.get_num(val, Type::$i8);     }
    void deserialize(u8&    val) const { node.get_num(val, Type::$u8);     }
    void deserialize(i16&   val) const { node.get_num(val, Type::$i16);    }
    void deserialize(u16&   val) const { node.get_num(val, Type::$u16);    }
    void deserialize(i32&   val) const { node.get_num(val, Type::$i32);    }
    void deserialize(u32&   val) const { node.get_num(val, Type::$u32);    }
    void deserialize(i64&   val) const { node.get_num(val, Type::$i64);    }
    void deserialize(u64&   val) const { node.get_num(val, Type::$u64);    }
    void deserialize(f32&   val) const { node.get_num(val, Type::$f32);    }
    void deserialize(f64&   val) const { node.get_num(val, Type::$f64);    }

    template<class T, class=decltype(T::_$member_cnt)>
    void deserialize(T& val) const
    { }

#pragma endregion

#pragma region serialize
    void serialize(bool val) { node = Node::from_bool(val); }
    void serialize(i8   val) { node = Node::from_num (val); }
    void serialize(u8   val) { node = Node::from_num (val); }
    void serialize(i16  val) { node = Node::from_num (val); }
    void serialize(u16  val) { node = Node::from_num (val); }
    void serialize(i32  val) { node = Node::from_num (val); }
    void serialize(u32  val) { node = Node::from_num (val); }
    void serialize(i64  val) { node = Node::from_num (val); }
    void serialize(u64  val) { node = Node::from_num (val); }
    void serialize(f32  val) { node = Node::from_num (val); }
    void serialize(f64  val) { node = Node::from_num (val); }
    void serialize(str  val) { node = Node::from_str (val); }
    void serialize(time val) { node = Node::from_time(val); }

    template<class T, class=decltype(T::_$member_cnt)>
    void serialize(const T& val) {
    }

#pragma endregion

#pragma region array
    /* array: index */
    NMS_API DOM operator[](u32 k);

    /* array: index */
    NMS_API DOM operator[](u32 k)     const;

#pragma endregion

#pragma region object
    /* object: index */
    NMS_API DOM operator[](str k);

    /* object: index */
    NMS_API DOM operator[](str k) const;

    /* object: find */
    NMS_API Iterator find(str) const;

    /* object: index */
    template<u32 N>
    DOM operator[](const char(&s)[N]) {
        return (*this)[str(s)];
    }

    /* object: index */
    template<u32 N>
    DOM operator[](const char(&s)[N]) const {
        return (*this)[str(s)];
    }
#pragma endregion

#pragma region method
    u32 strlen() const {
        if (nodes->count == 0) {
            return 0;
        }
        return (*nodes)[0].size;
    }

    NMS_API i32 add(i32 root, i32 prev, const Node& val);

    NMS_API i32 add(i32 root, i32 prev, const str& key, const Node& val);
#pragma endregion

#pragma region format
    void sformat(IString& buf, const str& data_fmt) const {
        if (data_fmt == "xml") {
            sformat(buf, $xml);
        }
        else {
            sformat(buf, $json);
        }
    }

    void sformat(IString& buf, DOMType data_fmt) const {
        switch (data_fmt) {
        case $json:
            _format_json(buf);
            break;
        case $xml:
            _format_xml_begin(buf);
            _format_xml_body(buf);
            _format_xml_end(buf);
            break;
        }
    }

    NMS_API void _format_json(IString& buf, u32 level = 0) const;
private:
    NMS_API void _format_xml_begin(IString& buf) const;
    NMS_API void _format_xml_end  (IString& buf) const;
    NMS_API void _format_xml_body (IString& buf, u32 level = 0) const;

#pragma endregion

#pragma region parse
public:
    void _parse(const str& str, DOMType type) {
        switch (type) {
        case $json:
            _parse_json(str);
            break;
        case $xml:
        default:
            break;
        }
    }

    NMS_API void _parse_json(const str& json);

#pragma endregion

};

template<u32 Icapicity=0>
class Tree
{
public:
    constexpr static const auto $capicity = Icapicity;
    using Tdata = List<Node, $capicity>;

#pragma region constructor
    Tree()
        : nodes_{}, dom_{&nodes_, 0}
    {}

    ~Tree()
    {}

    Tree(Tree&& rhs) noexcept
        : nodes_{ move(rhs.nodes_) }, dom_{ &nodes_, rhs.dom_.index }
    {}

    Tree(const Tree&)               = delete;
    Tree& operator=(Tree&&)         = delete;
    Tree& operator=(const Tree&)    = delete;

    template<class T, class=decltype(T::_$member_cnt) >
    static Tree from_object(const T& obj) {
        Tree tree;
        tree.dom_.serialize(obj);
        return tree;
    }

    static Tree from_json(const str& json) {
        Tree tree;
        tree.dom_._parse_json(json);
        return tree;
    }
#pragma endregion

#pragma region serialize/deserialize
    template<class T>
    void deserialize(T& val) const {
        dom_.deserialize(val);
    }

    template<class T>
    void serialize(const T& val) {
        dom_.serialize(val);
    }
#pragma endregion

#pragma region format/parse
    void sformat(IString& buf, const FormatStyle& style) const {
        dom_.sformat(buf, str{ style.spec, strlen(style.spec) });
    }

    void format(IString& buf, DOMType type) const {
        dom_.sformat(buf, type);
    }
#pragma endregion

protected:
    Tdata nodes_;
    DOM   dom_;
};

}
