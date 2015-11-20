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
	this->skipped = false;
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

int Job::getStart()
{
	return this->start;
}

bool Job::getSkipped()
{
	return this->skipped;
}

void Job::setStart(int location)
{
	this->start = location;
}

void Job::setSkipped()
{
	this->skipped = true;
}

Job::~Job()
{
	// destructor
}