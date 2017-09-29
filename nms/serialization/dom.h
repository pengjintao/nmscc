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
    NodeType    type;           // 2 byte
    Tsize   size;           // 2 byte
    Tnext   next;           // 4 byte

    union                   // 8byte
    {
        const void* $val;
        bool        $bool;
        i8          $i8;
        i16         $i16;
        i32         $i32;
        i64         $i64;

        u8          $u8;
        u16         $u16;
        u32         $u32;
        u64         $u64;

        f32         $f32;
        f64         $f64;
        i64         $time;
        cstr        $num;
        cstr        $str;
        cstr        $key;
        Node*       $arr;
        Node*       $obj;
    };

public:
    static Node from_bool(bool val) { Node ret = { NodeType::$bool, 0, 0, nullptr }; ret.$bool = val;       return ret; }
    static Node from_num (u8   val) { Node ret = { NodeType::$u8,   0, 0, nullptr }; ret.$u8   = val;       return ret; }
    static Node from_num (i8   val) { Node ret = { NodeType::$i8,   0, 0, nullptr }; ret.$i8   = val;       return ret; }
    static Node from_num (u16  val) { Node ret = { NodeType::$u16,  0, 0, nullptr }; ret.$u16  = val;       return ret; }
    static Node from_num (i16  val) { Node ret = { NodeType::$i16,  0, 0, nullptr }; ret.$i16  = val;       return ret; }
    static Node from_num (u32  val) { Node ret = { NodeType::$u32,  0, 0, nullptr }; ret.$u32  = val;       return ret; }
    static Node from_num (i32  val) { Node ret = { NodeType::$i32,  0, 0, nullptr }; ret.$i32  = val;       return ret; }
    static Node from_num (u64  val) { Node ret = { NodeType::$u64,  0, 0, nullptr }; ret.$u64  = val;       return ret; }
    static Node from_num (i64  val) { Node ret = { NodeType::$i64,  0, 0, nullptr }; ret.$i64  = val;       return ret; }
    static Node from_num (f32  val) { Node ret = { NodeType::$f32,  0, 0, nullptr }; ret.$f32  = val;       return ret; }
    static Node from_num (f64  val) { Node ret = { NodeType::$f64,  0, 0, nullptr }; ret.$f64  = val;       return ret; }
    static Node from_time(time val) { Node ret = { NodeType::$time, 0, 0, nullptr }; ret.$i64  = val.stamp; return ret; }

    static Node from_num (str  val) { Node ret = { NodeType::$num, Tsize(val.count), 0, val.data }; return ret; }
    static Node from_str (str  val) { Node ret = { NodeType::$str, Tsize(val.count), 0, val.data }; return ret; }
    static Node from_key (str  val) { Node ret = { NodeType::$key, Tsize(val.count), 0, val.data }; return ret; }

    static Node make_null()   { Node ret = { NodeType::$null,   0, 0, nullptr }; return ret; }
    static Node make_array()  { Node ret = { NodeType::$array,  0, 0, nullptr }; return ret; }
    static Node make_object() { Node ret = { NodeType::$object, 0, 0, nullptr }; return ret; }

    __declspec(property(get=get_text)) str text;
    str get_text() const {
        if (type == NodeType::$null) {
            return {};
        }

        if ((type != NodeType::$str) && (type != NodeType::$key) && (type != NodeType::$num) ) {
            NMS_THROW(Eunexpect<NodeType>{NodeType::$str, type});
        }
        return { $str, size};
    }

    template<class T>
    void get_num(T& val, NodeType expect) const {
        if (type != expect) {
            if (type == NodeType::$num) {
                auto num_str = str{ $num, size };
                if (str_cast(num_str, val)) {
                    const_cast<NodeType&>(this->type) = expect;
                    reinterpret_cast<T&>(const_cast<u64&>(this->$u64)) = val;
                    return;
                }
            }
            NMS_THROW(Eunexpect<NodeType>{NodeType::$num, type});
        }
        val = *reinterpret_cast<const T*>(&val);
    }

    void get_val(bool&  val) const {
        if (type == NodeType::$bool) {
            val = $bool;
            return;
        }
        NMS_THROW(Eunexpect<NodeType>{NodeType::$bool, type});
    }

    void get_val(str&   val) const {
        if (type == NodeType::$str) {
            val = str{ $str, size };
            return;
        }
        NMS_THROW(Eunexpect<NodeType>{NodeType::$str, type});
    }

    void get_val(time&  val) const {
        if (type == NodeType::$time) {
            val = time::from_stamp($time);
            return;
        }
        else if (type == NodeType::$str) {
            auto time_str = str{ $str, size };
            if (time::parse(time_str, {}, val)) {
                const_cast<NodeType&>(this->type)   = NodeType::$time;
                const_cast<i64&> (this->$time)      = val.stamp;
                return;
            }
        }
        NMS_THROW(Eunexpect<NodeType>{NodeType::$str, type});
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
            if (element.type != NodeType::$key) {
                NMS_THROW(Eunexpect<NodeType>{NodeType::$key, element.type });
            }
            return { element.$key, element.size };
        }

        DOM operator*() const {
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
        case NodeType::$null:
            return { nodes, 0 };

        case NodeType::$array:
            return { nodes, index + 1 };

        case NodeType::$object:
            return { nodes, index + 2 };

        default:
            break;

        }
    #ifdef NMS_CC_MSVC
    #pragma warning(pop)
    #endif
        NMS_THROW(Eunexpect<NodeType>{NodeType::$array, node.type});
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

    __declspec(property(get=get_type)) NodeType type;
    NodeType get_type() const {
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
        if (k.type != NodeType::$key) {
            NMS_THROW(Eunexpect<NodeType>{NodeType::$key, k.type });
        }
        str val = { k.$str, k.size };
        return val;
    }
