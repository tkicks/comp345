/*
Name: 	 Tyler Kickham
Program: First Fit Memory Allocation (OS4)
Purpose: 
Input:   
Output:  
*/


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "KickhamJobClass.cpp"
#include "KickhamSimClass.cpp"
#include "KickhamOS4.h"
using namespace std;


const int memSize = 1048576;							// size of memory (bytes)


int main()
{
	vector<Job> processes;								// vector for processes
	simulation memory;									// object of simulation class depicting memory
	memory.getFile(processes, memory);					// store processes from file into vector

	// test vector (working)
	for (int i = 0; i < processes.size(); i++)
		cout << "process " << i << ": op: " << processes.at(i).getOp() << " PID: " << processes.at(i).getPID() << " size: " << processes.at(i).getSize() << endl;
}