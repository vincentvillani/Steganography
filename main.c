/*
 * main.c
 *
 *  Created on: 30 Sep 2015
 *      Author: vincentvillani
 */

#include <png.h>
#include <string.h>

#include "PNG_Util.h"

int main()
{
	FILE* filePtr = NULL;
	char* fileName = "test.png";

	PNG_Util_Data_Struct* testPNG = malloc(sizeof(PNG_Util_Data_Struct));

	//Open and read the PDF file
	bool pngFileOpenedSuccessfully = png_util_read_png_file(&filePtr, fileName, testPNG);

	if(pngFileOpenedSuccessfully)
	{
		printf("File opened!\n");

	}

	png_byte r = testPNG->pngData[0][0];
	png_byte g = testPNG->pngData[0][1];
	png_byte b = testPNG->pngData[0][2];
	png_byte a = testPNG->pngData[0][3];



	//Print out the value of the first pixel
	printf("%u, %u, %u, %u\n", r, g, b, a);

	return 0;
}
