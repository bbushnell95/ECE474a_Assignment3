/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 11/14/2016
Assignment: 3
File: HLSM.h
Description: HLSM Class for hlsyn program
*/

#ifndef HLSM_H
#define HLSM_H

#include <ctime>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cstring>
#include<algorithm>

#include "DataType.h"
#include "Input.h"
#include "Output.h"
#include "Wire.h"


#define DATAWIDTH_1 1
#define DATAWIDTH_2 2
#define DATAWIDTH_8 8 
#define DATAWIDTH_16 16 
#define DATAWIDTH_32 32
#define DATAWIDTH_64 64


class HLSM {

private:
	std::string outputCircuit;
	std::vector<Input*> _inputs;
	std::vector<Output*> _outputs;
	std::vector<Wire*> _wires;
	

public:
	/*Constructors*/
	HLSM();


	/*Methods*/
	bool readFile(char* fileName);
	bool writeToFile(char* fileName);
	//void determineCriticalPath();
	//void visitComponent(DatapathComponent * compoenent, double currTime, double* cP);
	void createNewInputVariable(std::string checkString, int dataWidthIndex);
	void createNewOutputVariable(std::string checkString, int dataWidthIndex);
	void createNewWireVariable(std::string checkString, int dataWidthIndex);
	// void createNewRegisterVariable(std::string checkString, int dataWidthIndex);
	void createNewInput(std::string name, bool sign, int dataWidth);
	void createNewOutput(std::string name, bool sign, int dataWidth);
	void createNewWire(std::string name, bool sign, int dataWidth);
	// void createNewRegister(std::string name, bool sign, int dataWidth);
	bool checkVariable(std::string checkName, int* outputIndex, int* inputIndex, int* wireIndex);
	//bool determineComponent(std::string line, DataType* output);
	//void createNewDatapathComponent(std::string name, std::vector<DataType*> _Cinputs, std::vector<DataType*> _Coutputs);
	bool checkValidSymbol(std::string checkSymbol, std::string* dPType);
	bool writeInputsToFile(std::ofstream *outputFile, int i, int j);
	bool writeVarsToFile(std::ofstream *outputFile);
	bool checkIfComment(std::string checkString);

};

#endif // HLSM_H