#include "KickhamClass.h"

Job::Job()
// INPUT: none
// OUTPUT: none
// default CTOR for jobs w/ no input
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
	this->length = length;
	this->totalIO = 0;
	this->ioLen = 0;
}

Job::~Job()
// INPUT: none
// OUTPUT: none
// Destructor
{
	// delete;
}

int Job::getStart()
// INPUT: none
// OUTPUT: int start time for job
// returns time job object started
{
	return this->startTime;
}

int Job::getPID()
// INPUT: none
// OUTPUT: int PID for job
// returns the PID of job object
{
	return this->pid;
}

int Job::getProb()
// INPUT: none
// OUTPUT: int probability of IO
// returns prob of job object
{
	return this->prob;
}

int Job::getLeft()
// INPUT: none
// OUTPUT: int time job has left for completion
// returns time left of job object
{
	return this->timeLeft;
}

int Job::getIOLen()
// INPUT: none
// OUTPUT: int length of IO
// returns length of IO left to do of job object
{
	return this->ioLen;
}

int Job::getTurnaround()
// INPUT: none
// OUTPUT: int turnaround
// returns turnaround of job object
{
	return this->turnaround;
}

int Job::getLength()
// INPUT: none
// OUTPUT: int length
// returns total length of job object
{
	return this->length;
}

int Job::getWaitTime()
// INPUT: none
// OUTPUT: int wait time
// returns total wait time of job object
{
	return this->waitTime;
}

int Job::getTotalIO()
// INPUT: none
// OUTPUT: int total IO time
// returns total time spent in IO of job object
{
	return this->totalIO;
}

void Job::setLeft(int timeLeft)
// INPUT: int length of time left
// OUTPUT: none
// sets length of time left for completion of job object
{
	this->timeLeft = timeLeft;
}

void Job::setIOLen(int ioLen)
// INPUT: int length of IO
// OUTPUT: none
// sets length of IO of job object
{
	this->ioLen = ioLen;
}
void Job::setTotalIO(int ioLen)
// INPUT: int length of IO
// OUTPUT: none
// sets length of IO of job object
{
	this->totalIO = ioLen;
}

void Job::setTurnaround(int turnAround)
// INPUT: int length of IO
// OUTPUT: none
// sets turnaround of job object
{
	this->turnaround = turnAround;
}

void Job::setWaitTime(int waitTime)
// INPUT: int length of IO
// OUTPUT: none
// sets wait time of job object
{
	this->waitTime = waitTime;
}

void Job::setLength(int length)
// INPUT: int length of IO
// OUTPUT: none
// sets length of job object
{
	this->length = length;
}