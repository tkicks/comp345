/*
TODO: pass array by reference
	  split into functions
	  figure out how to bundle args to thread (struct?)
	  threading (A LOT)

Name: Tyler Kickham
Program: Multithreaded sort
Purpose: The purpose of this program is to sort a large list of
		 numbers on multiple threads
Input: The input is the name of a text file typed in from the keyboard
Output: The output is five text files, one for each of the threads' sort
		and one that has the final, sorted list.
*/

#include "kickhamOS1.h"
#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
using namespace std;


void readFile(string filename, int* numList)
{
	int line, numLines;						// var to hold value read in
	int lineNum = 0;						// var to hold line number
	ifstream numberFile(filename.c_str());	// convert filename to chars to be opened
	numberFile >> line;						// take number of lines
	numLines = line;						// numLines = first line
	// int numList[numLines];				// make an array of numbers size line
	numList = new int[numLines];

	while (lineNum < numLines)				// while not past the last line
	{
		numberFile >> line;					// read in the line
		numList[lineNum] = line;			// store the line in the next spot in array
		// cout << numList[lineNum] << endl;	// print the inputted number (debug only)
		lineNum++;							// move counter to next line
	}

// ================================================================
// 		THREADING
	pthread_t thread1, thread2,
			  thread3, thread4;				// up to 4 threads
	int half1, half2, quarter1,
		quarter2, quarter3, quarter4;		// breakpoints in array

	if (numLines < 5)						// 5 for debug
	{
		half1 = numLines/2;					// breakpoint
		pthread_create(&thread1, NULL,
							sortFunct, (void*) /*bundled args*/);
	}

//
// ================================================================

// ================================================================
//		BUBBLE SORT
	int temp;
	for (int i=0; i < numLines; i++)
	{
		for (int x=0; x < numLines-1; x++)
		{
			if (numList[x] > numList[x+1])
			{
				temp = numList[x+1];
				numList[x+1] = numList[x];
				numList[x] = temp;
			}
		}
	}
//
// ===============================================================

	for (int i = 0; i < numLines; i++)		//
		cout << numList[i] << endl;			// print out sorted list

	// sortList(numList, numLines);			// send to sortList

	// cout << numList[2] << " : " << numList[9] << "\n";	// (debugging) all stored
}

// void sortList(int* numList, int numLines)
// {
// 	int temp;
// 	for (int i=0; i < numLines; i++)
// 	{
// 		for (int x=0; x < numLines-1; x++)
// 		{
// 			if (numList[x] > numList[x+1])
// 			{
// 				temp = numList[x+1];
// 				numList[x+1] = numList[x];
// 				numList[x] = temp;
// 			}
// 		}
// 	}

// 	for (int i = 0; i < numLines; i++)
// 		cout << numList[i] << endl;
// }

int main()
{
	int* numList;
	string filename;
	cout << "Enter filename: ";
	cin >> filename;

	readFile(filename, numList);
	cout << numList[9] << endl;				// doesn't return by ref correctly
}