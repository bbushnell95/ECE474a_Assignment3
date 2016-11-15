/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 11/15/2016
Assignment: 3
File: Node.h
Description: Node Class for hlsyn program
*/
#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstring>
#include<algorithm>
#include"DataType.h"

class Node {
private:
	std::string operation;
	int nodeNum;
	std::vector<DataType*> _componentInputs;
	std::vector<DataType*> _componentOutputs;
	std::vector<Node*> previousNodes;
	std::vector<Node*> nextNodes;
	char visited;

	///* Datapath Component inputs */


	///* Datapath Component outputs */
	//Wire outWire;
	//Output outWire;

	///* FOR INFORMATION ONLY
	//	std::string validSymbols[13] = { "=","+" ,"-", "*", ">", "<","==", "?", ":", ">>", "<<", "/", "%"};
	//*/
	///*
	//FUNCT___SYM___
	//REG		=
	//ADD		+
	//SUB		-
	//MUL		*
	//COMP	<
	//COMP	>
	//COMP	==
	//MUX2x1	? :
	//SHR		>>
	//SHL		<<
	//DIV		/
	//MOD		%
	//INC		+1
	//DEC		-1
	//*/
	//std::string component[12];

public:
	/*Constructors*/
	Node();
	Node(std::string n, int num, std::vector<DataType*> _inputs, std::vector<DataType*> _outputs);

	/*Methods*/
	std::string getOperation();
	void setOperation(std::string o);
	int getNodeNum();
	void setNodeNum(int num);
	std::vector<DataType*> getInputs();
	void setInputs(std::vector<DataType*> _inputs);
	std::vector<DataType*> getOutputs();
	void setOutputs(std::vector<DataType*> _outputs);
	std::vector<Node*> getPreviousNodes();
	void setPreviousNodes(std::vector<Node*> pN);
	std::vector<Node*> getNextNodes();
	void setNextNodes(std::vector<Node*> nN);
	char getVisted();
	void setVisted(char v);
	void addInput(DataType* newInput);
	void addOutput(DataType* newOutput);
	void addPreviousNode(Node* previousNode);
	void addNextNode(Node* nextNode);

};

#endif // NODE_H