/*
 * main.c
 *
 *  Created on: 30 Sep 2015
 *      Author: vincentvillani
 */

#include <png.h>
#include <string.h>

#include "PNG_Util.h"

//TODO: Add a command parser
//TODO: Remove parts of the code that still use 'messageBitsPerChannel' or whatever it is, only use 1 bit. At least for now
//TODO: Keep existing metadata intact when writing out a file

int main()
{
	char* inputFileName = "test.png";
	char* exportFileName = "new.png";

	PNG* testPNG = malloc(sizeof(PNG));

	//Open and read the PDF file
	if(png_util_read_png_file(exportFileName, testPNG))
	{
		printf("File opened!\n");
	}


	printf("Total available characters: %u\n", png_util_total_message_byte_storage(testPNG, 1));

	//char* message = "Hello my name is Vincent!!";
	//uint32_t messageLength = strlen(message);

	//png_util_write_message(testPNG, message, messageLength, 1);

	char* readMessage = png_util_read_message(testPNG);

	//png_util_write_png_file(exportFileName, testPNG);

	printf("%s\n", readMessage);

	free(readMessage);
	png_util_free_PNG(testPNG);
	free(testPNG);

	return 0;
}
