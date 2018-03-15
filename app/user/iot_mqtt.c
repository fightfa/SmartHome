/**
 * @author Fightfa
 */
#include "config.h"
#include "user_config.h"
#include "mqtt.h"
#include "osapi.h"
#include "mem.h"
#include "iot_mqtt.h"
#include "my_cJSON.h"


#define TOPIC_SERVER_PREFIX  "server"
#define TOPIC_USER_PREFIX  "user"
#define DEVICE  "device"
#define ACK "ack"
#define BIND "bind"
#define UNBIND "unbind"
#define GET "get"
#define SET "set"
#define NOTIFY "notify"
//#define TOPIC_DEVICE_ACK_PREFIX TOPIC_DEVICE_PREFIX##/##ACK##/
//
//#define TOPIC_DEVICE_ACK_BIND TOPIC_DEVICE_ACK_PREFIX##BIND
//#define TOPIC_DEVICE_ACK_UNBIND TOPIC_DEVICE_ACK_PREFIX##UNBIND
//#define TOPIC_DEVICE_ACK_GET TOPIC_DEVICE_ACK_PREFIX##GET
//#define TOPIC_DEVICE_ACK_SET TOPIC_DEVICE_ACK_PREFIX##SET
//#define TOPIC_DEVICE_NOTIFY TOPIC_DEVICE_PREFIX##NOTIFY

SYSCFG sysCfg;//extern
MQTT_Client mqttClient;
LOCAL iot_mqtt_route_subscribe_callback_t iot_mqtt_route_subscribe_callback = NULL;
void ICACHE_FLASH_ATTR
iot_mqtt_route_subscribe_register(iot_mqtt_route_subscribe_callback_t _iot_mqtt_route_subscribe_callback)
{
	iot_mqtt_route_subscribe_callback = _iot_mqtt_route_subscribe_callback;
}
uint8_t iot_mqtt_publish_json_buffer[256];
//private:
int ICACHE_FLASH_ATTR handleSubcribeBind(const char *data, uint32_t data_len){
//TODO user_id解析自topic
	my_cJSON *root = NULL;
	my_cJSON *js_msg_id = NULL;
//	my_cJSON *js_msg_type = NULL;
//	my_cJSON *js_create_time= NULL;
	my_cJSON *js_user = NULL;
	char *s = NULL;
	int ret = -1;
	if(!data_len)return -1;
	//必备属性
	root = my_cJSON_Parse(data);
	if(!root) { INFO("get root faild !\n"); return -1; }

	js_msg_id = my_cJSON_GetObjectItem(root, "msg_id");
	if(!js_msg_id) { INFO("get msg_id faild !\n"); return -1; }

//		js_msg_type = my_cJSON_GetObjectItem(root, "msg_type");
//		if(!js_msg_type) { PRODUCT_DEBUG("get msg_type faild !\n"); return -1; }

	js_user = my_cJSON_GetObjectItem(root, "open_id");
	if(!js_user) { INFO("get user faild !\n"); return -1; }

	//TODO 保存更新本地user_id，并重新订阅
	os_strncpy(sysCfg.binded_user_id,js_user->string,sizeof(sysCfg.binded_user_id)-1);
	CFG_Save();
	INFO("saved binded_user_id=%s",sysCfg.binded_user_id);
	//TODO --重新订阅user/{user_id}/get/
	//TODO 返回json
	//Temp:
//	asy_error_code
//	asy_error_msg
	os_memcpy(iot_mqtt_publish_json_buffer, data,data_len);
	iot_mqtt_publish_ack_bind(iot_mqtt_publish_json_buffer,os_strlen(iot_mqtt_publish_json_buffer));
	my_cJSON_Delete(root);
}
//private:
void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Connected\r\n");
	char* topicBuf= (char*)os_zalloc(64);
	//TODO
	os_sprintf(topicBuf,"%s%s","server/bind/",DEVICE_ID);
	MQTT_Subscribe(client, topicBuf, 2);
	os_sprintf(topicBuf,"%s%s","server/unbind/",DEVICE_ID);
	MQTT_Subscribe(client, topicBuf, 2);
	os_sprintf(topicBuf,"%s%s","server/unbind/",DEVICE_ID);
	MQTT_Subscribe(client, topicBuf, 2);
	os_sprintf(topicBuf,"%s%s","server/get/",DEVICE_ID);
	MQTT_Subscribe(client, topicBuf, 2);
	os_sprintf(topicBuf,"%s%s","server/set/",DEVICE_ID);
	MQTT_Subscribe(client, topicBuf, 2);
	os_free(topicBuf);

}
//private:
void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
}
//private:
void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
}
//private:
void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);
	char *p=topicBuf;
	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
	if(0==os_strncmp(p,TOPIC_SERVER_PREFIX,sizeof(TOPIC_SERVER_PREFIX)-1)){
		p=os_strchr(p,'/')+1;
		if(0==os_strncmp(p,SET,sizeof(SET)-1)||0==os_strncmp(p,GET,sizeof(GET)-1)){
			//p=os_strchr(p,'/')+1;
			//TODO* handle
			if(iot_mqtt_route_subscribe_callback)
				iot_mqtt_route_subscribe_callback(topic, topic_len,data, data_len);
		}else if(0==os_strncmp(p,BIND,sizeof(BIND)-1)){
			handleSubcribeBind(dataBuf,data_len);
		}else if(0==os_strncmp(p,UNBIND,sizeof(UNBIND)-1)){
			//TODO* handle
//			handleSubcribeUnbind(dataBuf,data_len);
		}
	}
