/*
* config.c
*
**/
#include "ets_sys.h"
#include "os_type.h"
#include "mem.h"
#include "osapi.h"
#include "user_interface.h"

#include "config.h"
#include "user_config.h"
#include "mqtt_config.h"

//TODO 完善sector config分配机制。完善saveFlag通用代码块。
//TODO - 完善在线更新固件与设备配置内存地址分隔，以及产品生产线烧录设备配置数据的方法。

SYSCFG sysCfg;//for export
SAVE_FLAG saveFlag;

void ICACHE_FLASH_ATTR
CFG_Save()
{
	 spi_flash_read((CFG_LOCATION + 3) * SPI_FLASH_SEC_SIZE,
	                   (uint32 *)&saveFlag, sizeof(SAVE_FLAG));

	if (saveFlag.flag == 0) {
		spi_flash_erase_sector(CFG_LOCATION + 1);
		spi_flash_write((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE,
						(uint32 *)&sysCfg, sizeof(SYSCFG));
		saveFlag.flag = 1;
		spi_flash_erase_sector(CFG_LOCATION + 3);
		spi_flash_write((CFG_LOCATION + 3) * SPI_FLASH_SEC_SIZE,
						(uint32 *)&saveFlag, sizeof(SAVE_FLAG));
	} else {
		spi_flash_erase_sector(CFG_LOCATION + 0);
		spi_flash_write((CFG_LOCATION + 0) * SPI_FLASH_SEC_SIZE,
						(uint32 *)&sysCfg, sizeof(SYSCFG));
		saveFlag.flag = 0;
		spi_flash_erase_sector(CFG_LOCATION + 3);
		spi_flash_write((CFG_LOCATION + 3) * SPI_FLASH_SEC_SIZE,
						(uint32 *)&saveFlag, sizeof(SAVE_FLAG));
	}
}

void ICACHE_FLASH_ATTR
CFG_Load()
{

	CONFIG_DEBUG("\r\nCFG_Load...\r\n");
	spi_flash_read((CFG_LOCATION + 3) * SPI_FLASH_SEC_SIZE,
				   (uint32 *)&saveFlag, sizeof(SAVE_FLAG));
	if (saveFlag.flag == 0) {
		spi_flash_read((CFG_LOCATION + 0) * SPI_FLASH_SEC_SIZE,
					   (uint32 *)&sysCfg, sizeof(SYSCFG));
	} else {
		spi_flash_read((CFG_LOCATION + 1) * SPI_FLASH_SEC_SIZE,
					   (uint32 *)&sysCfg, sizeof(SYSCFG));
	}
	if(sysCfg.cfg_holder != CFG_HOLDER){
		os_memset(&sysCfg, 0x00, sizeof sysCfg);


		sysCfg.cfg_holder = CFG_HOLDER;

//		os_sprintf(sysCfg.device_id, MQTT_CLIENT_ID, system_get_chip_id());
		os_strncpy(sysCfg.device_id, DEVICE_ID, sizeof(sysCfg.device_id) - 1);
//		sysCfg.device_id[sizeof(sysCfg.device_id) - 1] = '\0';
//		os_strncpy(sysCfg.binded_user_id, "", sizeof(sysCfg.binded_user_id) - 1);
		sysCfg.binded_user_id[0]='\0';

		os_strncpy(sysCfg.mqtt_host, MQTT_HOST, sizeof(sysCfg.mqtt_host) - 1);
		sysCfg.mqtt_port = MQTT_PORT;
		os_strncpy(sysCfg.mqtt_user, MQTT_USER, sizeof(sysCfg.mqtt_user) - 1);
		os_strncpy(sysCfg.mqtt_pass, MQTT_PASS, sizeof(sysCfg.mqtt_pass) - 1);

		sysCfg.security = DEFAULT_SECURITY;	/* default non ssl */

		sysCfg.mqtt_keepalive = MQTT_KEEPALIVE;

		CONFIG_DEBUG("CFG_Load default configuration\r\n");

		CFG_Save();
	}
	CONFIG_DEBUG("CFG_Loaded\r\n");

}
