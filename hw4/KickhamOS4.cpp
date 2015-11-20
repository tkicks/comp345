/*
Name: 	 Tyler Kickham
Program: First Fit Memory Allocation (OS4)
Purpose: Explore first fit memory allocation and deallocation of processes
Input:   User will input the name of a file, and if it's good it will contain the following format
		 <op> <PID> <size>
		 where <op> is a char denoting whether the processes is to be allocated ('A'), deallocated ('D'), or if there are no more in the file ('Q')
		 <PID> and <size> are ints
Output:  A table will be displayed showing the ending memory schematic with PID, beginning/ending locations, and size of processes in memory
		 the number of processes skipped because they didn't fit, amount of memory being used, amount of memory remaining, number of contiguous blocks,
		 and the average size of the contiguous blocks of free memory
*/


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include "KickhamJobClass.cpp"
#include "KickhamSimClass.cpp"
using namespace std;


int main()
{
	cout << endl;										// separate output from command line
	vector<Job> processes;								// vector for processes
	simulation memory;									// object of simulation class depicting memory
	memory.getFile(processes, memory);					// store processes from file into vector (and allocate to memory)
	memory.getTable(processes, memory);					// gets data for table (calls write table)
}