//	"user/+/bind/%c","user/+/unbind/%c","user/+/get/%c","user/+/set/%c"
/*	else if(0==os_strncmp(p,TOPIC_USER_PREFIX,sizeof(TOPIC_USER_PREFIX))){
		p=os_strchr(p,'/')+1;
		//TODO 提取user_id并验证当前绑定的user_id，若已在订阅时过滤则忽略验证
		p=os_strchr(p,'/')+1;
		if(0==os_strncmp(p,SET,sizeof(SET))){
			//TODO 提取client_id并与DEVICE_ID验证，若已在订阅时过滤则忽略验证。当前提取的值暂不用到
			//p=os_strchr(p,'/')+1;
			//TODO* handleSET
		}else if(0==os_strncmp(p,GET,sizeof(GET))){
			//TODO* handle
		}else if(0==os_strncmp(p,BIND,sizeof(BIND))){
			//TODO* handle
		}else if(0==os_strncmp(p,UNBIND,sizeof(UNBIND))){
			//TODO* handle
		}
	}*/
	os_free(topicBuf);
	os_free(dataBuf);
}

void ICACHE_FLASH_ATTR
iot_mqtt_init(void){
//	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
//TODO 写sysCfg
//	MQTT_InitConnection(&mqttClient, "202.38.206.3", 11883, 0);
	MQTT_InitConnection(&mqttClient, MQTT_HOST, MQTT_PORT, 0);
//	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	MQTT_InitClient(&mqttClient, DEVICE_ID, MQTT_USER, MQTT_PASS, MQTT_KEEPALIVE, 1);
	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);
}
void ICACHE_FLASH_ATTR
iot_mqtt_connect(void){
	MQTT_Connect(&mqttClient);
}
void ICACHE_FLASH_ATTR
iot_mqtt_publish_notify(uint8_t *body, uint32_t bodylen){
	uint32_t topic_len=sizeof(DEVICE)+sizeof(DEVICE_ID)+sizeof(NOTIFY);//已算上/占的长度
	char *topicBuf = (char*)os_zalloc(topic_len);
	os_sprintf(topicBuf,"%s/%s/%s",DEVICE,DEVICE_ID,NOTIFY);
	MQTT_Publish(&mqttClient,topicBuf,body,bodylen,2,true);
	os_free(topicBuf);
}
//private:
void ICACHE_FLASH_ATTR
iot_mqtt_publish_ack_bind(uint8_t *body, uint32_t bodylen){
	uint32_t topic_len = sizeof(DEVICE) + sizeof(DEVICE_ID) + sizeof(ACK)+ sizeof(BIND);			//已算上/占的长度
	char *topicBuf = (char*) os_zalloc(topic_len);
	os_sprintf(topicBuf, "%s/%s/%s/%s", DEVICE, DEVICE_ID, ACK, BIND);
	MQTT_Publish(&mqttClient, topicBuf, body, bodylen,2, true);
	os_free(topicBuf);
}
//private:
void ICACHE_FLASH_ATTR
iot_mqtt_publish_ack_unbind(uint8_t *body, uint32_t bodylen){
	uint32_t topic_len = sizeof(DEVICE) + sizeof(DEVICE_ID) + sizeof(ACK)+ sizeof(UNBIND);			//已算上/占的长度
	char *topicBuf = (char*) os_zalloc(topic_len);
	os_sprintf(topicBuf, "%s/%s/%s/%s", DEVICE, DEVICE_ID, ACK, UNBIND);
	MQTT_Publish(&mqttClient, topicBuf, body, bodylen, 2, true);
	os_free(topicBuf);
}
void ICACHE_FLASH_ATTR
iot_mqtt_publish_ack_get(uint8_t *body, uint32_t bodylen){
	uint32_t topic_len = sizeof(DEVICE) + sizeof(DEVICE_ID) + sizeof(ACK)+ sizeof(GET);			//已算上/占的长度
	char *topicBuf = (char*) os_zalloc(topic_len);
	os_sprintf(topicBuf, "%s/%s/%s/%s", DEVICE, DEVICE_ID, ACK, GET);
	MQTT_Publish(&mqttClient, topicBuf, body, bodylen, 2, true);
	os_free(topicBuf);
}
void ICACHE_FLASH_ATTR
iot_mqtt_publish_ack_set(uint8_t *body, uint32_t bodylen){
	uint32_t topic_len = sizeof(DEVICE) + sizeof(DEVICE_ID) + sizeof(ACK)+ sizeof(SET);			//已算上/占的长度
	char *topicBuf = (char*) os_zalloc(topic_len);
	os_sprintf(topicBuf, "%s/%s/%s/%s", DEVICE, DEVICE_ID, ACK, SET);
	MQTT_Publish(&mqttClient, topicBuf, body, bodylen, 2, true);
	os_free(topicBuf);
}
