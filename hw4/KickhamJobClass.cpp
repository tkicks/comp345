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
	return this->PID;
}

int Job::getSize()
{
	return this->size;
}

Job::~Job()
{
	// destructor
}