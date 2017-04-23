/*
 * power.h
 *
 *  Created on: Mar 1, 2015
 *      Author: metalheart
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include "system.h"
#include "common/hal_common.h"

extern void led_init(const gpio_pin_t* cfg, uint16_t size);
extern void led_enable(const gpio_pin_t* cfg, uint16_t size, uint16_t id, uint8_t enable);

#endif /* LED_H_ */
