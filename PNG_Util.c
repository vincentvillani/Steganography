/*
 * PNG_Util.cpp
 *
 *  Created on: 30 Sep 2015
 *      Author: vincentvillani
 */

#include "PNG_Util.h"

//----- PRIVATE -----

//Static in this context and in C means that this can only be accessed inside this compilation unit
//Check if this file is a PNG file
//Returns true if the file is a PNG file
static bool file_is_png(FILE* file);

//For now this only writes one bit per channel
static void write_byte(PNG* png, uint32_t* rowIndex, uint32_t* columnIndex, png_byte byteToWrite);


static png_byte read_byte(PNG* png, uint32_t* rowIndex, uint32_t* columnIndex);



//----- PRIVATE IMPLEMENTATION -----

static bool file_is_png(FILE* filePtr)
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




static void write_byte(PNG* png, uint32_t* rowIndex, uint32_t* columnIndex, png_byte byteToWrite)
{
	//This is the bit we use to clear and write to the destination bit
	png_byte destinationBit = 1;

	//Write bits, across each channel
	for(int i = 0; i < 8; ++i)
	{
		//Bit to get from the byteToWrite this time through the loop
		png_byte sourceBit = 1 << i;

		//Clear the destination bit before it is set
		png->pngData[*rowIndex][*columnIndex] &= ~(destinationBit);

		//What is the actual value of the source bit in byteToWrite?
		png_byte sourceBitValue = byteToWrite & sourceBit;

		//Move this bit value into the destination bit position
		sourceBitValue >>= i;

		//Write the sourceBitValue to the destination bit
		png->pngData[*rowIndex][*columnIndex] |= sourceBitValue;

		//Move across one column index to the next channel
		*columnIndex += 1;

		//Will our next iteration though the loop cause us too go over to the next row?
		if( *columnIndex > png->imageWidth - 1)
		{
			*columnIndex = 0;
			*rowIndex += 1;
		}

	}


}



static png_byte read_byte(PNG* png, uint32_t* rowIndex, uint32_t* columnIndex)
{
	//Start the result byte at 0
	png_byte resultByte = 0;

	//This is the bit that we will be reading from the image, at each channel
	png_byte sourceBit = 1;

	for(int i = 0; i < 8; ++i)
	{
		//Read the value of the source bit from the image
		png_byte sourceBitValue = png->pngData[*rowIndex][*columnIndex] & sourceBit;

		//Move the bit across to the same bit position as destination bit
		sourceBitValue <<= i;

		resultByte |= sourceBitValue;

		//Move across one column index to the next channel
		*columnIndex += 1;

		//Will our next iteration though the loop cause us too go over to the next row?
		if( *columnIndex > png->imageWidth - 1)
		{
			*columnIndex = 0;
			*rowIndex += 1;
		}

	}

	return resultByte;


}



//----- PUBLIC IMPLEMENTATION -----

