/* config.h
*
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "os_type.h"
#include "user_config.h"
#if defined(GLOBAL_DEBUG_ON)
#define CONFIG_DEBUG( format, ... ) log_printf("CONFIG:", format, ## __VA_ARGS__ )
#else
#define CONFIG_DEBUG( format, ... )
#endif
typedef struct{
	uint32_t cfg_holder;
	uint8_t device_id[64];
	uint8_t binded_user_id[64];

	uint8_t mqtt_host[64];
	uint32_t mqtt_port;
	uint8_t mqtt_user[32];
	uint8_t mqtt_pass[32];
	uint32_t mqtt_keepalive;
	uint8_t security;
} SYSCFG;

typedef struct {
    uint8 flag;
    uint8 pad[3];
} SAVE_FLAG;

void ICACHE_FLASH_ATTR CFG_Save();
void ICACHE_FLASH_ATTR CFG_Load();

extern SYSCFG sysCfg;

#endif /* _CONFIG_H_ */
