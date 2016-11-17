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


//Delays of type of operation (cycles)
#define MULT_DELAY 2
#define DIV_MOD_DELAY 3
#define LOGIC_DELAY 1
#define ADD_SUB_DELAY 1

class Node {
private:
	std::string operation;
	int nodeNum;
	std::vector<DataType*> _componentInputs;
	std::vector<DataType*> _componentOutputs;
	std::vector<Node*> previousNodes;
	std::vector<Node*> nextNodes;
	int delay;
	char visited;


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
	int getDelay();
	void setDelay(int d);
	void addInput(DataType* newInput);
	void addOutput(DataType* newOutput);
	void addPreviousNode(Node* previousNode);
	void addNextNode(Node* nextNode);
	void assignDelay();

};

#endif // NODE_H