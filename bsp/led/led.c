#include "led.h"				/* cc.h为自行添加的文件 */
#include "fsl_iomuxc.h"

void led_init(void)
{
    CCM->CCGR1|=0X3<<26;                      /*使能GPIO1时钟*/
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03,0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03,0XF0B0);
    GPIO1->GDIR |= 0x8;  /*IO配置为输出模式*/
    GPIO1->DR |= 0x8;    /*GPIO1_IO3输出高电平*/

}

void LEDON(void)
{
    GPIO1->DR &= ~0x8;   /*GPIO1_IO3输出低电平*/
}



void LEDOFF(void)
{
    GPIO1->DR |= 0x8;    /*GPIO1_IO3输出高电平*/
}

void LED_TOGGLE(uint8_t state)
{
    if(state==1)
    LEDON();
    else
    LEDOFF();
}

