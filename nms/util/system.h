#pragma once

#include <nms/core.h>

namespace nms::system
{

/*! the the system environment  */
NMS_API str  getenv(const str& name);

/*! pause the program until press any key  */
NMS_API void    pause();

/*! PC beep */
NMS_API void    beep(u32 freq, f64 duration);

/*! sleep the thread for some time */
NMS_API void    sleep(double duration);

}
