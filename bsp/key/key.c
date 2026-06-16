#include "key.h"	
#include "fsl_iomuxc.h"
#include "delay.h"				/* cc.h为自行添加的文件 */
#include "gpio.h"
//GPIO5_IO01
uint8_t read_key(void)
{
    uint8_t ret;
    ret = (GPIO1->DR>>18) & 0x1;
    return ret;
}

//KEY0 IOMUXC_SW_MUX_CTL_PAD_UART1_CTS_B GPIO1_IO18
void key_init(void)
{
    gpio_pin_config_t config;
    config.direction=GPIO_INPUT; /*使用->时，左边必须是结构体指针；使用.时左边是结构体变量*/
    CCM->CCGR1|=0X3<<26;                      /*使能GPIO1时钟*/
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);  /*复用为GPIO1_IO18*/
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0XF080);  /*电气属性配置22k上拉输入，不使能输出功能*/

    gpio_init(GPIO1,18,&config);
}


uint8_t get_keyvalue(void)
{
    static uint8_t key_release=1; 
    static uint8_t ret=1;  //静态变量只初始化一次
    if(key_release && read_key()==0)
    {
        key_release=0;
        delay_ms(10);
        if(read_key()==0)
        ret=KEY0_VALUE;

    }

    else if(read_key()==1)
    { 
        ret=KEY_NO;
        key_release=1;
    }

    return ret;


}

