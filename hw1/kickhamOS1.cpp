/*
TODO: merge lists
	  make it handle up to 4 threads

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

void * sortList(void * segment)
{
	struct storeInfo * localSorting = (struct storeInfo *) segment;
	int temp;

	for (int i = localSorting->beginSort; i < localSorting->endSort; i++)
	{
		for (int x = localSorting->beginSort; x < localSorting->endSort-1; x++)
		{
			if (localSorting->numList[x] > localSorting->numList[x+1])
			{
				temp = localSorting->numList[x+1];
				localSorting->numList[x+1] = localSorting->numList[x];
				localSorting->numList[x] = temp;
			}
		}
	}

	ofstream sortedFile;					//
	if (localSorting->half == 1)
		sortedFile.open("sorted1.txt");		// open file to write list
	else
		sortedFile.open("sorted2.txt");		// open file to write list
	sortedFile.close();						// close file
}

int main()
{
	int line, numLines, numThreads,			// var to hold value read in, number of lines, and number of threads
	halfPoint, quarterPoint, quarter3Point;	// var hold halfway, quarter way, 3/4 way points		
	int lineNum = 0;						// var to hold line number
	pthread_t thread1, thread2;		// make two threads
	struct storeInfo * sort1;		//
	struct storeInfo * sort2;		// make a struct for each thread
	sort1 = new storeInfo;			//
	sort2 = new storeInfo;			// distinguish the two

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

	if (numLines <= 1000000)				//
		numThreads = 2;						// if there's 10 or less items make 2 threads

	if (numThreads == 2) {					// if you make 2
		// store data for first thread
		sort1->beginSort = 0;				// start at beginning of array
		sort1->endSort = numLines/2;		// end sorting halfway through
		sort1->numList = numList;			// store numberList in struct
		sort1->half = 1;					// store as first half
		// store data for second thread
		sort2->beginSort = ((numLines/2));	// start at second half
		sort2->endSort = numLines;			// end sorting at the end
		sort2->numList = numList;			// store numberList in struct
		sort2->half = 2;					// store as second half
	}

	pthread_create(&thread1, NULL, sortList, (void *) sort1);			// send first half to be sorted
	pthread_create(&thread2, NULL, sortList, (void *) sort2);			// send second half to be sorted
	pthread_join(thread1, NULL);										// let thread1 finish
	pthread_join(thread2, NULL);										// let thread2 finish

	
	ofstream sortedFile;					//
	sortedFile.open("finalAnswer.txt");		// open file to write list
	for (int i = 0; i < numLines; i++)		//
		sortedFile << numList[i] << "\n";	// write sorted list to file
	sortedFile.close();						// close file
}