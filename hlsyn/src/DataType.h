/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 10/17/2016
Assignment: 2
File: DataType.h
Description: Data Type class for dpgen program
*/

#ifndef DATATYPE_H
#define DATATYPE_H

#include<string>
#include<vector>
#include<cstring>
#include<algorithm>
//#include"DatapathComponent.h"

class DatapathComponent;

class DataType {
protected:
	std::string name;
	int dataWidth;
	bool signUnsigned; //used to see if variable is singed or unsigned (1 signed, 0 unsigned)


public:
	/*Methods*/
	std::string getName();
	void setName(std::string n);
	int getDataWidth();
	void setDataWidth(int dW);
	bool getSignUnsigned();
	void setSignUnsiged(bool sU);

};
#endif // DATATYPE_H