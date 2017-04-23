#include "hal_common.h"

void gpio_init(const gpio_pin_t* pins, uint8_t sz)
{
	GPIO_InitTypeDef gpioConfig;
	gpioConfig.GPIO_OType = GPIO_OType_PP;
	gpioConfig.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioConfig.GPIO_Speed = GPIO_Speed_2MHz;

	for (uint8_t i = 0; i < sz; ++i) {
		RCC_AHBPeriphClockCmd(gpio_to_rcc(&pins[i]), ENABLE);
		if (pins[i].mode < MODE_OUT)
		{
			gpioConfig.GPIO_Mode = GPIO_Mode_AF;
			GPIO_PinAFConfig(pins[i].gpioReg, gpio_to_pinsource(&pins[i].pin), pins[i].mode);
		}
		else if (pins[i].mode == MODE_IN)
		{
			gpioConfig.GPIO_Mode = GPIO_Mode_IN;
		}
		else if (pins[i].mode == MODE_AN)
		{
			gpioConfig.GPIO_Mode = GPIO_Mode_AN;
		}
		else
		{
			gpioConfig.GPIO_Mode = GPIO_Mode_OUT;
		}

		gpioConfig.GPIO_Pin = pins[i].pin;
		GPIO_Init(pins[i].gpioReg, &gpioConfig);
	}
}


void gpio_set(const gpio_pin_t* pin, uint8_t val)
{
	if (val && pin->mode != MODE_OUT_INV)
	{
		GPIO_SetBits(pin->gpioReg, pin->pin);
	}
	else
	{
		GPIO_ResetBits(pin->gpioReg, pin->pin);
	}
}

uint8_t gpio_read(const gpio_pin_t* pin)
{
	return GPIO_ReadInputDataBit(pin->gpioReg, pin->pin) == Bit_SET ? 1 : 0;
}
