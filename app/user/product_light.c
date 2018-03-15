/*
 * product.c
 *
 *  Created on: 2017年12月27日
 *      Author: Fightfa
 */
#include "ets_sys.h"
#include "os_type.h"
#include "mem.h"
#include "osapi.h"
#include "user_interface.h"

#include "product.h"
#include "my_cJSON.h"
#include "product_json.h"
LOCAL ablity_cfg_t local_ablity_cfg;
//TODO 行不通
//LOCAL ablity_cfg_t local_ablity_cfg = {
//    .status = product_state_run,
//    .power_switch = power_state_on,
//    .alpha = 100
//};
uint8_t product_json_send_buffer[512];

void ICACHE_FLASH_ATTR
product_cfg_save(void)
{
	spi_flash_erase_sector(PRODUCT_SEC);
			spi_flash_write((PRODUCT_SEC) * SPI_FLASH_SEC_SIZE,
							(uint32 *)&local_ablity_cfg, sizeof(local_ablity_cfg));
//	system_param_save_with_protect((PRODUCT_SEC),
//			 (void *)(&local_ablity_cfg), sizeof(local_ablity_cfg));
    PRODUCT_DEBUG("local_ablity_cfg param saved !\r\n");
}
void ICACHE_FLASH_ATTR
product_cfg_load(void)
{
	spi_flash_read((PRODUCT_SEC) * SPI_FLASH_SEC_SIZE,
				   (uint32 *)&local_ablity_cfg, sizeof(local_ablity_cfg));
//	system_param_load((PRODUCT_SEC),0,
//			 (void *)(&local_ablity_cfg), sizeof(local_ablity_cfg));
    PRODUCT_DEBUG("local_ablity_cfg param loaded !\r\n");
}
void ICACHE_FLASH_ATTR
product_apply_settings(){
	PRODUCT_DEBUG("product_apply_settings()\r\n");
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);//D5
	if(	local_ablity_cfg.status==product_state_run){
//		light_power_switch_on();
		GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0);//D4
		}else{
//			light_power_switch_off();
			GPIO_OUTPUT_SET(GPIO_ID_PIN(14),1);
		}
}
void ICACHE_FLASH_ATTR
product_on_power_switch_key_event(){
	int ret=-1;
	PRODUCT_DEBUG("product_on_power_switch_key_event()\r\n");
	if(	local_ablity_cfg.power_switch==power_state_off){
		local_ablity_cfg.power_switch=power_state_on;
	}else{
		local_ablity_cfg.power_switch=power_state_off;
	}
	if(	local_ablity_cfg.status==product_state_run){
		local_ablity_cfg.status=product_state_supend;
	}else{
		local_ablity_cfg.status=product_state_run;
	}
	product_cfg_save();
	product_apply_settings();
	os_memset(product_json_send_buffer, 0, sizeof(product_json_send_buffer));
	ret =product_json_notify(product_json_send_buffer,local_ablity_cfg.status, local_ablity_cfg.power_switch, local_ablity_cfg.alpha);
	if(ret == 0) iot_mqtt_publish_notify(product_json_send_buffer,os_strlen(product_json_send_buffer));

}

