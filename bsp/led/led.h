#ifndef __LED_H
#define __LED_H

#include "cc.h"				/* cc.h为自行添加的文件 */
void LEDON(void);
void LEDOFF(void);
void led_init(void);
void LED_TOGGLE(uint8_t state);
#endif