bool png_util_read_png_file(char* filename, PNG* pngDataStruct)
{

	//rb = read, binary
	FILE* filePtr = fopen(filename, "rb");

	if(filePtr == NULL)
	{
		printf("Cannot open PNG File\n");
		return false;
	}

	//Check if this file is actually a PNG file
	if(!file_is_png(filePtr))
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
	png_init_io(pngDataStruct->pngStruct, filePtr);

    //Now call png_read_info to receive the file info from the header file?
	//I.E. meta data from the front of the file, you can also read data from the end of the file somehow.
    png_read_info(pngDataStruct->pngStruct, pngDataStruct->pngInfo);

    //Get a bunch of data about the image
    pngDataStruct->imageWidth = png_get_image_width(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->imageHeight = png_get_image_height(pngDataStruct->pngStruct, pngDataStruct->pngInfo);

    pngDataStruct->bitDepth = png_get_bit_depth(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->channelNum = png_get_channels(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->colorType = png_get_color_type(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->interlaceType = png_get_interlace_type(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->compressionType = png_get_compression_type(pngDataStruct->pngStruct, pngDataStruct->pngInfo);
    pngDataStruct->filterType = png_get_filter_type(pngDataStruct->pngStruct, pngDataStruct->pngInfo);

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

    fclose(filePtr);

    //Everything has gone successfully, return true
	return true;
}


//Open and write a PNG file
bool png_util_write_png_file(char* filename, PNG* png)
{
	//Write binary
	FILE* filePtr = fopen(filename, "wb");

	if(filePtr == NULL)
	{
		printf("Cannot create/write a PNG file at this location\n");
		return false;
	}

	//Create a write struct
    png_structp write_struct_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!write_struct_ptr)
    {
    	printf("Unable to create a PNG write structure\n");
    	return false;
    }

    //Create a info struct for the write struct
    png_infop write_info_ptr = png_create_info_struct(write_struct_ptr);

    if (!write_info_ptr)
    {
       png_destroy_write_struct(&write_struct_ptr, NULL);
       printf("Unable to create a PNG info structure\n");
       return false;
    }

	png_init_io(write_struct_ptr, filePtr);

	//Set up the data so the image can be written
	png_set_IHDR(write_struct_ptr, write_info_ptr, png->imageWidth, png->imageHeight, png->bitDepth, png->colorType, png->interlaceType,
			png->compressionType, png->filterType);

	//Use the high level interface to write the data to a file

	//Get the rows from the input image that already exists in memory and set it to the rows for the write structs
	png_set_rows(write_struct_ptr, write_info_ptr, png->pngData);

	//Write the PNG image out to the file
	png_write_png(write_struct_ptr, write_info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/*
	//Write the actual image data
	png_write_image(write_struct_ptr, png->pngData);
	png_write_end(write_struct_ptr, NULL);
	*/

	//Free the allocated structs
	png_destroy_write_struct(&write_struct_ptr, &write_info_ptr);
	fclose(filePtr);

	return true;
}



uint32_t png_util_total_message_byte_storage(PNG* png)
{
	//Number of bytes = (channels per pixel * pixelWidth * pixelHeight) / 8, minus 4 because the first four bytes is the message length
	return ((png->channelNum * png->imageWidth * png->imageHeight) / 8) - 4;
}



void png_util_write_message(PNG* png, char* message, uint32_t messageCharacterLength)
{
	uint32_t storageLimit = png_util_total_message_byte_storage(png);

	//Is the message to big to store in this image?
	if(messageCharacterLength > (storageLimit) )
	{
		printf("The message is too big to store in this image!\n");
		return;
	}

	uint32_t rowIndex = 0;
	uint32_t columnIndex = 0;

	//This is used to write the messageCharactersLength, one byte at a time
	png_byte* messageLengthBytePointer = (png_byte*)&messageCharacterLength;


	//Write the messages character length to the image
	for(int i = 0; i < 4; ++i)
	{
		//Write the byte to the image to each LSB across the channels
		write_byte(png, &rowIndex, &columnIndex, *messageLengthBytePointer);

		//Move across by one byte
		messageLengthBytePointer += 1;
	}

	//Write the actual message itself
	for(int i = 0; i < messageCharacterLength; ++i)
	{
		write_byte(png, &rowIndex, &columnIndex, (png_byte) *(message + i) );
	}
}



//Read a message from a PNG image
char* png_util_read_message(PNG* png)
{
	char* message = NULL;

	uint32_t rowIndex = 0;
	uint32_t columnIndex = 0;

	png_byte messageLengthBytes[4];

	for(int i = 0; i < 4; ++i)
	{
		messageLengthBytes[i] = read_byte(png, &rowIndex, &columnIndex);
	}

	uint32_t* messageLength = (uint32_t*)messageLengthBytes;

	printf("Message length: %u\n", *messageLength);

	//+1 for the null terminator
	message = malloc(sizeof(char) * (*messageLength + 1));


	for(int i = 0; i < *messageLength; ++i)
	{
		message[i] = read_byte(png, &rowIndex, &columnIndex);
	}

	//Set the null terminator as the last character
	message[*messageLength] = '\0';

	return message;

}


void png_util_free_PNG(PNG* png)
{
	//Free the actual memory used to store each channel data
	for(int i = 0; i < png->imageHeight; ++i)
	{
		png_free(png->pngStruct, png->pngData[i]);
		png->pngData[i] = NULL;
	}

	//Free the pointers used to store the images row addresses
	free(png->pngData);
	png->pngData = NULL;

	png_destroy_read_struct( &(png->pngStruct), &(png->pngInfo), NULL);
	png->pngStruct = NULL;
	png->pngInfo = NULL;

	free(png);
	png = NULL;
}
