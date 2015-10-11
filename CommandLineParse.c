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
					printf("Output filename: %s", result->outputFileName);
					break;
				}
				else
				{
					fprintf(stderr, "If using the -o argument please specify an output filename\n");
					exit(1);
				}
			}
		}
	}
	else //We are decoding
	{

	}

	//TODO: FIX THIS!
	//exit(2);

	return result;
}
