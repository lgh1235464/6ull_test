#include "beep.h"	
#include "fsl_iomuxc.h"
//GPIO5_IO01
void beep_init(void)
{
    CCM->CCGR1|=0X3<<31;                      /*使能GPIO5时钟//GPIO5_IO01*/
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0XF0B0); /*22K上拉*/
    GPIO5->GDIR |= 1<<1;  /*IO配置为输出模式*/
    GPIO5->DR |= 1<<1;   /*GPIO5_IO01输出HIGH电平*/
}

void beep_off(void)
{
    
    GPIO5->DR |= 1<<1;   /*GPIO5_IO01输出HIGH电平*/

}

void beep_on(void)
{
    GPIO5->DR &= ~(1<<1);   /*GPIO5_IO01输出低电平*/

}

void beep_switch(uint8_t state)
{

    if(state==1)
    beep_on();
    else
    beep_off();
}