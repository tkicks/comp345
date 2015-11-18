#include "KickhamJobClass.h"

Job::Job()
{
	// constructor
}

Job::Job(char op, int PID, int size)
{
	this->op = op;
	this->PID = PID;
	this->size = size;
}

char Job::getOp()
{
	return this->op;
}

int Job::getPID()
{
	if (this->op == 'A' or this->op == 'D')
		return this->PID;
	else
		return 0;
}

int Job::getSize()
{
	if (this->op == 'A')
		return this->size;
	else
		return 0;
}

Job::~Job()
{
	// destructor
}