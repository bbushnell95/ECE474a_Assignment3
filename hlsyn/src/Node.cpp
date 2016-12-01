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
	delay = 0;
	visited = 'w';
	cycleAllowed = 0;
	asapTime = 0;
	alapTime = 0;
	scheduled = false;
}

Node::Node(std::string n, int num, std::vector<DataType*> _inputs, std::vector<DataType*> _outputs)
{
	operation = n;
	nodeNum = num;
	_componentInputs = _inputs;
	_componentOutputs = _outputs;
	visited = 'w';
	delay = 0;
	cycleAllowed = 0;
	asapTime = 0;
	alapTime = 0;
	scheduled = false;
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
	return _previousNodes;
}

void Node::setPreviousNodes(std::vector<Node*> pN)
{
	_previousNodes = pN;
}

std::vector<Node*> Node::getNextNodes()
{
	return _nextNodes;
}

void Node::setNextNodes(std::vector<Node*> nN)
{
	_nextNodes = nN;
}

std::vector<double> Node::getOperationProbability()
{
	return _operationProbability;
}

void Node::setOperationProbability(std::vector<double> oP)
{
	_operationProbability = oP;
}

std::vector<double> Node::getTotalForces()
{
	return _totalForce;
}

void Node::setTotalForces(std::vector<double> tF)
{
	_totalForce = tF;
}

char Node::getVisted()
{
	return visited;
}

void Node::setVisted(char v)
{
	visited = v;
}

int Node::getDelay()
{
	return delay;
}

void Node::setDelay(int d)
{
	delay = d;
}

int Node::getCycleAllowed()
{
	return cycleAllowed;
}

void Node::setCycleAllowed(int cA)
{
	cycleAllowed = cA;
}

int Node::getAsapTime()
{
	return asapTime;
}

void Node::setAsapTime(int asT)
{
	asapTime = asT;
}

int Node::getAlapTime()
{
	return alapTime;
}

void Node::setAlapTime(int alT)
{
	alapTime = alT;
}

bool Node::getScheduled()
{
	return scheduled;
}

void Node::setScheduled(bool s)
{
	scheduled = s;
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
	_previousNodes.push_back(previousNode);
}

void Node::addNextNode(Node * nextNode) 
{
	_nextNodes.push_back(nextNode);
}

void Node::assignOperationProbability(int latency)
{
	int i = 0;
	//double tempProb = 0.0;

	for (i = 0; i < latency; ++i) {
		if (i >= asapTime && i <= alapTime) {
			if ((alapTime - asapTime) != 0) {
				_operationProbability.push_back(1.0 / (alapTime - asapTime + 1.0));
			}
			else {
				_operationProbability.push_back(1);
			}
		}
		else {
			_operationProbability.push_back(0);
		}
	}
}

void Node::calculateSelfForce(std::vector<double> typeDistribution)
{
	int i = 0;
	int j = 0;
	double currTempAssigned = 0.0;
	double tempSelfForce = 0.0;

	//intialize each time cycle
	for (i = 0; i < (int)_operationProbability.size(); ++i) {
		_selfForce.push_back(NO_FORCE_IN_TIME_CYCLE);
	}

	//now calculate
	for (i = 0; i < (int)_operationProbability.size(); ++i) {
		//see if i is withing the time frame
		if (i >= asapTime && i <= alapTime) {
			//if the node was hyptohetically assigned to this time cycle
			currTempAssigned = typeDistribution.at(i) * (1 - _operationProbability.at(i));
			
			for (j = asapTime; j <= alapTime; ++j) {
				/* Not scheduled at time. */
				if (j != i) {
					tempSelfForce = tempSelfForce + typeDistribution.at(j) * (0 - _operationProbability.at(j));
				}
				/* Is hypothetically scheduled at time.*/
				else {
					tempSelfForce = tempSelfForce + currTempAssigned;
				}
			}
			
			_selfForce[i] = tempSelfForce;
		}
	}
}

