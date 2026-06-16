#include "gpt.h"
#include "led.h"	
#include "int.h"
#include "gpio.h"
#include "beep.h"

volatile uint8_t gpt1_flag=1;

void hperdelay_ms(uint32_t num)
{
    gpt1_flag=1;
    GPT1->OCR[0] = num*1000;    /* 比较寄存器值 */
    gpt1_enable();
    while(gpt1_flag);
}

void hperdelay_us(uint32_t num)
{
    gpt1_flag=1;
    GPT1->OCR[0] = num;    /* 比较寄存器值 */
    gpt1_enable();
    while(gpt1_flag);   
}


void gpt1_init(void)
{
    GPT1->CR = 0;
    GPT1->CR |= 1<<15;
    while((GPT1->CR>>15) & 0x01);



    GPT1->CR |= ((1<<1) | (1<<6));
    GPT1->PR |= 65;    /*66M/66=1000KHZ  */
    GPT1->IR |= 1<<0;   /*OF1IE中断使能 */

    GIC_EnableIRQ(GPT1_IRQn);				/* 使能GIC中对应的中断 */
	system_register_irqhandler(GPT1_IRQn, (system_irq_handler_t)gpt1_rqhandler, NULL);
    

}

void gpt1_enable(void)
{
    GPT1->CR |= 1;
}


void gpt1_disable(void)
{
    GPT1->CR &= ~1;
}


void gpt1_rqhandler(void)
{


    if(GPT1->SR & (1<<0)) /* 判断比较事件发生OF1 Flag */
    {
        gpt1_disable();
        gpt1_flag=0;
    }
    GPT1->SR |= 1<<0; /*写1清0*/
    

}


