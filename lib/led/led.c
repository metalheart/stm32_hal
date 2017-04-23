/*
 * power.c
 *
 *  Created on: Mar 1, 2015
 *      Author: metalheart
 */

#include "led.h"

void led_init(const gpio_pin_t* cfg, uint16_t size) {
	GPIO_InitTypeDef gpioConfig;
	gpioConfig.GPIO_OType = GPIO_OType_PP;
	gpioConfig.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioConfig.GPIO_Speed = GPIO_Speed_2MHz;
	gpioConfig.GPIO_Mode = GPIO_Mode_OUT;

	for (uint16_t i = 0; i < size; ++i) {
		RCC_AHBPeriphClockCmd(gpio_to_rcc(&cfg[i]), ENABLE);
		gpioConfig.GPIO_Pin = cfg[i].pin;
		GPIO_Init(cfg[i].gpioReg, &gpioConfig);

		if (cfg[i].mode == MODE_OUT_INV) {
			GPIO_SetBits(cfg[i].gpioReg, cfg[i].pin);
		} else {
			GPIO_ResetBits(cfg[i].gpioReg, cfg[i].pin);
		}
	}
}

void led_enable(const gpio_pin_t* cfg, uint16_t size, uint16_t id, uint8_t enable) {
	assert_param(id < size);
	if (enable) {
		cfg[id].mode == MODE_OUT_INV ? GPIO_ResetBits(cfg[id].gpioReg, cfg[id].pin) : GPIO_SetBits(cfg[id].gpioReg, cfg[id].pin);
	} else {
		cfg[id].mode == MODE_OUT_INV ? GPIO_SetBits(cfg[id].gpioReg, cfg[id].pin) : GPIO_ResetBits(cfg[id].gpioReg, cfg[id].pin);
	}
}

