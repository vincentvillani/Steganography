/*
 * CommandLineParse.c
 *
 *  Created on: 12 Oct 2015
 *      Author: vincentvillani
 */

#include "CommandLineParse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PNG_Util.h"

static void displayHelp();
static void displayHelpError(); //Just uses stderr, kinda dumbbbbb


static void displayHelp()
{
	printf("\n--- sunflower ---\n");
	printf("Version: 1.0\n");
	printf("\nExample Usage:\n");
	printf("Encoding: sunflower e inputFilename.png -o outputFilename.png -m \"My super secret message\"\n");
	printf("Decoding: sunflower d inputFilename.png\n\n");
}


static void displayHelpError()
{
	fprintf(stderr, "\nExample Usage:\n");
	fprintf(stderr, "Encoding: sunflower e inputFilename.png -o outputFilename.png -m \"My super secret message\"\n");
	fprintf(stderr, "Decoding: sunflower d inputFilename.png\n\n");
}

CLA* setup(int argc, char* argv[])
{
	CLA* result = NULL;

	if(argc == 1)
	{
		displayHelp();
		exit(0);
	}

	//Do we have the needed arguments?
	if(argc < 3)
	{
		fprintf(stderr, "\nError you are not passing the required arguments!\nPlease see the examples below on how to use sunflower.\n");
		displayHelpError();
		exit(1);
	}

	result = (CLA*)malloc(sizeof(CLA));
	result->inputFileName = NULL;
	result->outputFileName = NULL;
	result->message = NULL;

	//Are we encoding or decoding?
	if(strcmp(argv[1], "e") == 0 || strcmp(argv[1], "E") == 0)
	{
		printf("We are encoding!\n");
		result->encoding = true;
	}
	else if(strcmp(argv[1], "d") == 0 || strcmp(argv[1], "D") == 0)
	{
		printf("We are decoding!\n");
		result->encoding = false;
	}
	else
	{
		fprintf(stderr, "The first argument should be e (encoding) or d (decoding)!\n");
		exit(1);
	}


	//What is the input filename
	result->inputFileName = argv[2];
	printf("Input filename: %s\n", result->inputFileName);


	if(result->encoding)
	{
		bool foundOutputFilename = false;
		bool foundMessage = false;

		//Look for the -m message flag
		for(int i = 2; i < argc; ++i)
		{
			//We found the message flag
			if(strcmp(argv[i], "-m") == 0)
			{
				//We found the message itself
				if(i + 1 < argc)
				{
					result->message = argv[i + 1];
					printf("Message: %s\n", result->message);
					foundMessage = true;
					break;
				}
				else
				{
					fprintf(stderr, "If using the -m switch please specify a message\n");
					exit(1);
				}
			}
		}


		//look for -o (output filename)
		for(int i = 2; i < argc; ++i)
		{
			//We found the output filename switch
			if(strcmp(argv[i], "-o") == 0)
			{
				//We found the output filename
				if(i + 1 < argc)
				{
					result->outputFileName = argv[i + 1];
					printf("Output filename: %s\n", result->outputFileName);
					foundOutputFilename = true;
					break;
				}
				else
				{
					fprintf(stderr, "If using the -o argument please specify an output filename\n");
					exit(1);
				}
			}
		}


		//Error messages
		if(!foundMessage || !foundOutputFilename)
		{
			if(!foundMessage)
			{
				fprintf(stderr, "If encoding please include a message: -f \"Message\"\n");
			}

			if(!foundOutputFilename)
			{
				fprintf(stderr, "If encoding please include an output filename: -o filename.png\n");
			}

			exit(1);
		}

	}

	/*
	else //We are decoding
	{

	}*/


	return result;
}




void run(CLA* CLA)
{
	PNG* png = malloc(sizeof(PNG));

	//Encode the message into the input file and write out the new image to the output file
	if(CLA->encoding)
	{
		png_util_read_png_file(CLA->inputFileName, png);

		uint32_t messageLength = strlen(CLA->message);
		png_util_write_message(png, CLA->message, messageLength);

		png_util_write_png_file(CLA->outputFileName, png);

	}
	else //Read in the input image and decode the message
	{
		png_util_read_png_file(CLA->inputFileName, png);

		CLA->message = png_util_read_message(png);

		printf("Decoded message: %s\n", CLA->message);

		//Free the memory of the decoded memory
		free(CLA->message);
	}

	//Free the memory from the PNG struct
	png_util_free_PNG(png);

}
