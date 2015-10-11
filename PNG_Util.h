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
	png_structp pngStruct; //Basically a handle to the PNG
	png_infop pngInfo; //Information derived from the header of the PNG
	png_byte** pngData; //The actual pixel data, [row][column]

	png_uint_32 imageWidth; //Image width in pixels
	png_uint_32 imageHeight; //Image height in pixels

	png_uint_32 bitDepth; //Bits per CHANNEL! - NOT PIXEL
	png_uint_32 channelNum; //Number of channels
	png_uint_32 colorType; //Colour type (RGB, RGBA, Luminance, luminance alpha... palette... etc)
	png_byte interlaceType; //Interlace type
	png_byte compressionType; //Compression type
	png_byte filterType; //Filter type

} PNG;

//Open and read PNG file
bool png_util_read_png_file(char* filename, PNG* pngDataStruct);

//Open and write a PNG file
bool png_util_write_png_file(char* filename, PNG* png);

//Returns the number of bytes a message could store if you used x number of bits per channel
//x = messageBitsPerChannel
uint32_t png_util_total_message_byte_storage(PNG* png);


//Write a message to a PNG image
void png_util_write_message(PNG* png, char* message, uint32_t messageCharacterLength);

//Read a message from a PNG image
char* png_util_read_message(PNG* png);


void png_util_free_PNG(PNG* png);


#endif /* PNG_UTIL_H_ */
