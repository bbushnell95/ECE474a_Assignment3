/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 10/3/2016
Assignment: 2
File: Circuit.cpp
Description: Circuit Class definitions for dpgen program
*/

#include "HLSM.h"
using namespace std;





bool HLSM::readFile(char* fileName)
{
	ifstream inputFile;
	string checkString;
	string checkChar;
	bool foundDataType;
	const std::string validDataTypes[12] = { "Int1", "Int2", "Int8", "Int16", "Int32", "Int64",
		"UInt1", "UInt2", "UInt8", "UInt16", "UInt32","UInt64" };
	int i = 0;
	int j = 0;
	int componentOutputIndex = -1;
	int componentInputIndex = -1;
	int componentWireIndex = -1;

	inputFile.open(fileName);

	//checking if open
	if (!inputFile.is_open()) {
		// cout << "Could not open " << fileName << endl;
		return false;
	}

	//while there is something on the line
	while (!inputFile.eof()) {
		inputFile >> checkString;

		//first check if input or output or wire
		if (!checkString.compare("")) {
			break;
		}
		if (!checkString.compare("input")) {
			//getline(inputFile, checkString);
			inputFile >> checkString;

			foundDataType = false;

			for (i = 0; i < 12; ++i) {
				//go through valid data types and see which one it is
				if (!checkString.compare(validDataTypes[i])) {
					foundDataType = true;
					break;
				}
			}

			if (foundDataType) {
				getline(inputFile, checkString);
				createNewInputVariable(checkString, i);
			}
			else {
				cout << "Error: Invalid Data type: " << checkString << " Exiting Program." << endl;
				inputFile.close();
				return false;
			}
		}
		else if (!checkString.compare("output")) {
			inputFile >> checkString;

			foundDataType = false;

			for (i = 0; i < 12; ++i) {
				//go through valid data types and see which one it is
				if (!checkString.compare(validDataTypes[i])) {
					foundDataType = true;
					break;
				}
			}

			if (foundDataType) {
				getline(inputFile, checkString);
				createNewOutputVariable(checkString, i);
			}
			else {
				cout << "Error: Invalid Data type: " << checkString << " Exiting Program." << endl;
				inputFile.close();
				return false;
			}
		}
		else if (!checkString.compare("wire") || !checkString.compare("register")) {
			inputFile >> checkString;

			foundDataType = false;

			for (i = 0; i < 12; ++i) {
				//go through valid data types and see which one it is
				if (!checkString.compare(validDataTypes[i])) {
					foundDataType = true;
					break;
				}
			}

			if (foundDataType) {
				getline(inputFile, checkString);
				createNewWireVariable(checkString, i);
			}
			else {
				cout << "Error: Invalid Data type: " << checkString << " Exiting Program." << endl;
				inputFile.close();
				return false;
			}
		}
		else {
			/* To always have a fresh set of eyes... */
			componentOutputIndex = -1;
			componentInputIndex = -1;
			componentWireIndex = -1;
			/* Check where the inputs/outputs/wires are for this datapath component. */
			if (!checkVariable(checkString, &componentOutputIndex, &componentInputIndex, &componentWireIndex)) {
				cout << "Variable '" << checkString << "' not found, please correct Netlist Behavior File." << endl;
				inputFile.close();
				return false;
			}
			else {
				/* Create some datapath components. */
				getline(inputFile, checkString);
				if (componentOutputIndex != -1) {
					if (!determineComponent(checkString, _outputs.at(componentOutputIndex))) {
						return false;
					}
				}
				else if (componentWireIndex != -1) {
					if (!determineComponent(checkString, _wires.at(componentWireIndex))) {
						return false;
					}
				}
			}

		}
		checkString = "";
	}
	for (i = 0; i < (int)_datapathComponents.size(); ++i) {
		_datapathComponents.at(i).determineDataWidth();
		_datapathComponents.at(i).assignDelay();
		_datapathComponents.at(i).checkIfSigned();
		//set going to for inputs
		for (j = 0; j < (int)_datapathComponents.at(i).getInputs().size(); ++j) {
			(*_datapathComponents.at(i).getInputs().at(j)).addToGoingTo(&_datapathComponents.at(i));
		}
		//sets coming from for outputs
		for (j = 0; j < (int)_datapathComponents.at(i).getOutputs().size(); ++j) {
			(*_datapathComponents.at(i).getOutputs().at(j)).addToComingFrom(&_datapathComponents.at(i));
		}
	}

	inputFile.close();
	return true;

}

