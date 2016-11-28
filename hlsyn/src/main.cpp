/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 11/14/2016
Assignment: 3
File: main.cpp
Description: Main function for hlysn program
*/

#include <stdlib.h>
#include "HLSM.h"

using namespace std;

/* Command-line Argument as follows:
dpgen netlistFile verilogFile
*/
int main(int argc, char *argv[])
{

	HLSM newHLSM;

	/* Check for the correct number of arguments */
	if (argc != 4) {
		cout << endl;
		cout << "Usage: " << argv[0] << " cFile latency verilogFile";
		cout << endl << endl;
		return EXIT_FAILURE;
	}


	/* Read in the netlist file */
	if (!newHLSM.readFile(argv[1])) {
		cout << endl;
		cout << "Could not read from the input file " << argv[1] << ".";
		cout << endl << endl;
		return EXIT_FAILURE;
	}

	newHLSM.scheduleGraph(atoi(argv[2]));
	/* TODO: THE REAL WORK WILL HAPPEN HERE. */

	/* Write to the verilog file */
	if (!newHLSM.writeToFile(argv[3])) {
		cout << endl;
		cout << "Could not write to the output file " << argv[3] << ".";
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}