void Node::calculatePredecessorForce(std::vector<double> multDistribution, std::vector<double> addSubDistribution, std::vector<double> modDivDistribution, std::vector<double> logicDistribution)
{
	int i = 0; // Time (node)
	int j = 0; // Node (next node)
	int k = 0; // Time
	int m = 0; // Time
	double currTempAssigned = 0.0;
	std::vector<double> currNextNodeTypeDist;

	/* Initialize vector to zero. */
	for (i = 0; i < (int)_operationProbability.size(); ++i) {
		_predecessorForce.push_back(0.0);
	}

	/* Is there any previous nodes? */
	if (_previousNodes.size() > 0) {
		/* Cycle through allowable time cycles. */
		for (i = asapTime; i <= alapTime; ++i) {
			for (j = 0; j < (int)_nextNodes.size(); ++j) {
				currTempAssigned = 0.0;
				/* Does ALAP hit ASAP? No...? */
				if (i < _nextNodes.at(j)->getAsapTime()) {
					currTempAssigned += 0;
				}
				/* Yes...? */
				else {
					for (k = 0; k < (int)_operationProbability.size(); ++k) {
						if (k >= _nextNodes.at(j)->getAsapTime() && k <= _nextNodes.at(j)->getAlapTime()) {
							if (k > i) {
								/* Determine the resource type of the next node. */
								if (_nextNodes.at(j)->getOperation() == "*") {
									currNextNodeTypeDist = multDistribution;
								}
								else if (_nextNodes.at(j)->getOperation() == "+" || _nextNodes.at(j)->getOperation() == "-") {
									currNextNodeTypeDist = addSubDistribution;
								}
								else if (_nextNodes.at(j)->getOperation() == "/" || _nextNodes.at(j)->getOperation() == "%") {
									currNextNodeTypeDist = modDivDistribution;
								}
								else {
									currNextNodeTypeDist = logicDistribution;
								}
								/* Calculate the successor force at the time specified. */
								currTempAssigned += currNextNodeTypeDist.at(k) * (1 - _nextNodes.at(j)->getOperationProbability().at(k));
								for (m = k + 1; m < (int)_nextNodes.at(j)->getAlapTime(); ++m) {
									currTempAssigned = currTempAssigned + currNextNodeTypeDist.at(m) * (0 - _nextNodes.at(j)->getOperationProbability().at(m));
								}
							}
						}
					}
				}
				_sucessorForce[i] += currTempAssigned;
			}
		}
	}
}

void Node::calculateSuccessorForce(std::vector<double> multDistribution, std::vector<double> addSubDistribution, std::vector<double> modDivDistribution, std::vector<double> logicDistribution)
{
	int i = 0; // Time (node)
	int j = 0; // Node (next node)
	int k = 0; // Time
	int m = 0; // Time
	double currTempAssigned = 0.0;
	std::vector<double> currNextNodeTypeDist;

	/* Initialize vector to zero. */
	for (i = 0; i < (int)_operationProbability.size(); ++i) {
		_sucessorForce.push_back(0.0);
	}

	/* Is there any next nodes? */
	if (_nextNodes.size() > 0) {
		/* Cycle through allowable time cycles. */
		for (i = asapTime; i <= alapTime; ++i) {
			for (j = 0; j < (int)_nextNodes.size(); ++j) {
				currTempAssigned = 0.0;
				/* Does ALAP hit ASAP? No...? */
				if (i < _nextNodes.at(j)->getAsapTime()) {
					currTempAssigned += 0;
				}
				/* Yes...? */
				else {
					for (k = 0; k < (int)_operationProbability.size(); ++k) {
						if (k >= _nextNodes.at(j)->getAsapTime() && k <= _nextNodes.at(j)->getAlapTime()) {
							if (k > i) {
								/* Determine the resource type of the next node. */
								if (_nextNodes.at(j)->getOperation() == "*") {
									currNextNodeTypeDist = multDistribution;
								}
								else if (_nextNodes.at(j)->getOperation() == "+" || _nextNodes.at(j)->getOperation() == "-") {
									currNextNodeTypeDist = addSubDistribution;
								}
								else if (_nextNodes.at(j)->getOperation() == "/" || _nextNodes.at(j)->getOperation() == "%") {
									currNextNodeTypeDist = modDivDistribution;
								}
								else {
									currNextNodeTypeDist = logicDistribution;
								}
								/* Calculate the successor force at the time specified. */
								currTempAssigned += currNextNodeTypeDist.at(k) * (1 - _nextNodes.at(j)->getOperationProbability().at(k));
								for (m = k + 1; m < (int)_nextNodes.at(j)->getAlapTime(); ++m) {
									currTempAssigned = currTempAssigned + currNextNodeTypeDist.at(m) * (0 - _nextNodes.at(j)->getOperationProbability().at(m));
								}
							}
						}
					}
				}
				_sucessorForce[i] += currTempAssigned;
			}
		}
	}
}

void Node::calculateTotalForce()
{
	int i = 0; 

	for (i = 0; i < (int)_operationProbability.size(); ++i) {
		_totalForce.push_back(9999);
	}

	for (i = 0; i < (int)_totalForce.size(); ++i) {
		_totalForce[i] = _selfForce[i] + _predecessorForce[i] + _sucessorForce[i];
	}

}

void Node::assignDelay()
{
	if (!operation.compare("*")) {
		delay = MULT_DELAY;
	}
	else if (!operation.compare("/") || !operation.compare("%")) {
		delay = DIV_MOD_DELAY;
	}
	else if (!operation.compare("+") || !operation.compare("-")) {
		delay = ADD_SUB_DELAY;
	}
	else {
		delay = LOGIC_DELAY;
	}
}