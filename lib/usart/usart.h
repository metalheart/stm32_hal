/*
 * usart.h
 *
 *  Created on: Feb 22, 2015
 *      Author: metalheart
 */

#ifndef USART_H_
#define USART_H_

#include <stdint.h>
#include "system.h"
#include "common/hal_common.h"

typedef void (*usart_handler_t)();

typedef struct {
	USART_TypeDef *usart;
	USART_InitTypeDef cfg;
	gpio_pin_t rx_pin;
	gpio_pin_t tx_pin;
	usart_handler_t handler;
	uint8_t use_it;
} usart_t;

extern uint8_t usart_init(usart_t* usart);
extern uint8_t usart_set_handler(usart_t* usart, usart_handler_t handler);
//extern void uart1_reconfigure(uint32_t speed, uint8_t use_it);
//extern void usart1_set_handler(usart_handler_t handler);

#endif /* USART_H_ */
