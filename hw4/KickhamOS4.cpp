/*
Name: 	 Tyler Kickham
Program: First Fit Memory Allocation (OS4)
Purpose: 
Input:   
Output:  
*/


#include <vector>
#include "KickhamSimClass.cpp"
#include "KickhamJobClass.cpp"
#include "KickhamOS4.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;


int main()
{
	vector<Job> processes;								// vector for processes
	getFile(processes);									// store processes from file into vector
}

void getFile(vector<Job> &processes)
// read input file
{
	Job *newJob;
	string filename;
	char op;
	int PID, size;
	cout << "Enter filename: ";
	cin >> filename;
	ifstream dataFile(filename.c_str());
	if (!dataFile.good())
	{
		cout << "File not found\n";
		exit(1);
	}

	do {
		dataFile >> op >> PID;
		if (op != 'D')
			dataFile >> size;
		newJob = new Job(op, PID, size);
		cout << "op: " << newJob->getOp() << " PID: " << newJob->getPID() << " size: " << newJob->getSize() << endl;
	} while (op != 'Q');
}