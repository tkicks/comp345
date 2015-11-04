/*
Name: 	 Tyler Kickham
Program: Round Robin Simulator (OS3)
Purpose: Simulate a round robin scheduler to see how tweaks in quantum/degree/sim time can effect output
Input:   The input is the name of a text file typed in from the keyboard which contains four columns
		 (start time, PID, prob IO, length) after a first line that states how many jobs (lines) are in file
Output:  Stats gained from simulation (# jobs, throughput, jobs still in system, skipped, avg length, avg
		 turnaround, avg wait time, CPU utilization) displayed in console
TODO: make vars local/part of class not global
*/

#include "KickhamClass.cpp"
#include "KickhamOS3.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <time.h>
using namespace std;

// constants
const bool rando = true;								// true will randomize need of probability, false will not
const int switchTime = 4;								// time to swap jobs (ms)
// user specifiers
int simTime, quantum, degree;							// sim length, quantum size, max processes
// stats
int quantumCounter = 0;									// counter for taking off quantum
int waitTime = 0;										// calculates total wait time
// vectors
vector<Job> jobQ;										// queue jobs waiting for start time
vector<Job> readyQ;										// queue jobs ready for CPU
vector<Job> ioQ;										// queue jobs waiting to do IO
vector<Job> doneQ;										// vector of jobs that have finished

int main()
{
	srand(time(NULL));									// seed for random number generator
	getFile();											// get the input file
	getRestInfo();										// get the rest of the info needed for the simluation

	roundRobin();										// scheduler
}

void getFile()
// INPUT: none
// OUTPUT: none
// get jobs from the file
{
	Job *newJob;										// init newJob pointer for new objects of class Job
	string filename;									// string for filename
	int startTime, pid, prob, length;					// start time, PID, probability of IO, length of job
	cout << "Enter filename: ";							// ask for filename
	cin >> filename;									// store filename

	ifstream dataFile(filename.c_str());				// filename turned into input stream
	if (!dataFile.good())								// if it's not found
	{
		cout << "File not found\n";						// tell user it's not found
		exit(1);										// exit
	}

	dataFile >> startTime;								// read in number of lines (doesn't matter b/c vector is dynamic)
	while (!dataFile.eof())								// while more to read in
	{
		dataFile >> startTime >> pid >> prob >> length;	// gather all data
		if (rando == true)								// global constant to determine if randomly generate probability
			prob = randomGen(2);						// pass prob signal (2)
		newJob = new Job(startTime, pid, prob, length);	// create new item of class job
		jobQ.push_back(*newJob);						// push job to back of readyQ
	}
}

void getRestInfo()
// INPUT: none
// OUTPUT: none
// ask for and receive the simulation length, quantum, and degree of multiprogramming
{
	cout << "Enter simulation time (seconds): ";		// ask for sim time
	cin >> simTime;										// store sim time
	cout << "Enter quantum size (ms): ";				// ask for quantum size
	cin >> quantum;										// store quantum
	cout << "Enter degree of multiprogramming: ";		// ask for num process allowed
	cin >> degree;										// store degree of multiprogramming
}

int randomGen(int probOrLen)
// INPUT: int signal to determine whether to generate a probability, length of IO, or probability of IO
// OUTPUT: random int
// generate a random int for prob of IO/length of IO/probability of needing IO
{
	int returnVal;										// either probability or length to return
	if (probOrLen == 0)									// if generating probability
		returnVal = rand() % 100 + 0;					// probability of IO is between 0 and .05
	else if (probOrLen == 1)							// or if generating length of IO
		returnVal = rand() % 25 + 5;					// length of next IO request
	else												// else
		returnVal = rand() % 5 + 0;						// probability of needing IO
	return returnVal;									// return the value generated
}

void roundRobin()
// INPUT: none
// OUTPUT: none
// schedule simulator
{
	int runTime = 1;									// start sim at 1 unit time
	while (runTime < simTime*1000)						// while there is still time left
	{
		if (readyQ.size() + ioQ.size() < degree)		// if the degree is not yet met
			checkNewJob(runTime);						// do the job at index returned from check if there is a job starting
		doJob(runTime);									// do not check for new jobs and send signal to do job from readyQ
		runTime += 1;									// increment runtime
	}
	writeStats(runTime);								// write out all the final stats
}

void checkNewJob(int currentTime)
// INPUT: current time (ms)
// OUTPUT: none
// checks if there is a job that starts at current time, and if so put it at the back of readyQ
{
	for (int i=0; i<jobQ.size(); i++)					// for each job in readyQ
	{
		if (currentTime == jobQ.at(i).getStart())		// if there is a matching startTime
		{
			readyQ.push_back(jobQ.at(i));				// add new job to end of queue
			jobQ.erase(jobQ.begin()+i);					// erase the new job from the job queue
		}
	}
}

bool probIO(int genProb, Job currentJob)
// INPUT: probability of needing IO, current job object working on
// OUTPUT: true if the job needs IO, false if it doesn't
// calculates if the job is going to need IO
{
	return (genProb <= currentJob.getProb());			// return if the generated probability is within the read in one
}

