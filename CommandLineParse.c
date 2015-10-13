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
	printf("\nsunflower\n");
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
	result->messageOutputFilename = NULL;

	//Are we encoding or decoding?
	if(strcmp(argv[1], "e") == 0 || strcmp(argv[1], "E") == 0)
	{
		//printf("We are encoding!\n");
		result->encoding = true;
	}
	else if(strcmp(argv[1], "d") == 0 || strcmp(argv[1], "D") == 0)
	{
		//printf("We are decoding!\n");
		result->encoding = false;
	}
	else
	{
		fprintf(stderr, "The first argument should be e (encoding) or d (decoding)!\n");
		exit(1);
	}


	//What is the input filename
	result->inputFileName = argv[2];
	//printf("Input filename: %s\n", result->inputFileName);


	if(result->encoding)
	{
		bool foundOutputFilename = false;
		bool foundMessage = false;

		//Look for the -m message flag
		for(int i = 2; i < argc; ++i)
		{

			//We found the file message flag
			if(strcmp(argv[i], "-mf") == 0)
			{
				//We found the messages filename
				if(i + 1 < argc)
				{
					readMessageFile(result, argv[i + 1]);

					//printf("Message: %s\n", result->message);
					foundMessage = true;
					break;
				}
				else
				{
					fprintf(stderr, "If using the -mf switch please specify a filename containing the message.\n");
					exit(1);
				}
			}

			//We found the message flag
			else if(strcmp(argv[i], "-m") == 0)
			{
				//We found the message itself
				if(i + 1 < argc)
				{
					size_t messageLength = strlen(argv[i + 1]);
					result->message =  (char*)malloc(sizeof(char) * (messageLength + 1));
					memcpy(result->message, argv[i + 1], messageLength);
					result->message[messageLength] = '\0';

					//printf("Message: %s\n", result->message);
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
					//printf("Output filename: %s\n", result->outputFileName);
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
				fprintf(stderr, "If encoding please include a message: -m \"Message\" OR -mf \"messageFile.txt\"\n");
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
		//Look for the -mf message flag
		for(int i = 2; i < argc; ++i)
		{

			//We found the file message flag
			if(strcmp(argv[i], "-mf") == 0)
			{
				//We found the messages filename
				if(i + 1 < argc)
				{
					result->messageOutputFilename = argv[i + 1];

					//printf("Message output filename: %s\n", result->messageOutputFilename);
					break;
				}
				else
				{
					fprintf(stderr, "If using the -mf switch please specify a filename containing the message.\n");
					exit(1);
				}
			}
		}
	}


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

		if(CLA->messageOutputFilename != NULL)
		{
			writeMessageFile(CLA);
		}

		//printf("Decoded message: %s\n", CLA->message);
	}

	//Free the memory from the PNG struct
	png_util_free_PNG(png);

}



void readMessageFile(CLA* CLA, char* fileName)
{
	CLA->message = NULL;
	long int messageSize = 0;

	FILE* file = fopen(fileName, "rb");

	if(file == NULL)
	{
		fprintf(stderr, "Unable to open message file: %s\nAborting...", fileName);
		exit(1);
	}

	if(fseek(file, 0L, SEEK_END) != 0)
	{
		fprintf(stderr, "Cannot determine length of the message file: %s\nAborting...", fileName);
		exit(1);
	}

	messageSize = ftell(file);

	CLA->message = (char*)malloc(sizeof(char) * (messageSize + 1));

	//Back to the beginning of the file
	rewind(file);

	//read the file
	fread(CLA->message, messageSize, 1, file);

	//Set null character
	CLA->message[messageSize] = '\0';

	fclose(file);

}



void writeMessageFile(CLA* CLA)
{
	size_t messageSize = strlen(CLA->message) + 1;

	FILE* file = fopen(CLA->messageOutputFilename, "wb");

	if(file == NULL)
	{
		fprintf(stderr, "Unable to open message file: %s\nAborting...", CLA->messageOutputFilename);
		exit(1);
	}

	fwrite(CLA->message, messageSize, 1, file);

	fclose(file);

}
