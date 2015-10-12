/*
 * CommandLineParse.h
 *
 *  Created on: 12 Oct 2015
 *      Author: vincentvillani
 */

#ifndef COMMANDLINEPARSE_H_
#define COMMANDLINEPARSE_H_

#include <stdbool.h>

//Struct to store the command line arguments for the current run
typedef struct CLA
{
	char* inputFileName;
	char* outputFileName;
	char* message;

	bool encoding; //True if encoding, false if decoding

} CLA;

//Figure out what the user wants to do this run through the program
CLA* setup(int argc, char* argv[]);

//Either decode or encode, depending on the values set in the CLA struct
void run(CLA* CLA);


void readMessageFile(CLA* CLA, char* fileName);


#endif /* COMMANDLINEPARSE_H_ */
