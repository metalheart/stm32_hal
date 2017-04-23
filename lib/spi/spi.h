/*
 * hal_common.h
 *
 *  Created on: Apr 5, 2015
 *      Author: metalheart
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include "system.h"
#include "common/hal_common.h"

typedef enum {
	MISO = 0,
	MOSI,
	CLK,
	CS,
	NUM_SPI_PINS
} spi_pin_fn_t;

typedef struct {
	SPI_TypeDef* spi;
	SPI_InitTypeDef config;
	gpio_pin_t pins[NUM_SPI_PINS];
} spi_t;

extern uint8_t spi_init(const spi_t* spi, uint32_t alt);
extern void spi_assert(const spi_t* spi, uint8_t enable);
extern uint8_t spi_write_byte(const spi_t* spi, uint8_t byte);
extern uint8_t spi_read_byte(const spi_t* spi);
extern uint8_t spi_deinit(const spi_t* spi);

#endif /* SPI_H_ */
