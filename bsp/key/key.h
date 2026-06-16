#ifndef __KEY_H
#define __KEY_H

#include "cc.h"				/* cc.h为自行添加的文件 */


/*按键值*/
enum keyvalue{
    KEY_NO= 0,
    KEY0_VALUE,


};
uint8_t read_key(void);
void key_init(void);
uint8_t get_keyvalue(void);

#endif