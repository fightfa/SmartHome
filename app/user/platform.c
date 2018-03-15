
#include "platform.h"

#include "../smartlink/smartlink.h"
#include "network.h"
#include "user_key.h"
#include "product.h"
void ICACHE_FLASH_ATTR
platform_network_state_changed(network_state_t current_state)
{
	switch (current_state)
	{
	case network_state_idle:
		PLATFORM_DEBUG("stop all services\r\n");
		//TODO
//		airkiss_lan_udp_delete();
//		smartlink_stop();
		break;
	case network_state_smart:
		PLATFORM_DEBUG("start smartlink\r\n");
		//TODO
//		airkiss_cloud_delete();
//		airkiss_lan_udp_delete();
//		smartlink_stop();
		smartlink_start();
//		network_state_lock();
		break;
	case network_state_ap:
		PLATFORM_DEBUG("restart LAN services, stop cloud services\r\n");
		//TODO
//		airkiss_cloud_delete();
//		airkiss_lan_udp_delete();
//		smartlink_stop();
//		airkiss_lan_udp_create();
//		airkiss_lan_udp_server_broadcast_alive();
		break;
	case network_state_ap_sta_ok:
		PLATFORM_DEBUG("restart all services\r\n");
		//TODO
//		airkiss_cloud_delete();
//		airkiss_lan_udp_delete();
//		smartlink_stop();
//		airkiss_lan_udp_create();
//		airkiss_lan_udp_broadcast_link_success_repeat(10);
		break;
	case network_state_ap_sta_err:
		PLATFORM_DEBUG("restart LAN services, stop cloud services\r\n");
		//TODO
//		airkiss_cloud_delete();
//		airkiss_lan_udp_delete();
//		smartlink_stop();
//		airkiss_lan_udp_create();
//		airkiss_lan_udp_server_broadcast_alive();
		break;
	case network_state_sta_ok:
		PLATFORM_DEBUG("network_state_sta_ok,restart all services\r\n");

//		airkiss_cloud_delete();
//		airkiss_lan_udp_delete();
//		smartlink_stop();
		iot_mqtt_init();
		iot_mqtt_connect();
		iot_mqtt_route_subscribe_register(product_routeSubscribe);
		airkiss_lan_udp_create();
		airkiss_lan_udp_broadcast_link_success_repeat(10);

		break;
	case network_state_sta_err:
		PLATFORM_DEBUG("network_state_sta_err, stop all services\r\n");
		//TODO temp:
//		network_state_change(network_state_smart);
		//TODO
//		airkiss_cloud_delete();
//		airkiss_lan_udp_delete();
//		smartlink_stop();
		break;
	default:
		break;
	}
}

LOCAL void ICACHE_FLASH_ATTR
platform_network_smartlink_success(const void * pdata)
{
	PLATFORM_DEBUG("smartlink success,airkiss_lan_udp_broadcast_link_success_repeat\r\n");
//	airkiss_lan_udp_create();
//	airkiss_lan_udp_broadcast_link_success_repeat(10);
	network_state_unlock();
}

LOCAL void ICACHE_FLASH_ATTR
platform_network_smartlink_timeout(const void * pdata)
{
	PLATFORM_DEBUG("smartlink timeout\r\n");
	network_state_unlock();
	PLATFORM_DEBUG("change to Station-AP Mode\r\n");
//	wifi_set_opmode(STATIONAP_MODE);
}

void ICACHE_FLASH_ATTR
on_click_key_smartlink(void)
{
	network_state_change(network_state_smart);
}
void ICACHE_FLASH_ATTR
on_click_key_power_switch(void)
{
	product_on_power_switch_key_event();
}

void ICACHE_FLASH_ATTR
platform_init(void)
{
//	CFG_Load();
//	CFG_Save();
	product_cfg_load();
	user_airkiss_key_regist(NULL,on_click_key_smartlink);
	user_power_switch_key_regist(on_click_key_power_switch);
	user_keys_init();
	network_init(platform_network_state_changed);
//	network_system_timer_callback_register(airkiss_app_start_check);
	smartlink_success_callback_register(platform_network_smartlink_success);
	smartlink_timeout_callback_register(platform_network_smartlink_timeout);

}
