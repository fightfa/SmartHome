/*
 * product_json.h
 *
 *  Created on: 2017年12月28日
 *      Author: Administrator
 */

#ifndef APP_USER_PRODUCT_JSON_H_
#define APP_USER_PRODUCT_JSON_H_
#include "osapi.h"
#if defined(GLOBAL_DEBUG)
#define PRODUCT_JSON_DEBUG(format, ...)  log_printf("PRODUCT_JSON: ", format,  ##__VA_ARGS__)
#else
#define PRODUCT_JSON_DEBUG(format, ...)
#endif
//TODO 换为ability_cfg_t
int product_json_notify(char * _buf, unsigned char _operation_status, uint8_t _power_switch, uint8_t _alpha);
int product_json_ask_get(char * _buf, uint64_t _msg_id, char *_user, uint8_t _operation_status, uint8_t _power_switch, uint8_t _alpha);
int product_json_ask_set(char * _buf, uint64_t _msg_id, uint8_t _operation_status, uint8_t _power_switch, uint8_t _alpha);



#endif /* APP_USER_PRODUCT_JSON_H_ */
