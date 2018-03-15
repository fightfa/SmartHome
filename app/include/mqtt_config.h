#ifndef __MQTT_CONFIG_H__
#define __MQTT_CONFIG_H__

#define CFG_HOLDER	0x00FF55A4	/* Change this value to load default configurations */
#define CFG_LOCATION	0x79	/* Please don't change or if you know what you doing */
//#define CFG_LOCATION	0x3f9	/* Please don't change or if you know what you doing */
//#define MQTT_SSL_ENABLE

/*DEFAULT CONFIGURATIONS*/

//#define MQTT_HOST			"192.168.1.100" //or "mqtt.yourdomain.com"
//#define MQTT_HOST			"mqtt.fightfa.tunnel.echomod.cn" //or "mqtt.yourdomain.com"
//#define MQTT_PORT			11883
#define MQTT_HOST			"tunnel.echomod.cn" //or "mqtt.yourdomain.com"
//#define MQTT_PORT			38532
//#define MQTT_PORT			39175
//#define MQTT_PORT			45147
#define MQTT_PORT			45664
#define MQTT_BUF_SIZE		1024
#define MQTT_KEEPALIVE		120	 /*second*/
#define MQTT_RECONNECT_TIMEOUT 	5	/*second*/

#define MQTT_CLIENT_ID		"DVES_%08X"
#define MQTT_USER			"device"
#define MQTT_PASS			"password"

#define DEFAULT_SECURITY	0
#define QUEUE_BUFFER_SIZE		 		2048
#define PROTOCOL_NAMEv31	/*MQTT version 3.1 compatible with Mosquitto v0.15*/
//PROTOCOL_NAMEv311			/*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/*/




#endif // __MQTT_CONFIG_H__
