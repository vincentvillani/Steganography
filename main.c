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


//TODO: make install (Bundle pnglib??)
//TODO: Keep existing metadata intact when writing out a file


int main(int argc, char* argv[])
{
	//What do we want to in this run?
	CLA* commandLineArgs = setup(argc, argv);

	run(commandLineArgs);

	//Deallocate memory for CLA
	free(commandLineArgs);

	return 0;
}
