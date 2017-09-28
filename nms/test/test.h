#pragma once

#include <nms/core.h>
#include <nms/io/log.h>
#include <nms/io/console.h>

namespace nms::test
{

NMS_API u32  install(str name, void (*func)());
NMS_API u32  invoke(const View<str>& masks);

}

#define nms_test(func, ...)                                                                                     \
    static void func##_test();                                                                                  \
    static auto func##_temp   = nms::test::install(nms::TypeId::make<struct func##_tag>().name, func##_test);   \
    static void func##_test()
