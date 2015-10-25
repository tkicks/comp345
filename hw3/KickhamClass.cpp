#include "KickhamClass.h"

Job::Job()
{
	this->startTime = 0;
	this->pid = 0;
	this->prob = 0;
	this->timeLeft = 0;
}

Job::Job(int startTime, int pid, int prob, int length)
// INPUT: ints for start time, PID, probability of IO, length of process
// OUTPUT: none
// CTOR for new jobs read in from file
{
	this->startTime = startTime;
	this->pid = pid;
	this->prob = prob;
	this->timeLeft = length;
}

Job::~Job()
{
	// delete;
}

int Job::getStart()
{
	return this->startTime;
}

int Job::getPID()
{
	return this->pid;
}

int Job::getProb()
{
	return this->prob;
}

int Job::getLeft()
{
	return this->timeLeft;
}

int Job::getIOLen()
{
	return this->ioLen;
}

void Job::setLeft(int timeLeft)
{
	this->timeLeft = timeLeft;
}

void Job::setIOLen(int ioLen)
{
	this->ioLen = ioLen;
}