#pragma endregion

#pragma region deserialize
    void deserialize(bool&  val) const { node.get_val(val); }
    void deserialize(str&   val) const { node.get_val(val); }
    void deserialize(time&  val) const { node.get_val(val); }
    void deserialize(i8&    val) const { node.get_num(val, NodeType::$i8);     }
    void deserialize(u8&    val) const { node.get_num(val, NodeType::$u8);     }
    void deserialize(i16&   val) const { node.get_num(val, NodeType::$i16);    }
    void deserialize(u16&   val) const { node.get_num(val, NodeType::$u16);    }
    void deserialize(i32&   val) const { node.get_num(val, NodeType::$i32);    }
    void deserialize(u32&   val) const { node.get_num(val, NodeType::$u32);    }
    void deserialize(i64&   val) const { node.get_num(val, NodeType::$i64);    }
    void deserialize(u64&   val) const { node.get_num(val, NodeType::$u64);    }
    void deserialize(f32&   val) const { node.get_num(val, NodeType::$f32);    }
    void deserialize(f64&   val) const { node.get_num(val, NodeType::$f64);    }

#pragma region deserialize: array
    template<class T, u32 N>
    void deserialize(Vec<char, N>& vec) const {
        auto text = node.get_text();;
        vec = text;
    }

    template<class T, u32 N>
    void deserialize(Vec<T, N>& vec) const {
        auto element_idx = 0u;
        auto element_cnt = this->size;
        for (auto itr = begin(); element_idx < element_cnt; ++element_idx, ++itr) {
            const auto element = *itr;
            element.deserialize(vec[element_idx]);
        }
    }

    void deserialize(IList<char>& list) const {
        str ret;
        this->deserialize(ret);
        list = ret;
    }

    template<class T>
    void deserialize(IList<T>& list) const {
        auto element_idx = 0u;
        auto element_cnt = this->size;
        for (auto itr = begin(); element_idx < element_cnt; ++element_idx, ++itr) {
            list.resize(list.size + 1);
            (*itr).deserialize(list[element_idx]);
        }
    }
#pragma endregion

#pragma region deserialize: object
    template<class T, class=decltype(T::_$member_cnt)>
    void deserialize(T& obj) const {
        this->_deserialize_reflect_index(obj, Tu32<0>{}, Tu32<T::_$member_cnt>{});
    }

    template<class T, u32 Idx>
    void _deserialize_reflect_index(T& obj, Tu32<Idx>, Tu32<1>) const {
        this->_deserialize_reflect_member<Idx>(obj);
    }

    template<class T, u32 Idx, u32 Iver>
    void _deserialize_reflect_index(T& obj, Tu32<Idx>, Tu32<Iver>) const {
        this->_deserialize_reflect_member<Idx>(obj);
        this->_deserialize_reflect_index(obj, Tu32<Idx + 1>{}, Tu32<Iver - 1>{});
    }

    template<u32 Idx, typename T>
    void _deserialize_reflect_member(T& obj) const {
        using Tmember = typename Tmembers<T>::type<Idx>;
        const auto  member_name = Tmember::name();
        auto& member_value      = Tmember::value(obj);

        auto itr = this->find(member_name);
        if (itr.index != 0) {
            (*itr).deserialize(member_value);
        }
    }
#pragma endregion

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

#pragma region serialize: array
    void serialize_array(const char array[], u32 count) {
        serialize(str{ array, count });
    }

    template<class T>
    void serialize_array(const T array[], u32 count) {
        node = Node::make_array();

        auto prev_idx = this->index;
        for (auto i = 0u; i < count; ++i) {
            auto element_idx = this->add(this->index, prev_idx, Node::make_null());
            auto element_node= DOM{ nodes, element_idx };
            element_node.serialize(array[i]);
            prev_idx = element_idx;
        }
    }

    template<class T, u32 N>
    void serialize(const Vec<T,N>& vec) {
        serialize_array(vec.data, vec.count);
    }

    template<class T>
    void serialize(const View<T, 0>& view) {
        serialize_array(view.data, view.count);
    }
