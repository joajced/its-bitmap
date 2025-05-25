#include "stm32f4xx_hal.h"
#include "init.h"
#include "lcd.h"
#include "LCD_GUI.h"
#include "input.h"
#include "print_screen.h"
#include "button_input.h"
#include "header_parser.h"

#define TEILAUFGABE_A

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
		
		DWORD clrUsed = (infoHeader.biClrUsed == 0) ? 256 : infoHeader.biClrUsed;
		initBmpPrinter(infoHeader.biWidth, infoHeader.biHeight, infoHeader.biSizeImage, clrUsed);
		
		#if defined TEILAUFGABE_A
		printImage();
		
		#elif defined TEILAUFGABE_B
		printImageLines();
		
		#endif
		
		while (!isButtonPressed(S7));
		GUI_clear(WHITE);
	}
}
