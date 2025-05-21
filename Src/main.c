#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "BMP_types.h"
#include "input.h"
#include "button_input.h"

// ZUM TESTEN
#include "lcd.h"
#include "delay.h"
#include <stdio.h>

#define DISPLAY_LENGTH 480
#define DISPLAY_HEIGHT 320

int main()
{
	initITSboard();
	GUI_init(DEFAULT_BRIGHTNESS);
	initInput();
	
	while (1)
	{
		// 1. Wait for file (already done inside function)
		
		openNextFile();
		
		// 2. Process file and print to display
		
		// COMread und/oder nextChar
		
		for (WORD y = 0; y < DISPLAY_HEIGHT; y++)
		{
			for (WORD x = 0; x < DISPLAY_LENGTH; x++)
			{
				Coordinate temp = {x, y};
				GUI_drawPoint(temp, GREEN, DOT_PIXEL_1X1, DOT_FILL_AROUND);
			}
		}
		
		// 3. Wait for button for next file
		
		while (!isButtonPressed(S7));
	}
}
