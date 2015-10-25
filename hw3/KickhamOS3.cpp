/*
Name: 	 Tyler Kickham
Program: Round Robin Simulator (OS3)
Purpose: Simuate a round robin scheduler to see how tweaks in scheduling can effect output
Input:   The input is the name of a text file typed in from the keyboard which contains
Output:  
Completed: getFile, getRestInfo, generate random number (give it use though)
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
vector<Job> jobQ;										// queue jobs waiting for start time
vector<Job> readyQ;										// queue jobs ready for CPU
vector<Job> ioQ;										// queue jobs waiting to do IO

int main()
{
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
	srand(time(NULL));
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
	int runTime = 1;									// start sim at 1 unit time
	while (runTime < simTime*1000)						// while there is still time left
	{
		if (readyQ.size() < degree)					// if the degree is not yet met
			doJob(checkNewJob(runTime));				// do the job at index returned from check if there is a job starting
		else											// otherwise if too many jobs
			doJob(-1);									// do not check for new jobs and send signal to do job from readyQ
		runTime += 1;									// increment runtime
	}
}

int checkNewJob(int currentTime)
// INPUT: current time (ms)
// OUTPUT: queue index or -1 whether there is a job that starts at current time
// checks if there is a job that starts at current time and returns if there is or not
{
	for (int i=0; i<jobQ.size(); i++)					// for each job in readyQ
	{
		if (currentTime == jobQ.at(i).getStart())		// if there is a matching startTime
			return i;									// return index
	}
	return -1;											// return no new jobs
}

void doJob(int vectorIndex)
{
	Job currentJob;										// object being worked on
	if (vectorIndex != -1)								// if there is no job starting at this time
	{
		currentJob = jobQ.at(vectorIndex);				// set job to starting index job
		removeQuantum(currentJob);						// call function to remove quantum and place at back of queue
		jobQ.erase(jobQ.begin()+vectorIndex);			// erase the current job from unstarted jobs queue
	}
	else if (readyQ.size() > 0)							// otherwise if readyQ has jobs waiting
	{
		currentJob = readyQ.front();					// set job to the first in the queue
		removeQuantum(currentJob);						// call function to remove quantum and place at back of queue
		readyQ.erase(readyQ.begin());					// erase current job from front
	}
}

void removeQuantum(Job currentJob)
// INPUT: job object to modify
// OUPUT: none
// removes quantum from job
{
	int jobTime = currentJob.getLeft();					// get the job's time left
	jobTime -= quantum;									// subtract quantum from time left
	if (jobTime >= 0)									// if job isn't done
	{
		currentJob.setLeft(jobTime);					// set the new time left
		readyQ.push_back(currentJob);					// push job to back of queue
	}
	else												// otherwise if the job is done
		jobsDone += 1;									// add a job completed
}

void debugTest()
// debug and show that each job is in readyQ
{
	// check that everything got into ready queue -----------
	Job currentJob;
	int currentPID, currentStartTime, currentProb, currentTimeLeft;
	for (int i=0; i<readyQ.size(); i++)
	{
		currentJob = readyQ.at(i);
		currentStartTime = currentJob.getStart();
		currentPID = currentJob.getPID();
		currentProb = currentJob.getProb();
		currentTimeLeft = currentJob.getLeft();
		cout << currentStartTime << " " << currentPID << " " << currentProb << " " << currentTimeLeft << endl;
	}
	// check that rest of input made it ---------------------
	// cout << simTime << " " << quantum << " " << degree << endl;
	// random number generator ------------------------------
	// int probIO = randomGen(0);		// probability of IO
	// int lengthIO = randomGen(1);	// length of IO
	// cout << probIO << endl;
	// cout << lengthIO << endl;
	cout << "Throughput: " << jobsDone << endl;
}