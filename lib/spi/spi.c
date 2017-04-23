#include "spi.h"

uint8_t spi_init(const spi_t* spi, uint32_t alt)
{
	SPI_I2S_DeInit(spi->spi);

	//TODO: move to some other section of code?
	if (spi->spi == SPI1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	}

	gpio_init(spi->pins, NUM_SPI_PINS);

	SPI_Init(spi->spi, &spi->config);
	SPI_Cmd(spi->spi, ENABLE);
}

uint8_t spi_deinit(const spi_t* spi)
{

}

void spi_assert(const spi_t* spi, uint8_t enable)
{
	if (enable) {
		GPIO_ResetBits(spi->pins[CS].gpioReg, spi->pins[CS].pin);
	} else {
		GPIO_SetBits(spi->pins[CS].gpioReg, spi->pins[CS].pin);
	}
}

uint8_t spi_write_byte(const spi_t* spi, uint8_t byte)
{
    /** flush away any rogue data in rx buffer **/
    if (SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_RXNE) == SET)
    {
    	SPI_ReceiveData8(spi->spi);
    }
    /** Loop while DR register in not empty **/
    while(SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_TXE) == RESET);

	/** Send byte through the SPI1 peripheral **/
    SPI_SendData8(spi->spi, byte);
}


uint8_t spi_read_byte(const spi_t* spi)
{
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(spi->spi, SPI_I2S_FLAG_RXNE) == SET);

	/* Return the byte read from the SPI bus */
	return (uint8_t)SPI_ReceiveData8(spi->spi);
}

