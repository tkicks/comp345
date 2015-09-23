/*
TODO: change insertion sort to merge sort
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
	int half;						// which half/quarter is running
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
	else if (localSorting->half == 2)
		sortedFile.open("sorted2.txt");		// open file to write list
	else if (localSorting->half == 3)
		sortedFile.open("sorted3.txt");		// open file to write list
	else
		sortedFile.open("sorted4.txt");		// open file to write list
	for (int i = localSorting->beginSort; i < localSorting->endSort; i++)
		sortedFile << localSorting->numList[i] << "\n";
	sortedFile.close();						// close file
}


// void MergeSort(void * original, int size)
// // original = numList array
// {
// 	struct storeInfo * localSort = (struct storeInfo *) original;
// 	if (size > 1)
// 	{
// 		// allocate arrays
// 		int numFloor = (size/2);
// 		int numCeiling = size - numFloor;
// 		int Left[numFloor];
// 		int Right[numCeiling];

// 		// copy to left (B) and right (C)
// 		for (int i = 0; i < numFloor; i++)
// 		{
// 			Left[i] = localSort->numList[i];
// 		}
// 		for (int j = numFloor, k = 0; j < size; j++, k++)
// 		{
// 			Right[k] = localSort->numList[j];
// 		}

// 		// mergesort Left then Right
// 		MergeSort(Left, numFloor);
// 		MergeSort(Right, numCeiling);
// 		Merge(Left, Right, (void *) localSort, numFloor, numCeiling);
// 	}
// }

// void Merge(int* Left, int* Right, void * original, int sizeL, int sizeR)
// {
// 	cout << "into merge\n";
// 	int i = 0;	// how far in Left
// 	int j = 0;	// how far in Right
// 	int k = 0;	// how far in original
// 	struct storeInfo * localSort = (struct storeInfo *) original;

// 	// while loop
// 	while ((i < sizeL) and (j < sizeR))
// 	{
// 		if (Left[i] <= Right[j])
// 		{
// 			localSort->numList[k] = Left[i];
// 			i++;
// 		}
// 		else
// 		{
// 			localSort->numList[k] = Right[j];
// 			j++;
// 		}
// 		k++;
// 	}
// 	if (i == sizeL)
// 	{
// 		for (; j < sizeR; j++, k++)
// 		{
// 			localSort->numList[k] = Right[j];
// 		}
// 	}
// 	else
// 	{
// 		for (; i < sizeL; i++, k++)
// 		{
// 			localSort->numList[k] = Left[i];
// 		}
// 	}
// }


int main()
{
	int line, numLines, numThreads,			// var to hold value read in, number of lines, and number of threads
	halfPoint, quarterPoint, quarter3Point;	// var hold halfway, quarter way, 3/4 way points		
	int lineNum = 0;						// var to hold line number
	pthread_t thread1, thread2,				//
			  thread3, thread4;				// make four threads
	struct storeInfo * sort1;				//
	struct storeInfo * sort2;				//
	struct storeInfo * sort3;				//
	struct storeInfo * sort4;				// make a struct for each thread
	sort1 = new storeInfo;					// first quarter sort thread
	sort2 = new storeInfo;					// second quarter sort thread
	sort3 = new storeInfo;					// third quarter sort thread
	sort4 = new storeInfo;					// fourth quarter sort thread

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

	if (numLines <= 20000)					//
		numThreads = 2;						// if there's 10 or less items make 2 threads
	else
		numThreads = 4;

	if (numThreads == 2)
	{					// if you make 2
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


		pthread_create(&thread1, NULL,
			sortList, (void *) sort1);			// send first half to be sorted
		pthread_create(&thread2, NULL,
			sortList, (void *) sort2);			// send second half to be sorted
		pthread_join(thread1, NULL);			// let thread1 finish
		pthread_join(thread2, NULL);			// let thread2 finish
	}
	else
	{
		// store data for first thread
		sort1->beginSort = 0;				// start at beginning of array
		sort1->endSort = numLines/4;		// end sorting quarter through
		sort1->numList = numList;			// store numberList in struct
		sort1->half = 1;					// store as first half
		// store data for second thread
		sort2->beginSort = ((numLines/4));	// start at second half
		sort2->endSort = (numLines/2);		// end sorting at the half
		sort2->numList = numList;			// store numberList in struct
		sort2->half = 2;					// store as second half
		// store data for third thread
		sort3->beginSort = (numLines/2);				// start at beginning of array
		sort3->endSort = (numLines-(numLines/4));		// end sorting 3/4 through
		sort3->numList = numList;			// store numberList in struct
		sort3->half = 3;					// store as first half
		// store data for fourth thread
		sort4->beginSort = (numLines-(numLines/4));	// start at second half
		sort4->endSort = numLines;		// end sorting at the end
		sort4->numList = numList;			// store numberList in struct
		sort4->half = 4;					// store as second half

		pthread_create(&thread1, NULL,
			sortList, (void *) sort1);			// send first half to be sorted
		pthread_create(&thread2, NULL,
			sortList, (void *) sort2);			// send second half to be sorted
		pthread_create(&thread3, NULL,
			sortList, (void *) sort3);			// send first half to be sorted
		pthread_create(&thread4, NULL,
			sortList, (void *) sort4);			// send second half to be sorted
		pthread_join(thread1, NULL);			// let thread1 finish
		pthread_join(thread2, NULL);			// let thread2 finish
		pthread_join(thread3, NULL);			// let thread1 finish
		pthread_join(thread4, NULL);			// let thread2 finish
	}

	// =====================================================================
	// INSERTION SORT final sorted array
	int j, k;									// temp vars to hold values/iterators
	for (int i = 1; i < numLines; i++)			// while still lines to sort
	{
		j = numList[i];							// temp j = current value
		k = i-1;								// temp k is iteration-1
		while ((k >= 0) and (numList[k] > j))	// while val of k is > current
		{
			numList[k+1] = numList[k];			// move down through list
			k--;								// subtract 1 from where k ends up
		}
		numList[k+1] = j;						// make 1 past k = starting temp val
	}
	//
	// =====================================================================

	// =====================================================================
	// MERGE SORT final sorted array
	// struct storeInfo * finalSort;			// make a struct for each thread
	// finalSort = new storeInfo;				//
	// finalSort->numList = numList;			// add numList to finalSort struct
	// MergeSort((void *) finalSort, numLines);
	//
	// =====================================================================
	
	ofstream sortedFile;					//
	sortedFile.open("finalAnswer.txt");		// open file to write list
	for (int i = 0; i < numLines; i++)		//
		sortedFile << numList[i] << "\n";	// write sorted list to file
	sortedFile.close();						// close file
}