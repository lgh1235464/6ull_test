#ifndef __INT_H
#define __INT_H
#include "imx6ul.h"
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_int.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : 中断驱动头文件。
其他	   : 无
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
***************************************************************/

/* 中断服务函数形式 */ 
typedef void (*system_irq_handler_t) (unsigned int giccIar, void *param);
/*一种指向“返回 void、接受 (unsigned int, void ) 两个参数的函数”的指针类型*/
 
/* 中断服务函数结构体*/
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler; /* 中断服务函数 */
    void *userParam;                 /* 中断服务函数参数 */
} sys_irq_handle_t;


/* 函数声明 */
void int_init(void);
void system_irqtable_init(void);
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);
void system_irqhandler(unsigned int giccIar); 
void default_irqhandler(unsigned int giccIar, void *userParam); 



#endif
