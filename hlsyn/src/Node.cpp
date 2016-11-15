/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 11/15/2016
Assignment: 3
File: Node.cpp
Description: Node Class for hlsyn program
*/

#include "Node.h"

Node::Node()
{
	nodeNum = 0;
	visited = 'w';
}

Node::Node(std::string n, int num, std::vector<DataType*> _inputs, std::vector<DataType*> _outputs)
{
	operation = n;
	nodeNum = num;
	_componentInputs = _inputs;
	_componentOutputs = _outputs;
	visited = 'w';

}

std::string Node::getOperation()
{
	return operation;
}

void Node::setOperation(std::string o)
{
	operation = o;
}

int Node::getNodeNum()
{
	return nodeNum;
}

void Node::setNodeNum(int num)
{
	nodeNum = num;
}

std::vector<DataType*> Node::getInputs()
{
	return _componentInputs;
}

void Node::setInputs(std::vector<DataType*> _inputs)
{
	_componentInputs = _inputs;
}

std::vector<DataType*> Node::getOutputs()
{
	return _componentOutputs;
}

void Node::setOutputs(std::vector<DataType*> _outputs)
{
	_componentOutputs = _outputs;
}

std::vector<Node*> Node::getPreviousNodes()
{
	return previousNodes;
}

void Node::setPreviousNodes(std::vector<Node*> pN)
{
	previousNodes = pN;
}

std::vector<Node*> Node::getNextNodes()
{
	return nextNodes;
}

void Node::setNextNodes(std::vector<Node*> nN)
{
	nextNodes = nN;
}

char Node::getVisted()
{
	return visited;
}

void Node::setVisted(char v)
{
	visited = v;
}

void Node::addInput(DataType* newInput)
{
	_componentInputs.push_back(newInput);
}

void Node::addOutput(DataType* newOutput)
{
	_componentOutputs.push_back(newOutput);
}

void Node::addPreviousNode(Node * previousNode)
{
	previousNodes.push_back(previousNode);
}


