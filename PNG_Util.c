/*
 * PNG_Util.cpp
 *
 *  Created on: 30 Sep 2015
 *      Author: vincentvillani
 */

#include "PNG_Util.h"

//----- PRIVATE -----
//Check if this file is a PNG file

//Returns true if the file is a PNG file
static bool png_util_file_is_png(FILE* file);




//----- PRIVATE IMPLEMENTATION -----

bool png_util_file_is_png(FILE* filePtr)
{

	char buf[PNG_BYTES_TO_CHECK];

	//Check that we read (PNG_BYTES_TO_CHECK) bytes as expected
	if(fread(buf, 1, PNG_BYTES_TO_CHECK, filePtr) != PNG_BYTES_TO_CHECK)
	{
		printf("Incorrect number of PNG bytes read\n");

		return false;
	}

	//Returns 0 if file is a PNG
	//Returns non-zero if file IS NOT A PNG
	//memory to read, start byte, number of bytes to check
	bool fileIsPng = !png_sig_cmp((png_const_bytep)buf, 0, PNG_BYTES_TO_CHECK);

	return fileIsPng;
}








//----- PUBLIC IMPLEMENTATION -----

bool png_util_read_png_file(FILE** filePtr, char* filename, PNG_Util_Data_Struct* pngDataStruct)
{

	//rb = read, binary
	*filePtr = fopen(filename, "rb");

	if(filePtr == NULL)
	{
		printf("Cannot open PNG File\n");
		return false;
	}

	//Check if this file is actually a PNG file
	if(!png_util_file_is_png(*filePtr))
	{
		printf("file is not a PNG file\n");
		return false;
	}

	//Allocate memory for the png_structp
	pngDataStruct->pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	//Check if allocating the png_structp for this image worked
	if(pngDataStruct->pngStruct == NULL)
	{
		printf("Unable to allocate memory for the png_structp\n");
		return false;
	}

	//Allocate memory for the png info (data)
	pngDataStruct->pngInfo = png_create_info_struct(pngDataStruct->pngStruct);

	//Check if allocating the png_info for this image worked
	if(pngDataStruct->pngInfo == NULL)
	{
		printf("Unable to allocate memory for the png_infop\n");

		//Free the png_structp that was already successfully allocated above
		png_destroy_read_struct( &(pngDataStruct->pngStruct), NULL, NULL);
		pngDataStruct->pngStruct = NULL;
		return false;
	}

	//Tell the library that we have already read PNG_BYTES_TO_CHECK worth of bytes (it uses the file pointer)
	png_set_sig_bytes(pngDataStruct->pngStruct, PNG_BYTES_TO_CHECK);

	//Setup the file I/O for this PNG, we are going to load it from a file
	png_init_io(pngDataStruct->pngStruct, *filePtr);

    //Now call png_read_info to receive the file info from the header file?
    png_read_info(pngDataStruct->pngStruct, pngDataStruct->pngInfo);

    //Get a bunch of data about the image
    pngDataStruct->imageWidth = png_get_image_width(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->imageHeight = png_get_image_height(pngDataStruct->pngStruct, pngDataStruct->pngInfo);

    pngDataStruct->bitDepth = png_get_bit_depth(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->channelNum = png_get_channels(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->colorType = png_get_color_type(pngDataStruct->pngStruct, pngDataStruct->pngInfo);

    //Allocate memory to hold the PNG data

    //Allocate memory for the row POINTERS
    pngDataStruct->pngData = malloc(sizeof(png_byte*) * pngDataStruct->imageHeight);


    for(int i = 0; i < pngDataStruct->imageHeight; ++i)
    {
    	//For each row, allocate enough bytes to store the image
    	pngDataStruct->pngData[i] = png_malloc(pngDataStruct->pngStruct, png_get_rowbytes(pngDataStruct->pngStruct,
    			pngDataStruct->pngInfo));
    }

    //Now its time to actually read the image
    //Read the entire image
    png_read_image(pngDataStruct->pngStruct, pngDataStruct->pngData);

    fclose(*filePtr);

    //Everything has gone successfully, return true
	return true;
}





