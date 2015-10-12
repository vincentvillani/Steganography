/*
 * main.c
 *
 *  Created on: 30 Sep 2015
 *      Author: vincentvillani
 */

#include <png.h>
#include <string.h>

#include "PNG_Util.h"
#include "CommandLineParse.h"

//TODO: Read message from a file
//TODO: encrypt message before storing?
//TODO: Keep existing metadata intact when writing out a file
//TODO: CLA->message should always allocate its own memory, copy the message and then be freed when finshed

int main(int argc, char* argv[])
{
	//What do we want to in this run?
	CLA* commandLineArgs = setup(argc, argv);

	run(commandLineArgs);

	//Deallocate memory for CLA

	free(commandLineArgs->message);
	free(commandLineArgs);

	return 0;
}
