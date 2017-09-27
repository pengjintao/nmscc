#pragma once

#include <nms/core.h>

namespace nms::serialization
{

enum DOMType
{
    $json,
    $xml
};

struct Node;
struct DOM;

NMS_ENUM(Type, $null, $bool, $i8, $u8, $i16, $u16, $i32, $u32, $i64, $u64, $f32, $f64, $num, $str, $key, $time, $array, $object);

struct ISerializable
{
    friend struct DOM;

protected:
    template<class T>
    static void _serialize(DOM& node, const T& obj) {
#ifndef NMS_CC_INTELLISENSE
#define call_serialize_impl(n, ...)    _serialize_impl(Ti32<n>{}, &obj, &node);
        NMSCPP_LOOP(99, call_serialize_impl);
#undef call_serialize_impl
#endif
    }

    template<class T>
    static void _deserialize(const DOM& node, T& obj) {
#ifndef NMS_CC_INTELLISENSE
#define call_deserialize_impl(n, ...)    _deserialize_impl(Ti32<n>{}, &obj, &node);
        NMSCPP_LOOP(99, call_deserialize_impl);
#undef call_deserialize_impl
#endif
    }

private:
    // serialize-impl
    template<class T, i32 I>
    static auto _serialize_impl(Ti32<I> idx, const T* pobj, DOM* pnod)->$when<(I < T::_$property_cnt)> {
        auto obj_item = (*pobj)[idx];
        auto nod_item = (*pnod)[obj_item.name];
        nod_item << (*obj_item.pval);
        return;
    }

    // serialize-impl
    template<class T, i32 I >
    static auto _serialize_impl(Ti32<I>, const T* pobj, ...) -> $when<(I >= T::_$property_cnt)> {
        (void)pobj;
        return;
    }

    // deserialize-impl
    template<class T, i32 I>
    static auto _deserialize_impl(Ti32<I> idx, T* pobj, const DOM* pnod)->$when<(I < T::_$property_cnt)> {
        auto obj_item = (*pobj)[idx];
        auto nod_item = (*pnod)[obj_item.name];
        nod_item >> (*obj_item.pval);
        return;
    }

    // deserialize-impl
    template<class T, i32 I >
    static auto _deserialize_impl(Ti32<I>, T* pobj, ...) -> $when<(I >= T::_$property_cnt)> {
        (void)pobj;
        return;
    }
};

}
