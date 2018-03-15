/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__
#define  DEVICE_TYPE   "gh_51c420a3a430"
#define  DEVICE_ID     "gh_51c420a3a430_d4f4bda4cbdcd9f2"


#define log_printf(header, fmt, ...) do {    \
    static const char flash_str[] ICACHE_RODATA_ATTR __attribute__((aligned(4))) = fmt; \
    if(os_strlen(fmt) > 2) \
    { \
        static const char header_str[] ICACHE_RODATA_ATTR __attribute__((aligned(4))) = header; \
        os_printf_plus(header_str, ##__VA_ARGS__);   \
    } \
    os_printf_plus(flash_str, ##__VA_ARGS__);   \
    } while(0)

#ifndef ICACHE_FLASH
#define ICACHE_FLASH
#endif

#endif

