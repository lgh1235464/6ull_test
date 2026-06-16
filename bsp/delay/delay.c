#include "delay.h"				/* cc.h为自行添加的文件 */




void delay1ms(volatile unsigned int num)
{
    //volatile int num=0x7ff;           /*延时函数,在 396Mhz 的主频下延时时间大约为 1ms 实验中发现如果不加volatile会一直卡在循环*/ 
    while (num--);                    /*根本原因在于编译器在开启优化（-O1, -O2, -O3）后，认为你的延迟循环是“无用代码”，将其优化掉了。去掉-O2功能正常*/



}

void delay_ms(unsigned int num)  /*入参不加volatile功能也正常*/
{
     while(num--)
     {
        delay1ms(0x7ff);
     }
   
}




