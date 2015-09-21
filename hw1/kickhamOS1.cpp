/*
TODO: create struct to hold list info (starting address, length)
	  threading to sort

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

struct storeInfo {
	int beginSort;					// begin the sort here for thread
	int endSort;					// end sort here for thread
	int half;						// which half is running
	int* numList;					// array of numbers
};


void readFile(string filename, int* numList)
{
	

// ================================================================
// 		THREADING
	// pthread_t thread1, thread2,
	// 		  thread3, thread4;				// up to 4 threads
	// int half1, half2, quarter1,
	// 	quarter2, quarter3, quarter4;		// breakpoints in array

	// if (numLines < 5)						// 5 for debug
	// {
	// 	half1 = numLines/2;					// breakpoint
	// 	pthread_create(&thread1, NULL,
	// 						sortFunct, (void*) /*bundled args*/);
	// }

//
// ================================================================
}

void sortList(int* numList, int numLines)
{
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
}

int main()
{
	int line, numLines, numThreads,			// var to hold value read in, number of lines, and number of threads
	halfPoint, quarterPoint, quarter3Point;	// var hold halfway, quarter way, 3/4 way points		
	int lineNum = 0;						// var to hold line number
	pthread_t thread1, thread2;		// make two threads
	struct storeInfo * sort1;		//
	struct storeInfo * sort2;		// make a struct for each thread

	string filename;						//
	cout << "Enter filename: ";				// get filename from user
	cin >> filename;						//

	ifstream numberFile(filename.c_str());	// convert filename to chars to be opened
	if (!numberFile.good())					//
		cout << "File not found.\n";		// see if file exists

	numberFile >> line;						// take number of lines
	numLines = line;						// numLines = first line
	int numList[numLines];					// make an array of numbers size line

	while (lineNum < numLines)				// while not past the last line
	{
		numberFile >> line;					// read in the line
		numList[lineNum] = line;			// store the line in the next spot in array
		lineNum++;							// move counter to next line
	}

	if (numLines <= 10)						//
		numThreads = 2;						// if there's 10 or less items make 2 threads

	if (numThreads == 2) {					// if you make 2
		// store data for first thread
		sort1->beginSort = 0;				// start at beginning of array
		sort1->endSort = numLines/2;		// end sorting halfway through
		sort1->numList = numList;			// store numberList in struct
		// store data for second thread
		sort2->beginSort = (numLines/2)+1;	// start at second half
		sort2->endSort = numLines;			// end sorting at the end
		sort2->numList = numList;			// store numberList in struct
	}

	sortList(numList, numLines);			// send array to be sorted

	ofstream sortedFile;					//
	sortedFile.open("finalSort.txt");		// open file to write list
	for (int i = 0; i < numLines; i++)		//
		sortedFile << numList[i] << "\n";	// write sorted list to file
	sortedFile.close();						// close file
}