bool HLSM::writeToFile(char* fileName)
{
	/* Set up variables. */
	ofstream outputFile;
	time_t timeNow = time(0);
	string tempFileName = "";
	string moduleName = "";
	int i = 0;
	int j = 0;
	int k = 0;

	/* Staging file name. */
	tempFileName = fileName;
	for (i = tempFileName.size() - 1; i > 0; i--) {
		if (tempFileName.at(i) == '\\' || tempFileName.at(i) == '/') {
			break;
		}
	}
	for (i = i + 1; i < (int)tempFileName.size() - 1; i++) {
		if (tempFileName.at(i) == '.') {
			break;
		}
		moduleName += tempFileName.at(i);
	}

	/* Open the output file. */
	outputFile.open(fileName);
	if (!outputFile.is_open()) {
		return false;
	}

	/* Check for empty netlist behavior files. */
	if (_inputs.empty()) {
		outputFile.close();
		return false;
	}
	if (_outputs.empty()) {
		outputFile.close();
		return false;
	}
	/* Note: There can be no wires if there is
	a netlist behavior file containing no
	inter-connected datapath components. */

	/* Print Setup. */
	outputFile << "`timescale 1ns / 1ps" << endl;
	outputFile << "//////////////////////////////////////////////////////////////////////////////////" << endl;
	outputFile << "//" << endl;
	outputFile << "//Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)" << endl;
	outputFile << "//Date Created: " << asctime(localtime(&timeNow));
	outputFile << "//Assignment: " << "2" << endl;
	outputFile << "//File: " << moduleName << ".v" << endl;
	outputFile << "//Description: A netlist behavior circuit implementation" << endl;
	outputFile << "//" << endl;
	outputFile << "//////////////////////////////////////////////////////////////////////////////////" << endl;
	outputFile << endl;

	/* Start of Module. */
	outputFile << "module " << moduleName << "(clk, rst, ";
	for (i = 0; i < (int)_inputs.size(); i++) {
		outputFile << (*_inputs.at(i)).getName();
		outputFile << ", ";
	}
	for (i = 0; i < (int)_outputs.size(); i++) {
		outputFile << (*_outputs.at(i)).getName();
		if (i != (int)_outputs.size() - 1) {
			outputFile << ", ";
		}
	}
	outputFile << ");" << endl;

	/* Print clk, rst. */
	outputFile << "\t" << "input clk, rst;" << endl;

	/* Print Inputs, Outputs, Wires. */
	writeVarsToFile(&outputFile);

	/* WRITE THE DATAPATH COMPONENTS. */
	k = 0;
	for (i = 0; i < (int)_datapathComponents.size(); i++) {
		outputFile << "\t";
		/* Which module is it? */
		if ((!_datapathComponents.at(i).getName().compare("COMP_lt"))
			|| (!_datapathComponents.at(i).getName().compare("COMP_gt"))
			|| (!_datapathComponents.at(i).getName().compare("COMP_eq"))) {
			outputFile << "COMP";
		}
		else if ((!_datapathComponents.at(i).getName().compare("SCOMP_lt"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_gt"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_eq"))) {
			outputFile << "SCOMP";
		}
		else {
			outputFile << _datapathComponents.at(i).getName();
		}

		/* What is the Datawidth? */
		outputFile << " #(";
		outputFile << _datapathComponents.at(i).getDataWidth();
		outputFile << ") ";

		/* Name the module. */
		if ((!_datapathComponents.at(i).getName().compare("COMP_lt"))
			|| (!_datapathComponents.at(i).getName().compare("COMP_gt"))
			|| (!_datapathComponents.at(i).getName().compare("COMP_eq"))) {
			outputFile << "COMP";
		}
		else if ((!_datapathComponents.at(i).getName().compare("SCOMP_lt"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_gt"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_eq"))) {
			outputFile << "SCOMP";
		}
		else {
			outputFile << _datapathComponents.at(i).getName();
		}

		/* REG */
		outputFile << "_" << i << "(";
		if ((!_datapathComponents.at(i).getName().compare("REG"))
			|| (!_datapathComponents.at(i).getName().compare("SREG"))) {
			outputFile << (*_datapathComponents.at(i).getInputs().at(0)).getName() << ", ";
			outputFile << "clk, rst, ";
			outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName();
		}
		/* ADD, SUB, MUL, DIV, MOD, SHL, SHR, INC, DEC */
		else if ((!_datapathComponents.at(i).getName().compare("ADD"))
			|| (!_datapathComponents.at(i).getName().compare("SADD"))
			|| (!_datapathComponents.at(i).getName().compare("SUB"))
			|| (!_datapathComponents.at(i).getName().compare("SSUB"))
			|| (!_datapathComponents.at(i).getName().compare("MUL"))
			|| (!_datapathComponents.at(i).getName().compare("SMUL"))
			|| (!_datapathComponents.at(i).getName().compare("DIV"))
			|| (!_datapathComponents.at(i).getName().compare("SDIV"))
			|| (!_datapathComponents.at(i).getName().compare("MOD"))
			|| (!_datapathComponents.at(i).getName().compare("SMOD"))
			|| (!_datapathComponents.at(i).getName().compare("SHR"))
			|| (!_datapathComponents.at(i).getName().compare("SSHR"))
			|| (!_datapathComponents.at(i).getName().compare("SHL"))
			|| (!_datapathComponents.at(i).getName().compare("SSHL"))
			|| (!_datapathComponents.at(i).getName().compare("INC"))
			|| (!_datapathComponents.at(i).getName().compare("SINC"))
			|| (!_datapathComponents.at(i).getName().compare("DEC"))
			|| (!_datapathComponents.at(i).getName().compare("SDEC"))) {
			for (j = 0; j < (int)_datapathComponents.at(i).getInputs().size(); j++) {
				writeInputsToFile(&outputFile, i, j);
			}

			outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName();
		}
		/* MUX2x1 */
		else if ((!_datapathComponents.at(i).getName().compare("MUX2x1"))
			|| (!_datapathComponents.at(i).getName().compare("SMUX2x1"))) {
			// MUX2x1(a, b, sel, d);
			for (j = 1; j <= 2; j++) {
				writeInputsToFile(&outputFile, i, j);
			}
			/* Select bit. */
			if ((*_datapathComponents.at(i).getInputs().at(0)).getDataWidth() != 1) {
				outputFile << (*_datapathComponents.at(i).getInputs().at(0)).getName() << "[0], ";
			}
			else {
				outputFile << (*_datapathComponents.at(i).getInputs().at(0)).getName() << ", ";
			}
			outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName();
		}
		/* COMP_gt */
		else if ((!_datapathComponents.at(i).getName().compare("COMP_gt"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_gt"))) {
			for (j = 0; j < (int)_datapathComponents.at(i).getInputs().size(); j++) {
				writeInputsToFile(&outputFile, i, j);
			}
			if ((*_datapathComponents.at(i).getOutputs().at(0)).getDataWidth() != 1) {
				outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName() << "[0], ";
			}
			else {
				outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName() << ", ";
			}
			/* Old way:
			outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName() << ", ";
			*/
			outputFile << "na" << k << ", ";
			k++;
			outputFile << "na" << k;
			k++;
		}
		/* COMP_lt */
		else if ((!_datapathComponents.at(i).getName().compare("COMP_lt"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_lt"))) {
			for (j = 0; j < (int)_datapathComponents.at(i).getInputs().size(); j++) {
				writeInputsToFile(&outputFile, i, j);
			}
			outputFile << "na" << k << ", ";
			k++;
			if ((*_datapathComponents.at(i).getOutputs().at(0)).getDataWidth() != 1) {
				outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName() << "[0], ";
			}
			else {
				outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName() << ", ";
			}
			outputFile << "na" << k;
			k++;
		}
		/* COMP_eq */
		else if ((!_datapathComponents.at(i).getName().compare("COMP_eq"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_eq"))) {
			for (j = 0; j < (int)_datapathComponents.at(i).getInputs().size(); j++) {
				writeInputsToFile(&outputFile, i, j);
			}
			outputFile << "na" << k << ", ";
			k++;
			outputFile << "na" << k << ", ";
			k++;
			if ((*_datapathComponents.at(i).getOutputs().at(0)).getDataWidth() != 1) {
				outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName() << "[0]";
			}
			else {
				outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName();
			}
			/* Old way:
			outputFile << (*_datapathComponents.at(i).getOutputs().at(0)).getName();
			*/
		}
		outputFile << ");" << endl;
	}

	/* End Module. */
	outputFile << endl << "endmodule" << endl;

	/* Close the output file. */
	outputFile.close();

	return true;

}



void HLSM::createNewInputVariable(std::string checkString, int dataWidthIndex)
{
	string checkChar = "";
	string variableName = "";
	int i = 0;

	for (i = checkString.size() - 1; i >= 0; --i) {
		if (checkString.at(i) == ' ' || checkString.at(i) == '\r') {
			checkString.erase(checkString.end() - 1);
		}
		else {
			break;
		}
	}

	std::istringstream iss(checkString);
	//checkString += '\n';

	while (!iss.eof()) {
		iss >> variableName;
		if (variableName.at(variableName.size() - 1) == ',') {
			variableName.erase(variableName.size() - 1);
		}

		if (checkIfComment(variableName)) {
			break;
		}

		switch (dataWidthIndex) {
		case 0: createNewInput(variableName, 1, DATAWIDTH_1);
			break;
		case 1: createNewInput(variableName, 1, DATAWIDTH_2);
			break;
		case 2: createNewInput(variableName, 1, DATAWIDTH_8);
			break;
		case 3: createNewInput(variableName, 1, DATAWIDTH_16);
			break;
		case 4: createNewInput(variableName, 1, DATAWIDTH_32);
			break;
		case 5: createNewInput(variableName, 1, DATAWIDTH_64);
			break;
		case 6: createNewInput(variableName, 0, DATAWIDTH_1);
			break;
		case 7: createNewInput(variableName, 0, DATAWIDTH_2);
			break;
		case 8: createNewInput(variableName, 0, DATAWIDTH_8);
			break;
		case 9: createNewInput(variableName, 0, DATAWIDTH_16);
			break;
		case 10: createNewInput(variableName, 0, DATAWIDTH_32);
			break;
		case 11: createNewInput(variableName, 0, DATAWIDTH_64);
			break;
		}

	}
}

void HLSM::createNewOutputVariable(std::string checkString, int dataWidthIndex)
{
	string checkChar = "";
	string variableName = "";
	int i = 0;

	for (i = checkString.size() - 1; i >= 0; --i) {
		if (checkString.at(i) == ' ' || checkString.at(i) == '\r') {
			checkString.erase(checkString.end() - 1);
		}
		else {
			break;
		}
	}


	std::istringstream iss(checkString);
	//checkString += '\n';

	while (!iss.eof()) {
		iss >> variableName;

		if (variableName.at(variableName.size() - 1) == ',') {
			variableName.erase(variableName.size() - 1);
		}

		if (checkIfComment(variableName)) {
			break;
		}

		switch (dataWidthIndex) {
		case 0: createNewOutput(variableName, 1, DATAWIDTH_1);
			break;
		case 1: createNewOutput(variableName, 1, DATAWIDTH_2);
			break;
		case 2: createNewOutput(variableName, 1, DATAWIDTH_8);
			break;
		case 3: createNewOutput(variableName, 1, DATAWIDTH_16);
			break;
		case 4: createNewOutput(variableName, 1, DATAWIDTH_32);
			break;
		case 5: createNewOutput(variableName, 1, DATAWIDTH_64);
			break;
		case 6: createNewOutput(variableName, 0, DATAWIDTH_1);
			break;
		case 7: createNewOutput(variableName, 0, DATAWIDTH_2);
			break;
		case 8: createNewOutput(variableName, 0, DATAWIDTH_8);
			break;
		case 9: createNewOutput(variableName, 0, DATAWIDTH_16);
			break;
		case 10: createNewOutput(variableName, 0, DATAWIDTH_32);
			break;
		case 11: createNewOutput(variableName, 0, DATAWIDTH_64);
			break;
		}
	}
}

void HLSM::createNewWireVariable(std::string checkString, int dataWidthIndex)
{
	string checkChar = "";
	string variableName = "";
	int i = 0;

	for (i = checkString.size() - 1; i >= 0; --i) {
		if (checkString.at(i) == ' ' || checkString.at(i) == '\r') {
			checkString.erase(checkString.end() - 1);
		}
		else {
			break;
		}
	}

	std::istringstream iss(checkString);

	//checkString += '\n';
	while (!iss.eof()) {
		iss >> variableName;

		if (variableName.at(variableName.size() - 1) == ',') {
			variableName.erase(variableName.size() - 1);
		}

		if (checkIfComment(variableName)) {
			break;
		}

		switch (dataWidthIndex) {
		case 0: createNewWire(variableName, 1, DATAWIDTH_1);
			break;
		case 1: createNewWire(variableName, 1, DATAWIDTH_2);
			break;
		case 2: createNewWire(variableName, 1, DATAWIDTH_8);
			break;
		case 3: createNewWire(variableName, 1, DATAWIDTH_16);
			break;
		case 4: createNewWire(variableName, 1, DATAWIDTH_32);
			break;
		case 5: createNewWire(variableName, 1, DATAWIDTH_64);
			break;
		case 6: createNewWire(variableName, 0, DATAWIDTH_1);
			break;
		case 7: createNewWire(variableName, 0, DATAWIDTH_2);
			break;
		case 8: createNewWire(variableName, 0, DATAWIDTH_8);
			break;
		case 9: createNewWire(variableName, 0, DATAWIDTH_16);
			break;
		case 10: createNewWire(variableName, 0, DATAWIDTH_32);
			break;
		case 11: createNewWire(variableName, 0, DATAWIDTH_64);
			break;
		}
	}
}


void HLSM::createNewInput(std::string name, bool sign, int dataWidth)
{
	Input* newInput = new Input(name, sign, dataWidth);

	_inputs.push_back(newInput);
}

void HLSM::createNewOutput(std::string name, bool sign, int dataWidth)
{
	Output* newOutput = new Output(name, sign, dataWidth);

	_outputs.push_back(newOutput);
}

void HLSM::createNewWire(std::string name, bool sign, int dataWidth)
{
	Wire* newWire = new Wire(name, sign, dataWidth);

	_wires.push_back(newWire);
}


bool HLSM::checkVariable(std::string checkName, int* outputIndex, int* inputIndex, int* wireIndex)
{
	bool variableFound = false;
	int i = 0;

	/*check if in inputs*/
	for (i = 0; i < (int)_inputs.size(); ++i) {
		if (!(*_inputs.at(i)).getName().compare(checkName)) {
			variableFound = true;
			*inputIndex = i;
			break;
		}
	}

	/*check if in wires*/
	for (i = 0; i < (int)_wires.size(); ++i) {
		if (!(*_wires.at(i)).getName().compare(checkName)) {
			variableFound = true;
			*wireIndex = i;
			break;
		}
	}

	/*check if in outputs*/
	for (i = 0; i < (int)_outputs.size(); ++i) {
		if (!(*_outputs.at(i)).getName().compare(checkName)) {
			variableFound = true;
			*outputIndex = i;
			break;
		}
	}
	return variableFound;
}
//bool HLSM::determineComponent(std::string line, DataType* output)
//{
//	int i = 0;
//	int equalCount = 0;
//	int inputIndex = -1;
//	int wireIndex = -1;
//	int outputIndex = -1;
//	bool result = true;
//	bool isDecInc = false;
//	std::string checkString = "";
//	std::string componentType = "";
//	std::string tempVariableName = "";
//	std::vector<DataType*> componentInputs;
//	std::vector<DataType*> componentOutputs;
//
//	for (i = line.size() - 1; i >= 0; --i) {
//		if (line.at(i) == ' ' || line.at(i) == '\r') {
//			line.erase(line.end() - 1);
//		}
//		else {
//			break;
//		}
//	}
//
//	std::istringstream iss(line);
//
//	//line += '\n';
//	componentOutputs.push_back(output);
//
//
//	while (!iss.eof()) {
//		iss >> checkString;
//
//		if (!checkString.compare("=")) {
//			++equalCount;
//		}
//
//		if (checkIfComment(checkString)) {
//			break;
//		}
//
//		else if (!checkString.compare("1")) {
//			if (!componentType.compare("ADD")) {
//				componentType = "INC";
//				isDecInc = true;
//			}
//			if (!componentType.compare("SUB")) {
//				componentType = "DEC";
//				isDecInc = true;
//			}
//		}
//		else {
//			if (!checkValidSymbol(checkString, &componentType)) {
//				if (checkVariable(checkString, &outputIndex, &inputIndex, &wireIndex)) {
//					if (inputIndex != -1) {
//						componentInputs.push_back(_inputs.at(inputIndex));
//
//					}
//					else if (wireIndex != -1) {
//						componentInputs.push_back(_wires.at(wireIndex));
//					}
//				}
//				else {
//					cout << checkString << " is not a valid variable or symbol" << endl;
//					result = false;
//					break;
//				}
//			}
//
//		}
//	}
//	if (result) {
//		if (componentInputs.size() == 1 && !isDecInc) {
//			createNewDatapathComponent("REG", componentInputs, componentOutputs);
//		}
//		else {
//			createNewDatapathComponent(componentType, componentInputs, componentOutputs);
//		}
//	}
//	return result;
//
//}



bool HLSM::checkValidSymbol(std::string checkSymbol, std::string* dPType)
{
	const std::string validSymbols[13] = { "=","+" ,"-", "*", ">", "<","==", "?", ":", ">>", "<<", "/", "%" };
	int i = 0;
	bool foundValidSymbol = false;

	for (i = 0; i < 13; i++) {
		if (!checkSymbol.compare(validSymbols[i])) {
			foundValidSymbol = true;
			break;
		}
	}

	if (!foundValidSymbol) {
		return false;
	}

	switch (i) {
	case 0:
		// Do nothing.
		break;
	case 1: *dPType = "ADD";
		break;
	case 2: *dPType = "SUB";
		break;
	case 3: *dPType = "MUL";
		break;
	case 4: *dPType = "COMP_gt";
		break;
	case 5: *dPType = "COMP_lt";
		break;
	case 6: *dPType = "COMP_eq";
		break;
	case 7: *dPType = "MUX2x1";
		break;
	case 8: *dPType = "MUX2x1";
		break;
	case 9: *dPType = "SHR";
		break;
	case 10: *dPType = "SHL";
		break;
	case 11: *dPType = "DIV";
		break;
	case 12: *dPType = "MOD";
		break;
	}

	return true;
}

bool HLSM::writeInputsToFile(ofstream *outputFile, int i, int j)
{

	/* Make sure it is still open. */
	if (!(*outputFile).is_open()) {
		return false;
	}

	/* Bit-width Matching. */
	if ((*_datapathComponents.at(i).getInputs().at(j)).getDataWidth() == _datapathComponents.at(i).getDataWidth()) {
		(*outputFile) << (*_datapathComponents.at(i).getInputs().at(j)).getName() << ", ";
	}
	else if ((*_datapathComponents.at(i).getInputs().at(j)).getDataWidth() < _datapathComponents.at(i).getDataWidth()) { // Datapath width is larger than input.
		if ((*_datapathComponents.at(i).getInputs().at(j)).getSignUnsigned()) { // Signed
																				// Sign extensions for 2's complement. 
			(*outputFile) << "{{";
			(*outputFile) << _datapathComponents.at(i).getDataWidth() - (*_datapathComponents.at(i).getInputs().at(j)).getDataWidth();
			(*outputFile) << "{";
			(*outputFile) << (*_datapathComponents.at(i).getInputs().at(j)).getName();
			(*outputFile) << "[";
			(*outputFile) << (*_datapathComponents.at(i).getInputs().at(j)).getDataWidth() - 1;
			(*outputFile) << "]}},";
			(*outputFile) << (*_datapathComponents.at(i).getInputs().at(j)).getName();
			(*outputFile) << "}, ";

		}
		else { // Unsigned
			(*outputFile) << "{";
			(*outputFile) << _datapathComponents.at(i).getDataWidth() - (*_datapathComponents.at(i).getInputs().at(j)).getDataWidth();
			(*outputFile) << "'b0,";
			(*outputFile) << (*_datapathComponents.at(i).getInputs().at(j)).getName();
			(*outputFile) << "}, ";
		}
	}
	else { // Datapath width is smaller than input.

		   /* This way chops the last bits and uses those. */
		(*outputFile) << (*_datapathComponents.at(i).getInputs().at(j)).getName();
		(*outputFile) << "[";
		(*outputFile) << _datapathComponents.at(i).getDataWidth() - 1;
		(*outputFile) << ":0], ";
	}

	return true;
}

bool HLSM::writeVarsToFile(std::ofstream *outputFile)
{
	int i = 0;
	int j = 0;
	int k = 0;
	bool ind1 = false;
	bool sind1 = false;
	bool ind2 = false;
	bool sind2 = false;
	bool ind8 = false;
	bool sind8 = false;
	bool ind16 = false;
	bool sind16 = false;
	bool ind32 = false;
	bool sind32 = false;
	bool ind64 = false;
	bool sind64 = false;

	/* Make sure it is still open. */
	if (!(*outputFile).is_open()) {
		return false;
	}

	/* Print Inputs. */
	ind1 = false;
	sind1 = false;
	ind2 = false;
	sind2 = false;
	ind8 = false;
	sind8 = false;
	ind16 = false;
	sind16 = false;
	ind32 = false;
	sind32 = false;
	ind64 = false;
	sind64 = false;

	/* Categorize if there are certain datawidths. */
	for (i = 0; i < (int)_inputs.size(); i++) {
		if (!(*_inputs.at(i)).getSignUnsigned()) {
			if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_1) {
				ind1 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_2) {
				ind2 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_8) {
				ind8 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_16) {
				ind16 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_32) {
				ind32 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_64) {
				ind64 = true;
			}
		}
		else {
			if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_1) {
				sind1 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_2) {
				sind2 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_8) {
				sind8 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_16) {
				sind16 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_32) {
				sind32 = true;
			}
			else if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_64) {
				sind64 = true;
			}
		}
	}

	/* Check for each datawidth. */
	if (ind1) {
		k = 0;
		*outputFile << "\t" << "input ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if (!(*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_1) {
					if (k) {
						*outputFile << ", ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind1) {
		k = 0;
		*outputFile << "\t" << "input signed ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if ((*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_1) {
					if (k) {
						*outputFile << ", ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind2) {
		k = 0;
		*outputFile << "\t" << "input ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if (!(*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_2) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[1:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind2) {
		k = 0;
		*outputFile << "\t" << "input signed ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if ((*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_2) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[1:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind8) {
		k = 0;
		*outputFile << "\t" << "input ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if (!(*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_8) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[7:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind8) {
		k = 0;
		*outputFile << "\t" << "input signed ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if ((*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_8) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[7:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind16) {
		k = 0;
		*outputFile << "\t" << "input ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if (!(*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_16) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[15:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind16) {
		k = 0;
		*outputFile << "\t" << "input signed ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if ((*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_16) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[15:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind32) {
		k = 0;
		*outputFile << "\t" << "input ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if (!(*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_32) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[31:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind32) {
		k = 0;
		*outputFile << "\t" << "input signed ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if ((*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_32) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[31:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind64) {
		k = 0;
		*outputFile << "\t" << "input ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if (!(*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_64) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[63:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind64) {
		k = 0;
		*outputFile << "\t" << "input signed ";
		for (i = 0; i < (int)_inputs.size(); i++) {
			if ((*_inputs.at(i)).getSignUnsigned()) {
				if ((*_inputs.at(i)).getDataWidth() == DATAWIDTH_64) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[63:0] ";
					}
					*outputFile << (*_inputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	*outputFile << endl;

	/* Print Outputs. */
	ind1 = false;
	sind1 = false;
	ind2 = false;
	sind2 = false;
	ind8 = false;
	sind8 = false;
	ind16 = false;
	sind16 = false;
	ind32 = false;
	sind32 = false;
	ind64 = false;
	sind64 = false;

	/* Categorize if there are certain datawidths. */
	for (i = 0; i < (int)_outputs.size(); i++) {
		if (!(*_outputs.at(i)).getSignUnsigned()) {
			if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_1) {
				ind1 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_2) {
				ind2 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_8) {
				ind8 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_16) {
				ind16 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_32) {
				ind32 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_64) {
				ind64 = true;
			}
		}
		else {
			if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_1) {
				sind1 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_2) {
				sind2 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_8) {
				sind8 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_16) {
				sind16 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_32) {
				sind32 = true;
			}
			else if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_64) {
				sind64 = true;
			}
		}
	}

	/* Check for each datawidth. */
	if (ind1) {
		k = 0;
		*outputFile << "\t" << "output ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if (!(*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_1) {
					if (k) {
						*outputFile << ", ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind1) {
		k = 0;
		*outputFile << "\t" << "output signed ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if ((*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_1) {
					if (k) {
						*outputFile << ", ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind2) {
		k = 0;
		*outputFile << "\t" << "output ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if (!(*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_2) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[1:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind2) {
		k = 0;
		*outputFile << "\t" << "output signed ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if ((*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_2) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[1:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind8) {
		k = 0;
		*outputFile << "\t" << "output ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if (!(*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_8) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[7:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind8) {
		k = 0;
		*outputFile << "\t" << "output signed ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if ((*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_8) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[7:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind16) {
		k = 0;
		*outputFile << "\t" << "output ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if (!(*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_16) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[15:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind16) {
		k = 0;
		*outputFile << "\t" << "output signed ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if ((*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_16) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[15:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind32) {
		k = 0;
		*outputFile << "\t" << "output ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if (!(*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_32) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[31:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind32) {
		k = 0;
		*outputFile << "\t" << "output signed ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if ((*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_32) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[31:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind64) {
		k = 0;
		*outputFile << "\t" << "output ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if (!(*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_64) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[63:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind64) {
		k = 0;
		*outputFile << "\t" << "output signed ";
		for (i = 0; i < (int)_outputs.size(); i++) {
			if ((*_outputs.at(i)).getSignUnsigned()) {
				if ((*_outputs.at(i)).getDataWidth() == DATAWIDTH_64) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[63:0] ";
					}
					*outputFile << (*_outputs.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	*outputFile << endl;

	/* Print Wires. */
	ind1 = false;
	sind1 = false;
	ind2 = false;
	sind2 = false;
	ind8 = false;
	sind8 = false;
	ind16 = false;
	sind16 = false;
	ind32 = false;
	sind32 = false;
	ind64 = false;
	sind64 = false;

	/* Categorize if there are certain datawidths. */
	for (i = 0; i < (int)_wires.size(); i++) {
		if (!(*_wires.at(i)).getSignUnsigned()) {
			if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_1) {
				ind1 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_2) {
				ind2 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_8) {
				ind8 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_16) {
				ind16 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_32) {
				ind32 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_64) {
				ind64 = true;
			}
		}
		else {
			if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_1) {
				sind1 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_2) {
				sind2 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_8) {
				sind8 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_16) {
				sind16 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_32) {
				sind32 = true;
			}
			else if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_64) {
				sind64 = true;
			}
		}
	}

	/* Check for each datawidth. */
	if (ind1) {
		k = 0;
		*outputFile << "\t" << "wire ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if (!(*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_1) {
					if (k) {
						*outputFile << ", ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind1) {
		k = 0;
		*outputFile << "\t" << "wire signed ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if ((*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_1) {
					if (k) {
						*outputFile << ", ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind2) {
		k = 0;
		*outputFile << "\t" << "wire ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if (!(*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_2) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[1:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind2) {
		k = 0;
		*outputFile << "\t" << "wire signed ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if ((*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_2) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[1:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind8) {
		k = 0;
		*outputFile << "\t" << "wire ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if (!(*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_8) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[7:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind8) {
		k = 0;
		*outputFile << "\t" << "wire signed ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if ((*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_8) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[7:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind16) {
		k = 0;
		*outputFile << "\t" << "wire ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if (!(*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_16) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[15:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind16) {
		k = 0;
		*outputFile << "\t" << "wire signed ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if ((*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_16) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[15:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind32) {
		k = 0;
		*outputFile << "\t" << "wire ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if (!(*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_32) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[31:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind32) {
		k = 0;
		*outputFile << "\t" << "wire signed ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if ((*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_32) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[31:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (ind64) {
		k = 0;
		*outputFile << "\t" << "wire ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if (!(*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_64) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[63:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	if (sind64) {
		k = 0;
		*outputFile << "\t" << "wire signed ";
		for (i = 0; i < (int)_wires.size(); i++) {
			if ((*_wires.at(i)).getSignUnsigned()) {
				if ((*_wires.at(i)).getDataWidth() == DATAWIDTH_64) {
					if (k) {
						*outputFile << ", ";
					}
					else {
						*outputFile << "[63:0] ";
					}
					*outputFile << (*_wires.at(i)).getName();
					k = 1;
				}
			}
		}
		*outputFile << ";" << endl;
	}
	*outputFile << endl;

	/* Check for all N/A wires are necessary. */
	k = 0;
	j = 0;
	for (i = 0; i < (int)_datapathComponents.size(); i++) {
		if ((!_datapathComponents.at(i).getName().compare("COMP_lt"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_lt"))
			|| (!_datapathComponents.at(i).getName().compare("COMP_gt"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_gt"))
			|| (!_datapathComponents.at(i).getName().compare("COMP_eq"))
			|| (!_datapathComponents.at(i).getName().compare("SCOMP_eq"))) {
			j = 1;
			*outputFile << "\t" << "wire ";
			*outputFile << "na" << k << ", ";
			k++;
			*outputFile << "na" << k << ";" << endl;
			k++;
		}
	}
	/* Spacing */
	if (j == 1) {
		*outputFile << endl;
	}

	return true;

}

bool HLSM::checkIfComment(std::string checkString)
{
	bool result = false;

	if (checkString.size() >= 2) {
		if (checkString.at(0) == '/' && checkString.at(1) == '/') {
			result = true;
		}
	}

	return result;
}
