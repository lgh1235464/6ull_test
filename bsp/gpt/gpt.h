#ifndef __GPT_H
#define __GPT_H
#include "imx6ul.h"
void gpt1_init(void);
void gpt1_rqhandler(void);
void gpt1_enable(void);
void gpt1_disable(void);
void hperdelay_ms(uint32_t num);
void hperdelay_us(uint32_t num);





#endif 








