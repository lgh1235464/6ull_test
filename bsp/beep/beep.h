#ifndef __BEEP_H
#define __BEEP_H

#include "cc.h"				/* cc.h为自行添加的文件 */
void beep_on(void);
void beep_off(void);
void beep_init(void);
void beep_switch(uint8_t state);
#endif