/*
 * CommandLineParse.h
 *
 *  Created on: 12 Oct 2015
 *      Author: vincentvillani
 */

#ifndef COMMANDLINEPARSE_H_
#define COMMANDLINEPARSE_H_

#include <stdbool.h>


typedef struct CLA
{
	char* inputFileName;
	char* outputFileName;
	char* message;

	bool encoding; //True if encoding, false if decoding

} CLA;


CLA* setup(int argc, char* argv[]);


#endif /* COMMANDLINEPARSE_H_ */
