#ifndef _ADS_1291_H_
#define _ADS_1291_H_

#include <stdint.h>
#include "system.h"
#include "ads1291_commands.h"

#define ADS129x_SPI         	SPI1

#define ADS129x_CS_RCC_CMD		RCC_AHBPeriphClockCmd
#define ADS129x_CS_RCC          RCC_AHBPeriph_GPIOB
#define ADS129x_CS_PORT         GPIOB
#define ADS129x_CS_PIN          GPIO_Pin_0

#define ADS129x_INT_RCC_CMD		 RCC_AHBPeriphClockCmd
#define ADS129x_INT_RCC          RCC_AHBPeriph_GPIOA
#define ADS129x_INT_PORT         GPIOA
#define ADS129x_INT_PIN          GPIO_Pin_4

#ifdef HW_VERSION_2
#elif HW_VERSION_3
#define ADS129x_EN_RCC_CMD		RCC_AHBPeriphClockCmd
#define ADS129x_EN_RCC          RCC_AHBPeriph_GPIOB
#define ADS129x_EN_PORT         GPIOB
#define ADS129x_EN_PIN          GPIO_Pin_11
#endif

extern NVIC_InitTypeDef ads129x_exti_struct;

extern void ads129x_spi_assert(void);
extern void ads129x_spi_deassert(void);
extern uint8_t ads129x_send_byte(uint8_t byte);
extern uint8_t ads129x_read_byte(void);
extern void ads129x_cmd(uint8_t cmd, uint8_t pull_cs);
extern void ads129x_rwrite(uint8_t reg, const uint8_t *val, uint8_t sz, uint8_t pull_cs);
extern void ads129x_rread(uint8_t reg, uint8_t *buff, uint8_t sz, uint8_t pull_cs);
extern void ads129x_write(uint8_t reg, const uint8_t *val, uint8_t sz, uint8_t pull_cs);
extern void ads129x_read(uint8_t reg, uint8_t *buff, uint8_t sz, uint8_t pull_cs);

extern uint8_t ads129x_hw_init();
extern void ads129x_reset();
extern uint8_t ads129x_init(uint8_t enable_loff, uint8_t enable_rld, uint8_t sample_rate, uint8_t chan_gain);
#endif// _ADS_1291_H_
