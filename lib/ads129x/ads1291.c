#include "ads1291.h"
#include "ads1291_commands.h"

#include <stm32l1xx.h>

#include <stm32l1xx_rcc.h>
#include <stm32l1xx_spi.h>
#include <stm32l1xx_gpio.h>
#include <stm32l1xx_exti.h>
#include <stm32l1xx_syscfg.h>
#include "delay.h"

NVIC_InitTypeDef ads129x_exti_struct;

void ads129x_spi_assert() {
  GPIO_ResetBits(ADS129x_CS_PORT, ADS129x_CS_PIN);
}

void ads129x_spi_deassert() {
  GPIO_SetBits(ADS129x_CS_PORT, ADS129x_CS_PIN);
}

uint8_t ads129x_send_byte(uint8_t byte)
{
    /** flush away any rogue data in rx buffer **/
    if (SPI_I2S_GetFlagStatus(ADS129x_SPI, SPI_I2S_FLAG_RXNE) == SET)
    {
        SPI_I2S_ReceiveData(ADS129x_SPI);
    }
    /** Loop while DR register in not empty **/
    while(SPI_I2S_GetFlagStatus(ADS129x_SPI, SPI_I2S_FLAG_TXE) == RESET);

	/** Send byte through the SPI1 peripheral **/
	SPI_I2S_SendData(ADS129x_SPI, byte);

	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(ADS129x_SPI, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return (uint8_t)SPI_I2S_ReceiveData(ADS129x_SPI);
}

uint8_t ads129x_read_byte()
{
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(ADS129x_SPI, SPI_I2S_FLAG_RXNE) == SET);

	/* Return the byte read from the SPI bus */
	return (uint8_t)SPI_I2S_ReceiveData(ADS129x_SPI);
}

void ads129x_cmd(uint8_t cmd, uint8_t pull_cs) {
	if (pull_cs) ads129x_spi_assert();
	ads129x_send_byte(cmd);
	ads129x_send_byte(0x0);
	if (pull_cs) ads129x_spi_deassert();
}

void ads129x_rwrite(uint8_t reg, const uint8_t *val, uint8_t sz, uint8_t pull_cs) {
	if (pull_cs) ads129x_spi_assert();
	ads129x_send_byte(WREG | reg);
	ads129x_send_byte(sz);
	while (sz--) {
		ads129x_send_byte(*val++);
	}
	if (pull_cs) ads129x_spi_deassert();
}

void ads129x_rread(uint8_t reg, uint8_t *buff, uint8_t sz, uint8_t pull_cs) {
	if (pull_cs) ads129x_spi_assert();
	ads129x_send_byte(RREG | reg);
	ads129x_send_byte(sz);
	while (sz--) {
		*(buff++) = ads129x_send_byte(0x0);
	}
	if (pull_cs) ads129x_spi_deassert();
}

extern uint8_t ads129x_hw_init() {
	//init CS pin
	GPIO_InitTypeDef gpioConfig;

	ADS129x_CS_RCC_CMD(ADS129x_CS_RCC, ENABLE);

	gpioConfig.GPIO_Pin = ADS129x_CS_PIN;
	gpioConfig.GPIO_Mode = GPIO_Mode_OUT;
	gpioConfig.GPIO_Speed = GPIO_Speed_2MHz;
	gpioConfig.GPIO_OType = GPIO_OType_PP;
	gpioConfig.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ADS129x_CS_PORT, &gpioConfig);

#ifdef HW_VERSION_3
	ADS129x_EN_RCC_CMD(ADS129x_EN_RCC, ENABLE);
	//init reset pin
	gpioConfig.GPIO_Pin = ADS129x_EN_PIN;
	gpioConfig.GPIO_Mode = GPIO_Mode_OUT;
	gpioConfig.GPIO_Speed = GPIO_Speed_2MHz;
	gpioConfig.GPIO_OType = GPIO_OType_PP;
	gpioConfig.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ADS129x_EN_PORT, &gpioConfig);
	GPIO_ResetBits(ADS129x_EN_PORT, ADS129x_EN_PIN);
