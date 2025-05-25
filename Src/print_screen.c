#include <stdlib.h>
#include "print_screen.h"
#include "errorhandler.h"
#include "input.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

#define END_OF_LINE   (currPair.byte1 == 0x00 && currPair.byte2 == 0x00)
#define END_OF_BITMAP (currPair.byte1 == 0x00 && currPair.byte2 == 0x01)
#define DELTA         (currPair.byte1 == 0x00 && currPair.byte2 == 0x02)
#define ABS_RUN       (currPair.byte1 == 0x00 && currPair.byte2 >= 0x03)
#define RLE_RUN       (currPair.byte1 >= 0x01)

// Pixel data
DWORD imgWidth, imgHeight, imgSize;
RGBQUAD *palette = NULL;
RLE_PAIR currPair;

// Cursor
DWORD consumedBytes;
DWORD x, y;

// Teilaufgabe A
void printImage()
{
	while (consumedBytes < imgSize)
	{
		readBytePair();
		processBytePair();
	}
}

// Teilaufgabe B
void printImageLines()
{
	
}

void initBmpPrinter(DWORD width, DWORD height, DWORD size, DWORD clrUsed)
{
	imgWidth  = width;
	imgHeight = height;
	imgSize   = size;
	
	consumedBytes = 0;
	x = 0;
	y = 0;
	
	currPair.byte1 = 0x00;
	currPair.byte2 = 0x00;
	
	if (palette != NULL)
	{
		free(palette);
		palette = NULL;
	}
	
	palette = malloc(clrUsed * sizeof(RGBQUAD));
	
	for (DWORD i = 0; i < clrUsed; i++)
	{
		ERR_HANDLER(1 != COMread((char*) &palette[i], sizeof(RGBQUAD), 1),
			"error: cannot fill palette");
	}
}

void readBytePair()
{
	ERR_HANDLER(1 != COMread((char*) &currPair, sizeof(RLE_PAIR), 1),
		"error: cannot read pixel data");
	consumedBytes += sizeof(RLE_PAIR);
}

void processBytePair()
{
	if (END_OF_LINE)
	{
		x = 0;
		y++;
	}
	else if (END_OF_BITMAP) return;
	else if (DELTA)
	{
		readBytePair();
		x += currPair.byte1;
		y += currPair.byte2;
	}
	else if (ABS_RUN)
	{
		for (DWORD i = 0; i < currPair.byte2; i++)
		{
			BYTE nextByte = nextChar();
			RGBQUAD color = palette[nextByte];
			consumedBytes++;
			
			if (!printPixel(color))
			{
				x++;
			}
		}
		
		if (currPair.byte2 % 2 != 0)
		{
			nextChar();
			consumedBytes++;
		}
	}
	else if (RLE_RUN)
	{
		RGBQUAD color = palette[currPair.byte2];
		
		for (DWORD i = 0; i < currPair.byte1; i++)
		{
			if (!printPixel(color))
			{
				x++;
			}
		}
	}
}

int printPixel(RGBQUAD color)
{
	if (x >= DISPLAY_WIDTH)
	{
		do
		{
			readBytePair();
		}
		while (!END_OF_LINE);
		processBytePair();
		
		return -1; // Sonderfall, das x++ danach soll nicht ausgefuehrt werden
	}
	
	if (y >= DISPLAY_HEIGHT)
	{
		do
		{
			readBytePair();
		}
		while (!END_OF_BITMAP);
		processBytePair();
		
		return -1; // Sonderfall, das x++ danach soll nicht ausgefuehrt werden
	}
	
	Coordinate pos = {x, DISPLAY_HEIGHT - y - 1};
	
	BYTE r = color.rgbRed;
	BYTE g = color.rgbGreen;
	BYTE b = color.rgbBlue;
	WORD color565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
	
	GUI_drawPoint(pos, color565, DOT_PIXEL_1X1, DOT_FILL_AROUND);
	return 0;
}
