#include "clk.h"	
#include "fsl_iomuxc.h"
#include "delay.h"				/* cc.h为自行添加的文件 */


/*初始化PLL1\PLLL2\PLL3*/
void imx6u_clkinit(void)
{
    uint32_t reg;

    /* 1、设置 ARM 内核时钟为 528MHz */
    /* 1.1、判断当使用哪个时钟源启动的，正常情况下是由 pll1_sw_clk 驱动的，而
    * pll1_sw_clk 有两个来源： pll1_main_clk 和 step_clk， 如果要
    * 让 I.MX6ULL 跑到 528M， 那必须选择 pll1_main_clk 作为 pll1 的时钟
    * 源。 如果我们要修改 pll1_main_clk 时钟的话就必须先将 pll1_sw_clk 从
    * pll1_main_clk 切换到 step_clk,当修改完以后再将 pll1_sw_clk 切换
    * 回 pll1_main_cl， step_clk 等于 24MHz。
    */

    if((((CCM->CCSR) >> 2) & 0x1 ) == 0)/* pll1_main_clk */
    {
    CCM->CCSR &= ~(1 << 8); /* 配置 step_clk 时钟源为 24MHz OSC */
    CCM->CCSR |= (1 << 2); /* 配置 pll1_sw_clk 时钟源为 step_clk */
    }

    /* 1.2、设置 pll1_main_clk 为 1056MHz,也就是 528*2=1056MHZ,
    * 因为 pll1_sw_clk 进 ARM 内核的时候会被二分频！原子哥在线教学:www.yuanzige.com 论坛:www.openedv.com
    423
    I.MX6U 嵌入式 Linux 驱动开发指南
    * bit13: 1 使能时钟输出
    * bit[6:0]: 88, 由公式： Fout = Fin * div_select / 2.0，
    * 1056=24*div_select/2.0, 得出： div_select=88。
    */
    CCM_ANALOG->PLL_ARM = (1 << 13) | ((88 << 0) & 0X7F);
    CCM->CCSR &= ~(1 << 2);/* 将 pll_sw_clk 时钟切换回 pll1_main_clk */
    CCM->CACRR = 1; /* ARM 内核时钟为 pll1_sw_clk/2=1056/2=528Mhz */

    /*初始化PLL2，以及PFD3~PFD0*/
    reg = CCM_ANALOG->PFD_528;
    reg &= ~0x3F3F3F3F;
    reg |= 32<<24; /* PLL2_PFD3=528*18/32=297Mhz */
    reg |= 24<<16; /* PLL2_PFD2=528*18/24=396Mhz */
    reg |= 16<<8; /* PLL2_PFD1=528*18/16=594Mhz */
    reg |= 27<<0; /* PLL2_PFD0=528*18/27=352Mhz */
    CCM_ANALOG->PFD_528=reg; /* 设置 PLL2_PFD0~3 */

    /* 3、设置 PLL3(USB1)各个 PFD */
    reg = 0; /* 清零 */
    reg = CCM_ANALOG->PFD_480;
    reg &= ~(0X3F3F3F3F); /* 清除原来的设置 */
    reg |= 19<<24; /* PLL3_PFD3=480*18/19=454.74Mhz */
    reg |= 17<<16; /* PLL3_PFD2=480*18/17=508.24Mhz */
    reg |= 16<<8; /* PLL3_PFD1=480*18/16=540Mhz */
    reg |= 12<<0; /* PLL3_PFD0=480*18/12=720Mhz */
    CCM_ANALOG->PFD_480=reg; /* 设置 PLL3_PFD0~3*/

    /*4、设置AHB_CLK_ROOT=132Mhz*/
    /*CBCMR[PRE_PERIPH_CLK_SEL]设置为01，选择PLL2_PFD2作为时钟源*/
    reg = CCM->CBCMR;
    reg &= ~(0x3<<18);
    reg |= (0x1<<18);
    CCM->CBCMR = reg;
    /*CBCDR[PERIPH_CLK_SEL]设置为0*/
    CCM->CBCDR &= ~(1<<25);
    while((CCM->CDHIPR>>5) & 0x1);  /*等待握手完成*/
    /*CBCDR[AHB_PODF]设置为3分频*/
    /* 修改 AHB_PODF 位的时候需要先禁止 AHB_CLK_ROOT 的输出，但是
     * 我没有找到关闭 AHB_CLK_ROOT 输出的的寄存器，所以就没法设置。
     * 下面设置 AHB_PODF 的代码仅供学习参考不能直接拿来使用！！
     * 内部 boot rom 将 AHB_PODF 设置为了 3 分频，即使我们不设置 AHB_PODF，
     * AHB_ROOT_CLK 也依旧等于 396/3=132Mhz。
     */
    #if 0

     CCM->CBCDR &= ~(7<<10);
     CCM->CBCDR |= 2<<10;
     while(CCM->CDHIPR & (1 << 1));/* 等待握手完成 */
     #endif


    /*6、IPG_CLK_ROOT=66Mhz*/
    /*CBCDR[IPG_PODF]设置为01，2分频*/
    CCM->CBCDR &=~(3<<8);
    CCM->CBCDR |= (1<<8);

    /*5、PERCLK_CLK_ROOT=66Mhz*/
    /*CSCMR1[PERCLK_CLK_SEL]*设置为0，选择IPG_CLK*/
    CCM->CSCMR1 &= ~(0X1<<6);
    /*CSCMR1[PERCLK_PODF]设置为0，不分频*/
    CCM->CSCMR1 &= ~(0X3F);

    /*UART_CLK_ROOT=80Mhz*/   
    CCM->CSCDR1 &= ~(1<<6);   /*pll3_80M*/
    CCM->CSCDR1 &= ~0X3F;   /*1分频,UART_CLK_ROOT=80Mhz*/

}

