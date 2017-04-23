/*
 * uasrt.c
 *
 *  Created on: Feb 22, 2015
 *      Author: metalheart
 */
#include "usart.h"
#include <stm32l1xx.h>
#include <stm32l1xx_usart.h>
#include "hw_setup.h"

//TODO: implement handlers via changing interrupt vector address
typedef enum {
	USART1_HANDLER = 0,
	USART2_HANDLER,
	USART3_HANDLER,
	USART_HANDLER_SIZE
} UsartHandlerIndex;

static usart_handler_t usart_handlers[USART_HANDLER_SIZE];

void USART1_IRQHandler() {
	if (usart_handlers[USART1_HANDLER]) {
		(*(usart_handlers[USART1_HANDLER]))();
	} else {
		USART_ReceiveData(USART1);
	}
}

void USART2_IRQHandler() {
	if (usart_handlers[USART2_HANDLER]) {
		(*(usart_handlers[USART2_HANDLER]))();
	}
}

void USART3_IRQHandler() {
	if (usart_handlers[USART3_HANDLER]) {
		(*(usart_handlers[USART3_HANDLER]))();
	}
}
/*
void uart1_reconfigure(uint32_t speed, uint8_t use_it) {
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART1, DISABLE);

	USART1_InitStructure.USART_BaudRate = speed;// скорость
	USART_Init(USART1, &USART1_InitStructure); //инизиализируем

	USART_Cmd(USART1, ENABLE);
	if (use_it) {
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
}

void usart1_set_handler(usart_handler_t handler) {
	usart_handler = handler;
}
*/
uint8_t usart_init(usart_t* usart) {
	//USART_Cmd(usart->usart, DISABLE);
	USART_ITConfig(usart->usart, USART_IT_RXNE, DISABLE);
	USART_DeInit(usart->usart);

	GPIO_InitTypeDef GPIO_InitStructure;
	IRQn_Type irqChannel;
	UsartHandlerIndex handlerIndex;

	if (usart->usart == USART1) {
		RCC_AHBPeriphClockCmd(gpio_to_rcc(&usart->rx_pin), ENABLE);
		RCC_AHBPeriphClockCmd(gpio_to_rcc(&usart->tx_pin), ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		GPIO_PinAFConfig(usart->rx_pin.gpioReg, gpio_to_pinsource(&usart->rx_pin), GPIO_AF_USART1);
		GPIO_PinAFConfig(usart->tx_pin.gpioReg, gpio_to_pinsource(&usart->tx_pin), GPIO_AF_USART1);

		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = usart->rx_pin.pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(usart->rx_pin.gpioReg, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = usart->tx_pin.pin;
		GPIO_Init(usart->tx_pin.gpioReg, &GPIO_InitStructure);

		irqChannel = USART1_IRQn;
		handlerIndex = USART1_HANDLER;
	} else {
		return 0;
	}

	usart_handlers[handlerIndex] = usart->handler;

	USART_Init(usart->usart, &usart->cfg);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = irqChannel; //прерывание по uart2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0; //задаем приоритет в группе
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0; //задаем приоритет в подгруппе
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //разрешаем прерывание
	NVIC_Init(&NVIC_InitStructure); //инициализируем

	USART_Cmd(usart->usart, ENABLE);
	if (usart->use_it) {
		USART_ITConfig(usart->usart, USART_IT_RXNE, ENABLE);
	}

	USART_ClearFlag(usart->usart, USART_FLAG_CTS | USART_FLAG_LBD | USART_FLAG_TC | USART_FLAG_TXE | USART_FLAG_RXNE);

	return 1;
}

uint8_t usart_set_handler(usart_t* usart, usart_handler_t handler) {
	if (usart->usart == USART1) {
		usart_handlers[USART1_HANDLER] = handler;
		return 1;
	} else {
		return 0;
	}
}
