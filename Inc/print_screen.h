#ifndef PRINT_SCREEN_H
#define PRINT_SCREEN_H

#include "BMP_types.h"
#include "LCD_GUI.h"

typedef struct RLE_PAIR
{
	BYTE byte1;
	BYTE byte2;
} RLE_PAIR;

void printImage(int width, int height, int size, int clrUsed);

void fillPalette(int clrUsed);

void readBytePair();

void processBytePair();

int printPixel(RGBQUAD color);

#endif
