#ifndef __GPIO_H
#define __GPIO_H

#include "fsl_iomuxc.h"

/* 枚举类型和结构体定义 */
typedef enum _gpio_pin_direction{
    GPIO_INPUT = 0U,
    GPIO_OUTPUT = 1U,

}gpio_pin_direction_t;

typedef enum _gpio_interrupt_mode{
    GPIO_NoIntmode=5U,
    GPIO_IntLowLevel = 0U,
    GPIO_IntHighLevel = 1U,
    GPIO_IntRisingEdge = 2U,
    GPIO_IntFallingEdge = 3U,
    GPIO_IntRisingOrFallingEdge = 4U,

}gpio_interrupt_mode_t;


/* GPIO 配置结构体 */
typedef struct gpio_pin_config
{
    gpio_pin_direction_t direction;
    gpio_interrupt_mode_t mode;
    uint8_t gpio_value;
}gpio_pin_config_t;



void gpio_init(GPIO_Type *base,uint8_t pin,gpio_pin_config_t *config);
uint8_t gpio_read_pin(GPIO_Type *base ,uint8_t pin);
void gpio_write_pin(GPIO_Type *base ,uint8_t pin,uint8_t value);
void gpio_int_config(GPIO_Type *base,uint8_t pin,gpio_interrupt_mode_t mode);
void gpio_intenable(GPIO_Type *base,uint8_t pin);
void gpio_intdisable(GPIO_Type *base,uint8_t pin);
void gpio_clearintflags(GPIO_Type* base, uint8_t pin);

#endif











