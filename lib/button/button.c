/*
 * power.c
 *
 *  Created on: Mar 1, 2015
 *      Author: metalheart
 */

#include "button.h"

void btn_init(const gpio_pin_t* cfg, uint16_t size) {
	GPIO_InitTypeDef gpioConfig;
	gpioConfig.GPIO_OType = GPIO_OType_PP;
	gpioConfig.GPIO_Mode = GPIO_Mode_IN;
	gpioConfig.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpioConfig.GPIO_Speed = GPIO_Speed_2MHz;

	for (uint16_t i = 0; i < size; ++i) {
		RCC_AHBPeriphClockCmd(gpio_to_rcc(&cfg[i]), ENABLE);
		gpioConfig.GPIO_Pin = cfg[i].pin;
		GPIO_Init(cfg[i].gpioReg, &gpioConfig);
	}
}

uint8_t btn_is_pressed(const gpio_pin_t* cfg, uint16_t size, uint16_t id) {
	assert_param(id < size);
	uint8_t data = 0;
	for (uint8_t i = 0; i < 10; ++i) {
		if (GPIO_ReadInputDataBit(cfg[id].gpioReg, cfg[id].pin)) {
			++ data;
		}
	}

	return data > 8;
}

