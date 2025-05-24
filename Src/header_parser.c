#include "header_parser.h"
#include "input.h"
#include "errorhandler.h"
#include "delay.h"

void readHeaders(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* infoHeader)
{
	ERR_HANDLER(1 != COMread((char*) fileHeader, sizeof(BITMAPFILEHEADER), 1),
		"error: cannot read file header");
	
	ERR_HANDLER(1 != COMread((char*) infoHeader, sizeof(BITMAPINFOHEADER), 1),
		"error: cannot read info header");
	
	if (fileHeader->bfType != BMP_SIGNATURE)
	{
		ERR_HANDLER(1, "error: file format is not BMP");
	}
	
	if (infoHeader->biBitCount != 8 || infoHeader->biCompression != BI_RLE8)
	{
		ERR_HANDLER(1, "error: invalid encoding");
	}
}
