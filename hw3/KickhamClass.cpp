#include "KickhamClass.h"

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
	delete this;
}