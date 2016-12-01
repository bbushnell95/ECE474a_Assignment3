/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 12/1/2016
Assignment: 3
File: Conditional.h
Description: Conditional Class for hlsyn program
*/

#ifndef CONDITIONAL_H
#define CONDITIONAL_H

#include<string>
#include<vector>
#include<cstring>
#include<algorithm>

class Node;

class Conditional {

	private:
		Node* basedOn;
		std::vector<Node*> _ifTrue;
		std::vector<Node*> _ifFalse;

public:
	/*constructors*/
	Conditional();

	Node* getBasedOn();
	void setBasedOn(Node* bO);
	std::vector<Node*> getIfTrue();
	void setIfTrue(std::vector<Node*> iT);
	std::vector<Node*> getIfFalse();
	void setIfFalse(std::vector<Node*> iF);
	void addToTrue(Node* newIfTrue);
	void addToFalse(Node* newIfFalse);

};

#endif // CONDITIONAL_H
