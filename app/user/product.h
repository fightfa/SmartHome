/*
 * product.h
 *
 *  Created on: 2017Äê12ÔÂ27ÈÕ
 *      Author: Fightfa
 */

#ifndef APP_USER_PRODUCT_H_
#define APP_USER_PRODUCT_H_
#include "c_types.h"

#if defined(GLOBAL_DEBUG)
#define PRODUCT_DEBUG(format, ...)  log_printf("PRODUCT: ", format,  ##__VA_ARGS__)
#else
#define PRODUCT_DEBUG(format, ...)
#endif
//(512-16)KB=0x(80-4)000=0x7C 000
#define PRODUCT_SEC 0x7a

typedef struct ablity_cfg_t
{
        uint8_t status;
        uint8_t power_switch;
        uint8_t alpha;
} __attribute__((aligned(1), packed)) ablity_cfg_t;

typedef enum
{
	product_state_run,
	product_state_supend,
    product_state_off
} product_state_t;
typedef enum
{
	power_state_off=0,
	power_state_on
} power_state_t;
int product_routeSubscribe(const char* topic, uint32_t topic_len, const char *data, uint32_t data_len);
void product_apply_settings();
void product_on_power_switch_key_event();
#endif /* APP_USER_PRODUCT_H_ */
