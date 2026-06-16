
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "clk.h"
#include "gpio.h"
#include "int.h"
#include "exit.h"
#include "epit.h"
#include "gpt.h"
#include "stdio.h"

int main (void)
{
    static uint8_t state = 0;
    int a,b;
    uint8_t c=100;
    int_init();
    imx6u_clkinit();
    uart_init();
    led_init();
    beep_init();
    key_init();
    exit_init();  /*初始化中断*/
    epit1_init(); /*epit1初始化*/
    gpt1_init();  /*gpt1初始化*/

    printf("%d的16进制是:%x\r\n",c,c);
    while (1)
    {

        printf("请输入2个整数中间用空格隔开");
        scanf("%d %d",&a,&b);
        printf("\r\n%d乘%d的结果是:%d\r\n",a,b,a*b);




        state = !state;
        LED_TOGGLE(state);
 
    
    }
    
    return 0;


}

