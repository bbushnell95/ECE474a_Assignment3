/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 12/1/2016
Assignment: 3
File: Conditional.cpp
Description: Conditional Class for hlsyn program
*/

#include "Conditional.h"

Conditional::Conditional()
{
}

Node * Conditional::getBasedOn()
{
	return basedOn;
}

void Conditional::setBasedOn(Node * bO)
{
	basedOn = bO;
}

std::vector<Node*> Conditional::getIfTrue()
{
	return _ifTrue;
}

void Conditional::setIfTrue(std::vector<Node*> iT)
{
	_ifTrue = iT;
}

std::vector<Node*> Conditional::getIfFalse()
{
	return _ifFalse;
}

void Conditional::setIfFalse(std::vector<Node*> iF)
{
	_ifFalse = iF;
}

void Conditional::addToTrue(Node * newIfTrue)
{
	_ifTrue.push_back(newIfTrue);
}

void Conditional::addToFalse(Node * newIfFalse)
{
	_ifFalse.push_back(newIfFalse);
}
