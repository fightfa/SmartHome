#include "user_key.h"
//#include "../driver/key.h"
#include "key.h"
#include "user_interface.h"

#define KEY_NUM        2
//D1
#define AIRKISS_KEY_IO_MUX     PERIPHS_IO_MUX_GPIO5_U
#define AIRKISS_KEY_IO_NUM     5
#define AIRKISS_KEY_IO_FUNC    FUNC_GPIO5
//D2
#define POWER_SWITCH_KEY_IO_MUX     PERIPHS_IO_MUX_GPIO4_U
#define POWER_SWITCH_KEY_IO_NUM     4
#define POWER_SWITCH_KEY_IO_FUNC    FUNC_GPIO4


LOCAL struct keys_param keys;
LOCAL struct single_key_param* single_key[KEY_NUM];
void ICACHE_FLASH_ATTR
user_airkiss_key_regist(key_function long_press, key_function short_press){
	single_key[0] = key_init_single(AIRKISS_KEY_IO_NUM, AIRKISS_KEY_IO_MUX, AIRKISS_KEY_IO_FUNC,
			long_press, short_press);

}
void ICACHE_FLASH_ATTR
user_power_switch_key_regist(key_function short_press){
	single_key[1]= key_init_single(POWER_SWITCH_KEY_IO_NUM, POWER_SWITCH_KEY_IO_MUX, POWER_SWITCH_KEY_IO_FUNC,
			NULL, short_press);
}
void ICACHE_FLASH_ATTR
user_keys_init(){
	keys.key_num = KEY_NUM;
	keys.single_key= single_key;
	key_init(&keys);
}
