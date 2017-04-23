/*
 * power.h
 *
 *  Created on: Mar 1, 2015
 *      Author: metalheart
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include "system.h"
#include "common/hal_common.h"

extern void btn_init(const gpio_pin_t* cfg, uint16_t size);
extern uint8_t btn_is_pressed(const gpio_pin_t* cfg, uint16_t size, uint16_t id);

#endif /* BUTTON_H_ */
