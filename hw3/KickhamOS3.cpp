/*
Name: 	 Tyler Kickham
Program: Round Robin Simulator (OS3)
Purpose: Simuate a round robin scheduler to see how tweaks in scheduling can effect output
Input:   The input is the name of a text file typed in from the keyboard which contains
Output:  
*/

#include "KickhamOS3.h"
#include "KickhamClass.cpp"
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <deque>
using namespace std;

bool rando = false;										// not randomly generated probabilities
int switchTime = 4;										// time to swap jobs (ms)
deque<int> readyQ;										// queue size numLines for ready jobs
deque<int> ioQ;											// queue size numLines for IO

int main()
{
	getFile();											// get the input file
	int pid;
	for (deque<int>::iterator it = readyQ.begin(); it != readyQ.end(); ++it)
	{
		pid = it.getPID();
		cout << "PID: " << pid << endl;
	}
	// getRestInfo();
}

void getFile()
// INPUT: none
// OUTPUT: none
// get file and simulation params
{
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

	dataFile >> startTime;								// read in number of lines (deque doesn't matter b/c dynamic array)
	while (!dataFile.eof())								// while more to read in
	{
		dataFile >> startTime >> pid >> prob >> length;	// gather all data
		Job(startTime, pid, prob, length);				// create new item of class job
	}
}


// void getRestInfo()
// {
// 	int simTime, quantum, degree;						// sim length, quantum size, max processes
// 	cout << "Enter simulation time (int seconds): ";	// ask for sim time
// 	cin >> simTime;										// store sim time
// 	cout << "Enter quantum size: ";						// ask for quantum size
// 	cin >> quantum;										// store quantum
// 	cout << "Enter degree of multiprogramming: "		// ask for num process allowed
// 	cin >> degree;										// store degree of multiprogramming
// }