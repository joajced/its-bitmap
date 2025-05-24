#ifndef PRINT_SCREEN_H
#define PRINT_SCREEN_H

#include "BMP_types.h"
#include "LCD_GUI.h"

typedef struct RLE_PAIR
{
	BYTE byte1;
	BYTE byte2;
} RLE_PAIR;

void printImage(DWORD width, DWORD height, DWORD size, DWORD clrUsed);

void fillPalette(DWORD clrUsed);

void readBytePair();

void processBytePair();

int printPixel(RGBQUAD color);

#endif