#pragma endregion

#pragma region serialize: object
    template<class T, class=decltype(T::_$member_cnt)>
    void serialize(const T& obj) {
        node = Node::make_object();
        this->_serialize_reflect_index(obj, 0, Tu32<0>{}, Tu32<T::_$member_cnt>{});
    }

    template<class T, u32 Idx>
    void _serialize_reflect_index(const T& obj, i32 prev_id, Tu32<Idx>, Tu32<1>) {
        this->_serialize_reflect_member<Idx>(obj, prev_id);
    }

    template<class T, u32 Idx, u32 Iver>
    void _serialize_reflect_index(const T& obj, i32 prev_id, Tu32<Idx>, Tu32<Iver>) {
        const auto this_id = this->_serialize_reflect_member<Idx>(obj, prev_id);
        this->_serialize_reflect_index(obj, this_id, Tu32<Idx + 1>{}, Tu32<Iver - 1>{});
    }

    template<u32 Idx, typename T>
    i32 _serialize_reflect_member(const T& obj, i32 prev_id) {
        using Tmember = typename Tmembers<T>::type<Idx>;
        const auto  member_name  = Tmember::name();
        const auto& member_value = Tmember::value(obj);

        auto member_id = this->add(index, prev_id, member_name, Node::make_null());
        auto member_dom= DOM{ nodes, member_id };
        member_dom.serialize(member_value);

        return member_id;
    }
#pragma endregion

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

    NMS_API i32 add(i32 root_idx, i32 prev_idx, const Node& val);
    NMS_API i32 add(i32 root_idx, i32 prev_idx, const str&  key, const Node& val);
#pragma endregion

#pragma region format
    void sformat(IString& outbuf, const FormatStyle& fmt_style) const {
        const auto spec = fmt_style.spec;
        if (spec == "xml") {
            sformat(outbuf, $xml);
        }
        else {
            sformat(outbuf, $json);
        }
    }

    void sformat(IString& outbuf, DOMType fmt_type) const {
        switch (fmt_type) {
        case $json:
            _format_json(outbuf);
            break;
        case $xml:
            _format_xml_begin(outbuf);
            _format_xml_body(outbuf);
            _format_xml_end(outbuf);
            break;
        }
    }

private:
    NMS_API void _format_json(IString& outbuf, u32 indent_level = 0) const;

    NMS_API void _format_xml_begin(IString& outbuf) const;
    NMS_API void _format_xml_end  (IString& outbuf) const;
    NMS_API void _format_xml_body (IString& outbuf, u32 indent_level = 0) const;

#pragma endregion

#pragma region parse
public:
    void parse(const str& str, DOMType type, i32 root_idx, i32 prev_idx) {
        switch (type) {
        case $json:
            _parse_json(str, root_idx, prev_idx);
            break;
        case $xml:
        default:
            break;
        }
    }

private:
    NMS_API void _parse_json(const str& json, i32 root_idx, i32 prev_idx);

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
        : nodes_{}
    {}

    ~Tree()
    {}

    Tree(Tree&& rhs) noexcept
        : nodes_{ move(rhs.nodes_) }
    {}

    Tree(const Tree&)               = delete;
    Tree& operator=(Tree&&)         = delete;
    Tree& operator=(const Tree&)    = delete;

    template<class T, class=decltype(T::_$member_cnt) >
    static Tree from_object(const T& obj) {
        Tree tree;
        tree.nodes_ += Node::make_null();
        tree.nodes_ += Node::make_object();
        tree.dom.serialize(obj);
        return tree;
    }

    static Tree from_json(const str& json_text) {
        Tree tree;
        tree.dom.parse(json_text, $json, -1, -1); 
        return tree;
    }
#pragma endregion

#pragma region property
    __declspec(property(get=get_dom)) DOM dom;

    DOM get_dom() const {
        return DOM{ const_cast<Tdata*>(&nodes_), 1 };
    }
#pragma endregion

#pragma region serialize/deserialize
    template<class T>
    void deserialize(T& val) const {
        this->dom.deserialize(val);
    }

    template<class T>
    void serialize(const T& val) {
        this->dom.serialize(val);
    }
#pragma endregion

#pragma region format/parse
    void sformat(IString& buf, const FormatStyle& style) const {
        this->dom.sformat(buf,  style);
    }

    void format(IString& buf, DOMType type) const {
        this->dom.sformat(buf, type);
    }
#pragma endregion

protected:
    Tdata nodes_;
};

}
