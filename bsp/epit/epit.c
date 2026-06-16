#include "epit.h"
#include "led.h"	
#include "int.h"
#include "gpio.h"
#include "beep.h"

void epit1_init(void)
{
    volatile uint32_t temp;
    temp = EPIT1->CR;  /*复位值是0x00000000*/
    temp |= 7<<1;      /*设置bit1-bit3为1*/
    temp &= ~(4095<<4);
    temp |= 659<<4;    /*66M/660=100KHZ,装载是100，定时1ms*/
    temp &= ~(3<<24);
    temp |= 1<<24;    /*bit25-bit24,epit时钟源选择ipgclock=66M*/
    EPIT1->CR = temp;
    EPIT1->LR = 2000; /*定时20ms*/
    EPIT1->CMPR = 0; /* 比较寄存器值 */
    GIC_EnableIRQ(EPIT1_IRQn);				/* 使能GIC中对应的中断 */
	system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_rqhandler, NULL);
    

}

void epit1_enable(void)
{
    EPIT1->CR &= ~1;
    EPIT1->LR = 2000; /*定时20ms*/
    EPIT1->CR |= 1;
}


void epit1_disable(void)
{
    EPIT1->CR &= ~1;
}


void epit1_rqhandler(void)
{
    static uint8_t state = 0;
    if(EPIT1->SR & (1<<0)) /* 判断比较事件发生 */
    {
        epit1_disable();  /*关闭epit1定时器*/
        if(gpio_read_pin(GPIO1, 18) == 0)	/* 按键按下了  */
        {
            state = !state;
            beep_switch(state);
        } 
    }
    EPIT1->SR |= 1<<0; /*写1清0*/
    

}


