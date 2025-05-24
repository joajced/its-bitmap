#include "stm32f4xx_hal.h"
#include "init.h"
#include "lcd.h"
#include "LCD_GUI.h"
#include "input.h"
#include "print_screen.h"
#include "button_input.h"
#include "header_parser.h"

int main()
{
	initITSboard();
	GUI_init(DEFAULT_BRIGHTNESS);
	initInput();
	
	while (1)
	{
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;
		
		openNextFile();
		readHeaders(&fileHeader, &infoHeader);
		
		int clrUsed = (infoHeader.biClrUsed == 0) ? 256 : infoHeader.biClrUsed;
		lcdGotoXY(1, 1);
		lcdPrintS(" ");
		printImage(infoHeader.biWidth, infoHeader.biHeight, infoHeader.biSizeImage, clrUsed);
		
		while (!isButtonPressed(S7));
		GUI_clear(WHITE);
	}
}
