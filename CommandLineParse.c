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

CLA* setup(int argc, char* argv[])
{
	CLA* result = NULL;

	//Do we have the needed arguments?
	//TODO: FIX THIS!
	if(argc < 3)
	{
		fprintf(stderr, "Please pass the required arguments\n");
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
	else //We are decoding
	{

	}

	//TODO: FIX THIS!
	//exit(2);

	return result;
}




void run(CLA* CLA)
{
	PNG* png = malloc(sizeof(PNG));

	if(CLA->encoding)
	{
		png_util_read_png_file(CLA->inputFileName, png);

		uint32_t messageLength = strlen(CLA->message);
		png_util_write_message(png, CLA->message, messageLength);

		png_util_write_png_file(CLA->outputFileName, png);

	}
	else
	{
		png_util_read_png_file(CLA->inputFileName, png);

		CLA->message = png_util_read_message(png);

		printf("Decoded message: %s\n", CLA->message);
	}

	//png_util_read_png_file(exportFileName, testPNG);

	png_util_free_PNG(png);

}
