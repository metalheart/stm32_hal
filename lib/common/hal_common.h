/*
 * hal_common.h
 *
 *  Created on: Apr 5, 2015
 *      Author: metalheart
 */

#ifndef HAL_COMMON_H_
#define HAL_COMMON_H_

#include <stdint.h>
#include "system.h"

typedef struct gpio_pin {
	GPIO_TypeDef* gpioReg;
	uint16_t pin;
	uint32_t mode;
} gpio_pin_t;

#define MODE_OUT 		(0xff - 10)
#define MODE_OUT_INV 	(MODE_OUT + 1)
#define MODE_IN 		(MODE_OUT + 2)
#define MODE_AN 		(MODE_OUT + 3)

extern void gpio_init(const gpio_pin_t* pins, uint8_t sz);
extern void gpio_set(const gpio_pin_t* pin, uint8_t val);
extern uint8_t gpio_read(const gpio_pin_t* pin);

static inline uint32_t gpio_to_rcc(const gpio_pin_t* gpio) {
#ifndef STM32F4XX
	switch((uint32_t)gpio->gpioReg) {
	case (uint32_t)GPIOA:
		return RCC_AHBPeriph_GPIOA;
	case (uint32_t)GPIOB:
		return RCC_AHBPeriph_GPIOB;
	case (uint32_t)GPIOC:
		return RCC_AHBPeriph_GPIOC;
	case (uint32_t)GPIOD:
		return RCC_AHBPeriph_GPIOD;
	case (uint32_t)GPIOE:
		return RCC_AHBPeriph_GPIOE;
	case (uint32_t)GPIOF:
		return RCC_AHBPeriph_GPIOF;
	}
	return 0;
#else
	switch((uint32_t)gpio->gpioReg) {
	case (uint32_t)GPIOA:
		return RCC_AHB1Periph_GPIOA;
	case (uint32_t)GPIOB:
		return RCC_AHB1Periph_GPIOB;
	case (uint32_t)GPIOC:
		return RCC_AHB1Periph_GPIOC;
	case (uint32_t)GPIOD:
		return RCC_AHB1Periph_GPIOD;
	case (uint32_t)GPIOE:
		return RCC_AHB1Periph_GPIOE;
	case (uint32_t)GPIOF:
		return RCC_AHB1Periph_GPIOF;
	}
	return 0;
#endif
}

static inline uint32_t gpio_to_pinsource(const gpio_pin_t* gpio) {
	switch(gpio->pin) {
	case GPIO_Pin_0:
		return EXTI_PinSource0;
	case GPIO_Pin_1:
		return EXTI_PinSource1;
	case GPIO_Pin_2:
		return EXTI_PinSource2;
	case GPIO_Pin_3:
		return EXTI_PinSource3;
	case GPIO_Pin_4:
		return EXTI_PinSource4;
	case GPIO_Pin_5:
		return EXTI_PinSource5;
	case GPIO_Pin_6:
		return EXTI_PinSource6;
	case GPIO_Pin_7:
		return EXTI_PinSource7;
	case GPIO_Pin_8:
		return EXTI_PinSource8;
	case GPIO_Pin_9:
		return EXTI_PinSource9;
	case GPIO_Pin_10:
		return EXTI_PinSource10;
	case GPIO_Pin_11:
		return EXTI_PinSource11;
	case GPIO_Pin_12:
		return EXTI_PinSource12;
	case GPIO_Pin_13:
		return EXTI_PinSource13;
	case GPIO_Pin_14:
		return EXTI_PinSource14;
	case GPIO_Pin_15:
		return EXTI_PinSource15;
	}
	return 0;
}
#endif /* HAL_COMMON_H_ */