void doJob(int runTime)
// INPUT: current run time
// OUTPUT: none
// performs a clock tick of IO if jobs waiting, checks if CPU job needs IO
{
	Job currentJob;										// object being worked on
	int ioLen;											// length of IO
	if (ioQ.size() > 0)									// if there are jobs waiting for IO
	{
		currentJob = ioQ.front();						// current job is first one in queue
		ioLen = currentJob.getIOLen();					// get the length of IO left for it
		ioLen -= 1;										// decrease by one
		ioQ.front().setIOLen(ioLen);					// reset the length of IO
		if (ioLen <= 0)									// if it is done with IO
		{
			// cout << currentJob.getPID() << " out of IO at " << runTime << endl;
			readyQ.push_back(currentJob);				// put the job back on the CPU queue
			ioQ.erase(ioQ.begin());						// erase it from the IO queue
		}
	}
	if (readyQ.size() > 0)								// if readyQ has jobs waiting
	{
		currentJob = readyQ.front();					// set job to the first in the queue
		// bool needsIO = false;	// while testing, uncomment next when working
		bool needsIO = probIO(randomGen(0), currentJob);// check if the job needs IO w/ generated number and current job to f(x)
		if (!needsIO)									// if it doesn't need IO
			removeQuantum(currentJob, runTime);			// call function to remove quantum and place at back of queue
		else											// otherwise it does need IO
		{
			ioLen = randomGen(1);						// generate a random length of IO
			currentJob.setIOLen(ioLen);					// set the IO length left of the job to the generated value
			ioLen += currentJob.getTotalIO();			// increase job's total time in IO by new IO length
			currentJob.setTotalIO(ioLen);				// store job's total IO
			// cout << "job " << currentJob.getPID() << " in IO for " << currentJob.getIOLen() << " totaling " << currentJob.getTotalIO() << " at " << runTime << endl;
			ioQ.push_back(currentJob);					// put the job in the back of the IO queue
		}
		readyQ.erase(readyQ.begin());					// erase current job from front
	}
}

void removeQuantum(Job currentJob, int runTime)
// INPUT: job object to modify, current time in program
// OUPUT: none
// removes quantum from job after expired, takes care of swap time as well
{
	int jobTime = currentJob.getLeft();					// get the job's time left
	quantumCounter += 1;								// add one to quantum counter

	if (readyQ.size() > 1)								// if there are more than one jobs waiting for CPU
		waitTime += 1;									// add one to wait time
	if (quantumCounter < quantum)						// if the quantum isn't expired
	{
		// cout << currentJob.getPID() << " in CPU at " << runTime << endl;
		readyQ.insert(readyQ.begin(), currentJob);		// push job to front of queue
	}
	else												// otherwise
	{
		currentJob.setLeft(jobTime-quantum);			// subtract quantum from jobTime
		readyQ.push_back(currentJob);					// push job to back of queue
		if (readyQ.size() > 2)							// if jobs waiting in ready queue
		{
			runTime += 4;								// add 4 for swap time
			waitTime += 4;								// add wait time equivalent to swap time
		}
		if (currentJob.getLeft() <= 0)					// if the job is done
		{
			runTime += 1;								// increase runTime
			jobDone(currentJob, runTime);				// call function to get ending stats
		}
		quantumCounter = 0;								// reset quantumCounter to 0
	}
}

void jobDone(Job currentJob, int runTime)
// INPUT: job object to get stats from, runTime so far
// OUTPUT: none
// calculate turnaround b/c need current time
{
	// turnaround ----------------------------------------
	int startTime = currentJob.getStart();				// get the starting time of the job
	int turnAround = runTime - startTime;				// calculate turnaround time
	currentJob.setTurnaround(turnAround);				// store the turnaround time for the job
	// cout << "turnaround: " << turnAround << " currentJob.getTA(): " << currentJob.getTurnaround() << endl;
	// cout << currentJob.getPID() << " started at " << startTime << " finished after " << turnAround << " at " << runTime << " after waiting " << waitTime << endl;
	doneQ.push_back(currentJob);						// put job into the finished jobs queue
	readyQ.erase(readyQ.end());							// erase job from from CPU queue
}

void writeStats(int runTime)
// INPUT: current run time (ending time)
// OUTPUT: stats from simulation (# jobs, thruput, in system, skip, len, turnaround, wait, Ucpu)
// finds summation of lengths and turnarounds of completed jobs and Ucpu and displays to console
{
	Job currentJob;										// job object to be used
	// cout << "jobQ size: " << jobQ.size() << endl;
	// cout << "readyQ: " << readyQ.size() << endl;

	int lenJob = 0;										// total length of jobs
	int turnAround = 0;									// total turnaround
	int jobsDone = doneQ.size();						// number of jobs finished
	for (int i = 0; i < doneQ.size(); i++)				// for each job in the finished queue
	{
		currentJob = doneQ.at(i);						// current job is one at index
		lenJob += currentJob.getLength();				// add job's length to total length of jobs
		turnAround += currentJob.getTurnaround();		// add job's turnaround to total turnaround
		// cout << turnAround << endl;
	}
	// cout << "job length: " << lenJob << endl;
	// cout << "run time: " << runTime << endl;
	// cout << "turnaround: " << turnAround << endl;
	int avgLength = lenJob/jobsDone;					// calculate average length of jobs
	int avgTurnaround = turnAround/jobsDone;			// calculate average turnaround
	int avgWaitTime = waitTime/jobsDone;				// calculate average wait time
	int Ucpu = (100*lenJob)/runTime;					// calculate CPU utilization

	// display stats
	cout << "\n\nNumber total jobs: " << readyQ.size() + ioQ.size() + jobQ.size() + jobsDone << endl;
	cout << "Throughput: " << jobsDone << endl;
	cout << "Number of jobs still in system: " << readyQ.size() + ioQ.size() << endl;
	cout << "Number of jobs skipped: " << jobQ.size() << endl;
	cout << "Avg job length: " << avgLength << " ms" << endl;
	cout << "Avg turnaround: " << avgTurnaround << " ms" << endl;
	cout << "Avg wait time: " << avgWaitTime << " ms" << endl;
	cout << "CPU utilization: " << Ucpu << '%' << endl;
}