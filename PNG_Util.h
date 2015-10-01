/*
 * PNG_Util.h
 *
 *  Created on: 30 Sep 2015
 *      Author: vincentvillani
 */

#ifndef PNG_UTIL_H_
#define PNG_UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <png.h>

#define PNG_BYTES_TO_CHECK 4

typedef struct PNG
{
	png_structp pngStruct;
	png_infop pngInfo;
	png_byte** pngData; //The actual pixel data, [row][column]

	png_uint_32 imageWidth;
	png_uint_32 imageHeight;

	png_uint_32 bitDepth; //Bits per CHANNEL! - NOT PIXEL
	png_uint_32 channelNum; //Number of channels
	png_uint_32 colorType; //Colour type (RGB, RGBA, Luminance, luminance alpha... palette... etc)

} PNG;

//Open and read PNG file
bool png_util_read_png_file(FILE** filePtr, char* filename, PNG* pngDataStruct);



#endif /* PNG_UTIL_H_ */