int ICACHE_FLASH_ATTR
product_routeSubscribe(const char* topic, uint32_t topic_len, const char *data, uint32_t data_len){
	PRODUCT_DEBUG("product_routeSubscribe()");
	my_cJSON *root = NULL;
	my_cJSON *js_msg_id = NULL;
	my_cJSON *js_msg_type = NULL;
	my_cJSON *js_user = NULL;
	my_cJSON *js_services = NULL;
	my_cJSON *js_operation_status = NULL;
	my_cJSON *js_status = NULL;
	my_cJSON *js_power_switch = NULL;
	my_cJSON *js_on_off = NULL;
	my_cJSON *js_lightbulb = NULL;
	my_cJSON *js_alpha = NULL;
	char *s = NULL;
	int ret = -1;

	if(data_len)
	{
		//必备属性
		root = my_cJSON_Parse(data);
		if(!root) { PRODUCT_DEBUG("get root faild !\n"); return -1; }

		js_msg_id = my_cJSON_GetObjectItem(root, "msg_id");
		if(!js_msg_id) { PRODUCT_DEBUG("get msg_id faild !\n"); return -1; }

		js_msg_type = my_cJSON_GetObjectItem(root, "msg_type");
		if(!js_msg_type) { PRODUCT_DEBUG("get msg_type faild !\n"); return -1; }

		js_user = my_cJSON_GetObjectItem(root, "user");
		if(!js_user) { PRODUCT_DEBUG("get user faild !\n"); return -1; }

		js_services = my_cJSON_GetObjectItem(root, "services");
		if(!js_services) { PRODUCT_DEBUG("get services faild !\n"); return -1; }

		js_operation_status = my_cJSON_GetObjectItem(js_services, "operation_status");
		if(!js_operation_status) { PRODUCT_DEBUG("get operation_status faild !\n"); return -1; }

		js_status = my_cJSON_GetObjectItem(js_operation_status, "status");
		if(!js_status) { PRODUCT_DEBUG("get status faild !\n"); return -1; }

		local_ablity_cfg.status = (uint8_t) js_status->valueint;

//        os_memcpy(airkiss_app_msg.msg_type, js_msg_type->valuestring, os_strlen(js_msg_type->valuestring));
//        os_memcpy(airkiss_app_msg.user, js_user->valuestring, os_strlen(js_user->valuestring));
//        airkiss_app_msg.msg_id = js_msg_id->valueint;
//        airkiss_app_msg.status = js_status->valueint;

		//非必备属性
		//开关
		js_power_switch = my_cJSON_GetObjectItem(js_services, "power_switch");
		if(js_power_switch)
		{
			js_on_off = my_cJSON_GetObjectItem(js_power_switch, "on_off");
			if(!js_on_off)
			{
				PRODUCT_DEBUG("no on_off!\n");
				return -1;
			}
			local_ablity_cfg.power_switch = (js_on_off->type == my_cJSON_False)?power_state_off:power_state_on;
		}
		else
		{
			PRODUCT_DEBUG("get power_switch faild !\n");
		}

		//灯能力属性
		js_lightbulb = my_cJSON_GetObjectItem(js_services, "lightbulb");
		if(js_lightbulb)
		{
			js_alpha = my_cJSON_GetObjectItem(js_lightbulb, "alpha");
			if(!js_alpha)
			{
				PRODUCT_DEBUG("no alpha!\n");
				return -1;
			}
			if(js_alpha->valueint <= 100 && js_alpha->valueint >= 0 )
				local_ablity_cfg.alpha = js_alpha->valueint;
		}
		else
		{
			PRODUCT_DEBUG("get lightbulb faild !\n");
		}

		my_cJSON_Delete(root);

		PRODUCT_DEBUG("msg_type: %s\n", js_msg_type->valuestring);
		//回复信息
		if(os_strncmp(js_msg_type->valuestring, "set", sizeof("set")-1) == 0)
		{
			//TODO *处理信息
			product_apply_settings();
			product_cfg_save();
			os_memset(product_json_send_buffer, 0, sizeof(product_json_send_buffer));
			//TODO 临时代码，iot_mqtt_publish_ack_set方面微信硬件平台给的API URL没说清楚。
			ret =product_json_notify(product_json_send_buffer,local_ablity_cfg.status, local_ablity_cfg.power_switch, local_ablity_cfg.alpha);
			if(ret == 0) iot_mqtt_publish_notify(product_json_send_buffer,os_strlen(product_json_send_buffer));
//			ret =product_json_ask_set(product_json_send_buffer,js_msg_id->valueint,local_ablity_cfg.status, local_ablity_cfg.power_switch, local_ablity_cfg.alpha);
//			if(ret == 0) iot_mqtt_publish_ack_set(product_json_send_buffer,os_strlen(product_json_send_buffer));
		}
		else if(os_strncmp(js_msg_type->valuestring, "get", sizeof("get")-1) == 0)
		{
			os_memset(product_json_send_buffer, 0, sizeof(product_json_send_buffer));
			//TODO 临时代码，iot_mqtt_publish_ack_set方面微信硬件平台给的API URL没说清楚。
			ret =product_json_notify(product_json_send_buffer,local_ablity_cfg.status, local_ablity_cfg.power_switch, local_ablity_cfg.alpha);
			if(ret == 0) iot_mqtt_publish_notify(product_json_send_buffer,os_strlen(product_json_send_buffer));
//			ret =product_json_ask_get(product_json_send_buffer,js_msg_id->valueint,js_user->valuestring,local_ablity_cfg.status, local_ablity_cfg.power_switch, local_ablity_cfg.alpha);
//			if(ret == 0) iot_mqtt_publish_ack_get(product_json_send_buffer,os_strlen(product_json_send_buffer));
		}

	}
	return 0;
}
