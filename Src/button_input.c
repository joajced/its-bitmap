#include "stm32f4xx_hal.h"

int isButtonPressed(int offset)
{
	int mask = (0x01u << offset);
	int input = GPIOF->IDR;
	return !(input & mask);
}
