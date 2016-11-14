/*
Students: Brett Bushnell (Undergrad), Matt Dzurick (Grad)
Date Create: 10/17/2016
Assignment: 2
File: Output.cpp
Description: output Class for dpgen program
*/

#include "Wire.h"

Wire::Wire()
{
	name = "Unknown";
	dataWidth = 0;
}

Wire::Wire(std::string n, bool s, int dW)
{
	name = n;
	signUnsigned = s;
	dataWidth = dW;
}


//std::string Wire::getName()
//{
//	return name;
//}
//
//void Wire::setName(std::string n)
//{
//	name = n;
//}
//
//int Wire::getDataWidth()
//{
//	return dataWidth;
//}
//
//void Wire::setDataWidth(int dW)
//{
//	dataWidth = dW;
//}
