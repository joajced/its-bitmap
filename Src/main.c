#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "BMP_types.h"
#include "delay.h" // TESTEN

#define DISPLAY_LENGTH 480
#define DISPLAY_HEIGHT 320

int main()
{
	initITSboard();
	GUI_init(DEFAULT_BRIGHTNESS);
	
	for (WORD y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for (WORD x = 0; x < DISPLAY_LENGTH; x++)
		{
			Coordinate temp = {x, y};
			GUI_drawPoint(temp, GREEN, DOT_PIXEL_1X1, DOT_FILL_AROUND);
		}
	}
	
	while (1);
}
