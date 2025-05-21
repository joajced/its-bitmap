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

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

// In ein Modul reinpacken
WORD rgb888To565(BYTE r, BYTE g, BYTE b);

int main()
{
	initITSboard();
	GUI_init(DEFAULT_BRIGHTNESS);
	initInput();
	
	while (1)
	{
		// 1. Wait until file is opened
		
		openNextFile();
		
		// 2. Process file and print to display
		
			// - 3 bytes per pixel, row must be multiple of 4 -> padding!
			// - Something with modulo (% 4)?
			// - We also need to print either to the width of the image or the edge of the screen
			//   Whatever comes first.
		
		// GUI_drawPoint uses the RGB565 color format. Umwandeln jedes Mal?
		
		BITMAPFILEHEADER fileHeader;
		COMread((char*) &fileHeader, sizeof(BITMAPFILEHEADER), 1);
		DWORD pixelData = fileHeader.bfOffBits;
		
		BITMAPINFOHEADER infoHeader;
		COMread((char*) &infoHeader, sizeof(BITMAPINFOHEADER), 1);
		LONG imgWidth = infoHeader.biWidth;
		LONG imgHeight = infoHeader.biHeight;
		
		int bytesProZeile = ((imgWidth * 24 + 31) / 32) * 4;
		
		/*
		char temp[10];
		sprintf(temp, "%d", pixelData);
		lcdGotoXY(0, 0);
		lcdPrintS("Offset: ");
		lcdPrintS(temp);
		*/
		
		// Funktioniert nur fuer nicht komprimierte Bilder!!!
		
		for (WORD y = 0; y < DISPLAY_HEIGHT; y++)
		{
			for (WORD x = 0; x < DISPLAY_WIDTH; x++)
			{
				if (x * 3 >= bytesProZeile) break; // End of line
				
				RGBTRIPLE currPixel;
				COMread((char*) &currPixel, 3, 1);
				
				BYTE r = currPixel.rgbtRed;
				BYTE g = currPixel.rgbtGreen;
				BYTE b = currPixel.rgbtBlue;
				
				Coordinate currPos = {x, DISPLAY_HEIGHT - y - 1};
				COLOR currColor = rgb888To565(r, g, b);
				GUI_drawPoint(currPos, currColor, DOT_PIXEL_1X1, DOT_FILL_AROUND);
			}
		}
		
		
		// 3. Wait for button for next file
		
		while (!isButtonPressed(S7));
		GUI_clear(WHITE);
	}
}

WORD rgb888To565(BYTE r, BYTE g, BYTE b)
{
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}
