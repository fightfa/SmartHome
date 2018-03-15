/*
 * iot_mqtt.h
 *
 *  Created on: 2017Äê12ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef APP_USER_IOT_MQTT_H_
#define APP_USER_IOT_MQTT_H_
#if defined(GLOBAL_DEBUG_ON)
#define INFO( format, ... ) log_printf("IOT_MQTT:", format, ## __VA_ARGS__ )
#else
#define INFO( format, ... )
#endif
typedef int (* iot_mqtt_route_subscribe_callback_t)(const char* topic, uint32_t topic_len, const char *data, uint32_t data_len);
void iot_mqtt_init(void);
void iot_mqtt_connect(void);
void iot_mqtt_publish_notify(uint8_t *body, uint32_t bodylen);
void iot_mqtt_publish_ack_get(uint8_t *body, uint32_t bodylen);
void iot_mqtt_publish_ack_set(uint8_t *body, uint32_t bodylen);
void iot_mqtt_route_subscribe_register(iot_mqtt_route_subscribe_callback_t _iot_mqtt_route_subscribe_callback);
//private:
void iot_mqtt_publish_ack_bind(uint8_t *body, uint32_t bodylen);

#endif /* APP_USER_IOT_MQTT_H_ */
