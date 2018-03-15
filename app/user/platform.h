#ifndef __PLATFORM_H__
#define __PLATFORM_H__
#include "osapi.h"
#if defined(GLOBAL_DEBUG)
#define PLATFORM_DEBUG(format, ...) log_printf("PLATFORM: ", format, ##__VA_ARGS__)
#else
#define PLATFORM_DEBUG(format, ...)
#endif

#include "c_types.h"
void platform_init(void);

#endif /* __PLATFORM_H__ */
