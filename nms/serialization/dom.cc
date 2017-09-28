#include <nms/test.h>
#include <nms/serialization/dom.h>

namespace nms::serialization
{

NMS_API DOM DOM::operator[](u32 k) const {
    if (this->type != Type::$array) {
        NMS_THROW(Eunexpect<Type>{ Type::$array, this->type });
    }

    const auto n = this->count;
    if (k >= n) {
        NMS_THROW(Eoutofrange<Tsize>{ 0, n, Tsize(k) });
    }

    // find index
    auto itr = begin();
    for (auto i = 0u; i < k; ++i) {
        ++itr;
    }
    return *itr;
}

NMS_API DOM DOM::operator[](u32 k) {
    auto& node_list = *nodes;

    if (node_list.count == 0) {
        node_list.append(Node::make_null());
        node_list.append(Node::make_array());
        this->index = 1;
    }
    else if (this->type != Type::$array) {
        NMS_THROW(Eunexpect<Type>{ Type::$array, this->type });
    }

    // find index
    auto itr = begin();
    for (auto i = 0u; i < k; ++i) {
        // out of range, insert new val
        if (i >= this->count) {
            auto pnode = add(index, itr.index, Node::make_null());
            (void)pnode;
        }
        ++itr;
    }
    return *itr;
}

NMS_API DOM::Iterator DOM::find(str expect) const {
    if (this->type != Type::$object) {
        NMS_THROW(Eunexpect<Type>{ Type::$object, this->type });
    }

    auto n      = this->count;
    auto itr    = begin();
    for (u32 i = 0; i < n; ++i, ++itr) {
        auto key = itr.key();
        if (key == expect) {
            return { nodes, itr.index };
        }
    }
    return { nodes, 0 };
}

NMS_API DOM DOM::operator[](str key) const {
    auto itr = find(key);
    if (itr != end()) {
        return *itr;
    }

    NMS_THROW(Enotfound<str>{ key });
}


NMS_API DOM DOM::operator[](str key) {
    auto& node_list = *nodes;

    if (node_list.count == 0) {
        node_list.append(Node::make_null());
        node_list.append(Node::make_object());
        this->index = 1;
        auto idx = add(1, 0, key, Node::make_null());
        return { nodes, idx };
    }

    auto& v = this->node;
    if (this->type == Type::$null) {
        const_cast<Type&>(v.type) = Type::$object;
    }
    else if (this->type != Type::$object) {
        NMS_THROW(Eunexpect<Type>{ Type::$object, this->type });
    }

    auto n      = v.size;
    auto itr    = this->begin();
    auto last   = itr.index;

    for (u32 i = 0; i < n; ++i, ++itr) {
        last = itr.index;
        if (itr.key() == key) {
            return { nodes, itr.index };
        }
    }

    // not find, insert new key/val
    auto pval = add(index, last, key, Node::make_null());

    return DOM{ nodes, pval };
}

NMS_API i32 DOM::add(i32 root, i32 prev, const Node& val) {
    auto& node_list = *nodes;

    auto xpos = i32(node_list.count);

    if (xpos == 0) {
        node_list.append(Node::make_null());
        this->index = 1;
        ++xpos;
    }
    node_list.append(val);

    if (root > 0) {
        node_list[root].size += 1;
    }
    if (prev > 0) {
        node_list[prev].next = xpos - prev;
    }
    if (val.type == Type::$str) {
        node_list[0].size += val.size;
    }
    return i32(xpos);
}

NMS_API i32 DOM::add(i32 root, i32 prev, const str& key, const Node& val) {
    auto& node_list = *nodes;

    auto xpos = i32(node_list.count + 1);
    if (xpos == 1) {
        node_list.append(Node::make_null());
        this->index = 1;
    }

    node_list.append(Node::from_key(key));
    node_list.append(val);

    if (root > 0) {
        node_list[root].size++;
    }
    if (prev > 0) {
        const auto offset = xpos - prev;
        node_list[prev - 0].next = offset;
        node_list[prev - 1].next = offset;
    }
    node_list[0].size += Node::Tsize(key.count + val.size);
    return xpos;
}

}