#endif
	//init interrupt pin
	ADS129x_INT_RCC_CMD(ADS129x_INT_RCC, ENABLE);

	gpioConfig.GPIO_Pin = ADS129x_INT_PIN;
	gpioConfig.GPIO_Mode = GPIO_Mode_IN;
	gpioConfig.GPIO_Speed = GPIO_Speed_2MHz;
	gpioConfig.GPIO_OType = GPIO_OType_PP;
	gpioConfig.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ADS129x_INT_PORT, &gpioConfig);

	//set up interrupt on data ready
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	ads129x_exti_struct.NVIC_IRQChannel = EXTI4_IRQn;
	ads129x_exti_struct.NVIC_IRQChannelPreemptionPriority = 0x1;//0x0F;
	ads129x_exti_struct.NVIC_IRQChannelSubPriority = 0x0;//0x0F;
	ads129x_exti_struct.NVIC_IRQChannelCmd = DISABLE;

	NVIC_Init(&ads129x_exti_struct);
	ads129x_spi_deassert();
}

extern void ads129x_reset() {
#ifdef HW_VERSION_2
	ads129x_cmd(NRESET, 1);
#elif HW_VERSION_3
	//GPIO_ResetBits(ADS129x_EN_PORT, ADS129x_EN_PIN);
	//delay_ms(100);
	GPIO_SetBits(ADS129x_EN_PORT, ADS129x_EN_PIN);
#endif
	ads129x_cmd(NRESET, 1);
	delay_ms(100);
}
uint8_t ads129x_init(uint8_t enable_loff, uint8_t enable_rld, uint8_t sample_rate, uint8_t chan_gain) {
	ads129x_reset();

	ads129x_cmd(SDATAC, 1);
	delay_us(20);

	uint8_t reg_val;

	reg_val = CONFIG1_const | sample_rate;
	ads129x_rwrite(CONFIG1, &reg_val, 1, 1);
	delay_us(20);

	reg_val = CONFIG2_const | PDB_REFBUF | (enable_loff ? PDB_LOFF_COMP : 0x0);
	ads129x_rwrite(CONFIG2, &reg_val, 1, 1);
	delay_us(100);

	reg_val = CH1SET_const | chan_gain;
	ads129x_rwrite(CH1SET, &reg_val, 1, 1);
	delay_us(20);

	reg_val = CH2SET_const | CH2_INP_SHRT | CH2_PD2;
	ads129x_rwrite(CH2SET, &reg_val, 1, 1);
	delay_us(20);

	reg_val = RESP2_const | RLDREF_INT | CALIB_ON;
	ads129x_rwrite(RESP2, &reg_val, 1, 1);
	delay_us(20);

	reg_val = RLD_SENSP_const | (enable_rld ? PDB_RLD : 0x0);
	ads129x_rwrite(RLD_SENS, &reg_val, 1, 1);
	delay_us(20);

	reg_val = LOFF_const | (enable_loff ? ILEAD_OFF_6nA | COMP_TH_95 : 0x0);
	ads129x_rwrite(LOFF, &reg_val, 1, 1);
	delay_us(20);

	reg_val = LOFF_SENSP_const | (enable_loff ? LOFF1N | LOFF1P : 0x0);
	ads129x_rwrite(LOFF_SENS, &reg_val, 1, 1);

	reg_val = RESP1_const;
	ads129x_rwrite(RESP1, &reg_val, 1, 1);
	delay_us(20);

	reg_val = 0x0;
	ads129x_rwrite(GPIO, &reg_val, 1, 1);
	delay_us(20);

	reg_val = 0xff;
	ads129x_rread(ADS_ID, &reg_val, 1, 1);
	delay_us(20);

	for (uint16_t i = 0; i <= 0xff; ++i)
	{
		reg_val = 0xff;
		ads129x_rread(i, &reg_val, 1, 1);
		if (reg_val != 0xff) {
			delay_us(40);
		} else
		{
			delay_us(20);
		}
	}

	if ((reg_val & (ID_ADS1x9x | ID_const)) || (reg_val & (ID_ADS1291 | ID_const))) {
		ads129x_cmd(OFFSETCAL, 1);
		delay_ms(100);

		return 1;
	} else {
		return 0;
	}

}
