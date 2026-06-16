#include "gpio.h"




void gpio_init(GPIO_Type *base,uint8_t pin,gpio_pin_config_t *config)
{
    if(config->direction == GPIO_INPUT) /*是结构体指针，用来指向配置结构体*/
    base->GDIR &=~(1<<pin);
    else 
    {
        base->GDIR |=(1<<pin);
        gpio_write_pin(base,pin,config->gpio_value);
    }
    gpio_int_config(base,pin,config->mode); /*GPIO中断初始化*/
}

uint8_t gpio_read_pin(GPIO_Type *base ,uint8_t pin)
{
    return((base->DR)>>pin & 0x01);

}

void gpio_write_pin(GPIO_Type *base ,uint8_t pin,uint8_t value)
{
     if(value == 1)
     base->DR |= (1<<pin);
     else base->DR &=~(1<<pin); 
}

/*GPIO中断初始化*/
void gpio_int_config(GPIO_Type *base,uint8_t pin,gpio_interrupt_mode_t mode)
{
    volatile uint32_t *icr;
    uint32_t icrshift;
    icrshift = pin;
    base->EDGE_SEL &= ~(1<<pin); /*置1时忽视ICR[n] setting*/
    if(pin<16)
    icr=&(base->ICR1);
    else 
    {
        icr=&(base->ICR2);
        icrshift-=16; 
    }
    switch (mode)
    {
    case GPIO_IntLowLevel:
        *icr&=~(3<<2*icrshift);
        break;
    case GPIO_IntHighLevel:
        *icr&=~(3<<2*icrshift);
        *icr|=(1<<2*icrshift);
        break;    
    case GPIO_IntRisingEdge:
        *icr&=~(3<<2*icrshift);
        *icr|=(2<<2*icrshift);
        break;
    case GPIO_IntFallingEdge:
        *icr|=(3<<2*icrshift);
        break;
    case GPIO_IntRisingOrFallingEdge:
        base->EDGE_SEL |= 1<<pin;
        break;        
    default:
        break;
    }


}


void gpio_intenable(GPIO_Type *base,uint8_t pin)
{
    base->IMR |=1<<pin; /*使能GPIO中断*/
}

void gpio_intdisable(GPIO_Type *base,uint8_t pin)
{
    base->IMR &= ~(1<<pin); /*不使能GPIO中断*/
}


void gpio_clearintflags(GPIO_Type* base, uint8_t pin)
{

    base->ISR |= 1<<pin;  /*清中断标志*/

}