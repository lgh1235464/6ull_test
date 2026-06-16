#include "uart.h"
#include "gpio.h"




void uart_init(void)
{
    uart_io_init();
    uart_disable(UART1);
    uart_softreset(UART1);

    UART1->UCR1 = 0;
    UART1->UCR2 |= 3<<1; /*TXEN RXEN*/
    UART1->UCR2 |= 1<<5; /*8位数据*/
    UART1->UCR2 |= 1<<14; /*忽略RTS*/

    UART1->UCR3 |= 2<<1; /*RX_MUX必须是1*/

   



/*
* 设置波特率
* 波特率计算公式:Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1))
* 如果要设置波特率为 115200，那么可以使用如下参数:
* Ref Freq = 80M 也就是寄存器 UFCR 的 bit9:7=101, 表示 1 分频
* UBMR = 3124
* UBIR =  71
* 因此波特率= 80000000/(16 * (3124+1)/(71+1))
*           = 80000000/(16 * 3125/72)
*           = (80000000*72) / (16*3125)*           = 115200
*/
#if 0   
    UART1->UFCR &= ~(1<<8);
    UART1->UFCR |= 5<<7; /*Reference Frequency Divider 1分频*/
    UART1->UBIR = 71;
    UART1->UBMR = 3124;
#endif



    uart_setbaudrate(UART1,115200, 80000000);    
    uart_enable(UART1);




}




void uart_io_init(void)
{
    /*UART1_TXD:GPIO1_IO16*/

    CCM->CCGR1|=0X3<<26;                      /*使能GPIO1时钟*/
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX,0);  /*复用为UART1_TX*/
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX,0X10B0);  /*电气气属性配置为保持*/

    


    /*UART1_RXD:GPIO1_IO17*/
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX,0);  /*复用为UART1_RX*/
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX,0X10B0);  /*电气属性配置为保持*/


}



void uart_disable(UART_Type *base)
{
    base->UCR1 &= ~(1<<0);

}

void uart_enable(UART_Type *base)
{
    base->UCR1 |= 1<<0;

}




void uart_softreset(UART_Type *base)
{
    base->UCR2 &= ~(1<<0);
    while((base->UCR2 & 0x01)==0);  /*等待复位完成*/

}


void putc(uint8_t byte)
{
    
    while(((UART1->USR2>>3) & 0X01)==0);  /*等待上一次传输完成 TXDC复位值是1，所以第一次进来不会卡住*/
    UART1->UTXD = byte;

}

void puts(uint8_t *str)
{
    uint8_t *p=str;
    while(*p)
    {
       putc(*p);
       p++; 
    }
}


uint8_t getc(void)
{
    while((UART1->USR2 & 0X1)==0);  /*等待接收到数据*/
    return (UART1->URXD & 0XFF);
}

/*
 * @description 		: 波特率计算公式，
 *    			  	  	  可以用此函数计算出指定串口对应的UFCR，
 * 				          UBIR和UBMR这三个寄存器的值
 * @param - base		: 要计算的串口。
 * @param - baudrate	: 要使用的波特率。
 * @param - srcclock_hz	:串口时钟源频率，单位Hz
 * @return		: 无
 */
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u;		//分子
    uint32_t denominator = 0U;		//分母
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0)
    {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK))
    {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator)
        {
            numerator = 1;
        }
        if (0 == denominator)
        {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider)
    {
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0)
    {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3)
    {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1); //要先写UBIR寄存器，然后在写UBMR寄存器，3592页 
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
        //base->ONEMS = UART_ONEMS_ONEMS(srcclock_hz / (1000 * divider));
    }

}

/*
 * @description : 防止编译器报错
 * @param 		: 无
 * @return		: 无
 */
void raise(int sig_nr) 
{

}

