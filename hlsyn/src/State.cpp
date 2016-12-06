/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 12/5/2016
Assignment: 3
File: State.cpp
Description: State class definitions for hlysn program
*/

#include "State.h"

/*
void State::setStateNum(int n)
{
	stateNum = n;
}

int State::getStateNum()
{
	return stateNum;
}
*/

void State::addAssignedNode(Node* addNode)
{
	assignedNodes.push_back(addNode);
}

std::vector<Node*> State::getAssignedNodes()
{
	return assignedNodes;
}