/*
Name: 	 Tyler Kickham
Program: Round Robin Simulator (OS3)
Purpose: Simuate a round robin scheduler to see how tweaks in scheduling can effect output
Input:   The input is the name of a text file typed in from the keyboard which contains
Output:  
Completed: getFile, getRestInfo, checkNewJob, removeQuantum
TODO: what if two jobs start at same time
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

bool rando = false;										// not randomly generated probabilities
// user specifiers
int simTime, quantum, degree;							// sim length, quantum size, max processes
// stats
int jobsDone = 0;										// how many jobs have finished
int switchTime = 4;										// time to swap jobs (ms)
int quantumCounter = 0;									// counter for taking off quantum
int runTime = 1;									// start sim at 1 unit time
vector<Job> jobQ;										// queue jobs waiting for start time
vector<Job> readyQ;										// queue jobs ready for CPU
vector<Job> ioQ;										// queue jobs waiting to do IO

int main()
{
	srand(time(NULL));									// seed for random number generator
	getFile();											// get the input file
	getRestInfo();										// get the rest of the info needed for the simluation

	roundRobin();										// scheduler

	debugTest();										// debugger for filling readyQ after reading file
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
// INPUT:
// OUTPUT: random int
// generate a random int for prob of IO/length of IO
{
	int returnVal;										// either probability or length to return
	if (probOrLen == 0)									// if generating probability
		returnVal = rand() % 5 + 0;						// probability of IO is between 0 and .05
	else												// else generating length
		returnVal = rand() % 25 + 5;					// length of next IO request
	return returnVal;									// return the value generated
}

void roundRobin()
// INPUT: none
// OUTPUT: none
// schedule simulator
{
	int isJob, jobTime, newJob;							// is there job at current time, job in IO time left
	while (runTime < simTime*1000)						// while there is still time left
	{
		if (readyQ.size() < degree)						// if the degree is not yet met
			checkNewJob(runTime);				// do the job at index returned from check if there is a job starting
		doJob();									// do not check for new jobs and send signal to do job from readyQ
		// if (ioQ.size() > 0)								// if there are items waiting on IO
			//
		runTime += 1;									// increment runtime
	}
}

void checkNewJob(int currentTime)
// INPUT: current time (ms)
// OUTPUT: queue index or -1 whether there is a job that starts at current time
// checks if there is a job that starts at current time and returns if there is or not
{
	for (int i=0; i<jobQ.size(); i++)					// for each job in readyQ
	{
		if (currentTime == jobQ.at(i).getStart())		// if there is a matching startTime
		{
			readyQ.push_back(jobQ.at(i));				// add new job to end of queue
			jobQ.erase(jobQ.begin()+i);					// erase the new job from the job queue
			cout << "new job added\n";
		}
	}
}

void doJob()
{
	Job currentJob;										// object being worked on
	int ioLen;
	if (ioQ.size() > 0)
	{
		currentJob = ioQ.front();
		ioLen = currentJob.getIOLen();
		ioLen -= 1;
		ioQ.front().setIOLen(ioLen);
		// cout << ioQ.front().getIOLen() << endl;
		if (ioLen <= 0)
		{
			readyQ.push_back(currentJob);
			ioQ.erase(ioQ.begin());
		}
	}
	if (readyQ.size() > 0)								// if readyQ has jobs waiting
	{
		currentJob = readyQ.front();					// set job to the first in the queue
		bool needsIO = false;
		// bool needsIO = probIO(randomGen(0), currentJob);
		if (!needsIO)
			removeQuantum(currentJob);					// call function to remove quantum and place at back of queue
		else
		{
			cout << "in IO\n";
			ioLen = randomGen(1);
			currentJob.setIOLen(ioLen);
			ioQ.push_back(currentJob);
		}
		readyQ.erase(readyQ.begin());					// erase current job from front
	}
}

void removeQuantum(Job currentJob)
// INPUT: job object to modify
// OUPUT: none
// removes quantum from job
{
	int jobTime = currentJob.getLeft();					// get the job's time left
	quantumCounter += 1;								// add one to quantum counter
	
	if (quantumCounter < quantum)						// if the quantum isn't expired
		readyQ.insert(readyQ.begin(), currentJob);		// push job to front of queue
	else												// otherwise
	{
		currentJob.setLeft(jobTime-quantum);			// subtract quantum from jobTime
		// cout << quantumCounter << " " << quantum << " " << runTime << " " << currentJob.getPID() << " " << currentJob.getLeft() << endl;
		readyQ.push_back(currentJob);					// push job to back of queue
		quantumCounter = 0;								// reset quantumCounter to 0
	}
	if (currentJob.getLeft() <= 0)						// if the job is done
	{
		jobsDone += 1;									// add one to jobsDone counter
		cout << currentJob.getPID() << " finished" << endl;
		readyQ.erase(readyQ.end());
	}
}

bool probIO(int genProb, Job currentJob)
// INPUT: probability of needing IO, current job object working on
// OUTPUT: true if the job needs IO, false if it doesn't
// calculates if the job is going to need IO
{
	return (genProb <= currentJob.getProb());
}

void debugTest()
// debug and show that each job is in readyQ
{
	// check that everything got into ready queue -----------
	Job currentJob;
	int currentPID, currentStartTime, currentProb, currentTimeLeft, currentIOTime;
	cout << "jobQ size: " << jobQ.size() << endl;
	cout << "readyQ:\n";
	for (int i=0; i<readyQ.size(); i++)
	{
		currentJob = readyQ.at(i);
		currentStartTime = currentJob.getStart();
		currentPID = currentJob.getPID();
		currentProb = currentJob.getProb();
		currentTimeLeft = currentJob.getLeft();
		currentIOTime = currentJob.getIOLen();
		cout << currentStartTime << " " << currentPID << " " << currentProb << " " << currentTimeLeft << " " << currentIOTime << endl;
	}
	cout << "ioQ:\n";
	for (int i=0; i<ioQ.size(); i++)
	{
		currentJob = ioQ.at(i);
		currentStartTime = currentJob.getStart();
		currentPID = currentJob.getPID();
		currentProb = currentJob.getProb();
		currentTimeLeft = currentJob.getLeft();
		currentIOTime = currentJob.getIOLen();
		cout << currentStartTime << " " << currentPID << " " << currentProb << " " << currentTimeLeft << " " << currentIOTime << endl;
	}
	// check that rest of input made it ---------------------
	// cout << simTime << " " << quantum << " " << degree << endl;
	// random number generator ------------------------------
	// int probIO = randomGen(0);		// probability of IO
	// int lengthIO = randomGen(1);	// length of IO
	// cout << probIO << endl;
	// cout << lengthIO << endl;
	cout << "Number total jobs: " << readyQ.size() + ioQ.size() + jobQ.size() + jobsDone << endl;
	cout << "Throughput: " << jobsDone << endl;
	cout << "Number of jobs still in system: " << readyQ.size() + ioQ.size() << endl;
	cout << "Number of jobs skipped: " << jobQ.size() << endl;
	cout << "Avg job length: " << endl;
	cout << "Avg turnaround: " << endl;
	cout << "Avg wait time: " << endl;
	cout << "CPU utilization: " << endl;
}