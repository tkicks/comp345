/*
Name: 	 Tyler Kickham
Program: Multithreaded sort
Purpose: The purpose of this program is to sort a large list of
		 numbers on multiple threads
Input:   The input is the name of a text file typed in from the keyboard
Output:  The output is five text files, one for each of the threads' sort
		 and one that has the final, sorted list.
Todo:	 Put merge into function
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
// INPUT: struct of info for segment to sort
// OUTPUT: none
// sorts segment of list
{
	struct storeInfo * localSorting = (struct storeInfo *) segment;				// init struct in this function
	int temp;																	// hold temp value

	for (int i = localSorting->beginSort; i < localSorting->endSort; i++)		// for each value in segment
	{
		for (int x = localSorting->beginSort; x < localSorting->endSort-1; x++)	// for each value in segment -1
		{
			if (localSorting->numList[x] > localSorting->numList[x+1])			// if current value > next value
			{
				temp = localSorting->numList[x+1];								// store next temporarily
				localSorting->numList[x+1] = localSorting->numList[x];			// put current into next value
				localSorting->numList[x] = temp;								// put smaller number before larger from temp
			}
		}
	}

	ofstream sortedFile;														//
	if (localSorting->half == 1)
		sortedFile.open("sorted1.txt");											// open file to write list
	else if (localSorting->half == 2)
		sortedFile.open("sorted2.txt");											// open file to write list
	else if (localSorting->half == 3)
		sortedFile.open("sorted3.txt");											// open file to write list
	else
		sortedFile.open("sorted4.txt");											// open file to write list
	for (int i = localSorting->beginSort; i < localSorting->endSort; i++)		// for each value in segment
		sortedFile << localSorting->numList[i] << "\n";							// write to file
	sortedFile.close();															// close file

	// tell user what was sorted and saved
	cout << "Finished sorting portion " << localSorting->half << " of list. Saved as: sorted" << localSorting->half << ".txt\n";
}

int main()
{
	int line, numLines, numThreads,					// var to hold value read in, number of lines, and number of threads
	halfPoint, quarterPoint, quarter3Point;			// var hold halfway, quarter way, 3/4 way points		
	int lineNum = 0;								// var to hold line number
	pthread_t thread1, thread2,						//
			  thread3, thread4;						// make four threads
	struct storeInfo * sort1;						//
	struct storeInfo * sort2;						//
	struct storeInfo * sort3;						//
	struct storeInfo * sort4;						// make a struct for each thread
	sort1 = new storeInfo;							// first quarter sort thread
	sort2 = new storeInfo;							// second quarter sort thread
	sort3 = new storeInfo;							// third quarter sort thread
	sort4 = new storeInfo;							// fourth quarter sort thread

	string filename;								//
	cout << "Enter filename: ";						// get filename from user
	cin >> filename;								//

	ifstream numberFile(filename.c_str());			// convert filename to chars to be opened
	if (!numberFile.good())							//
		cout << "File not found.\n";				// see if file exists

	numberFile >> line;								// take number of lines
	numLines = line;								// numLines = first line
	int numList[numLines];							// make an array of numbers size line

	while (lineNum < numLines)						// while not past the last line
	{
		numberFile >> line;							// read in the line
		numList[lineNum] = line;					// store the line in the next spot in array
		lineNum++;									// move counter to next line
	}

	if (numLines <= 20000)							//
		numThreads = 2;								// if there's 20000 or less items make 2 threads
	else											//
		numThreads = 4;								// if there's more make 4 threads

	if (numThreads == 2)							// if you make 2
	{
		// store data for first thread
		sort1->beginSort = 0;						// start at beginning of array
		sort1->endSort = numLines/2;				// end sorting halfway through
		sort1->numList = numList;					// store numberList in struct
		sort1->half = 1;							// store as first half
		// store data for second thread
		sort2->beginSort = ((numLines/2));			// start at second half
		sort2->endSort = numLines;					// end sorting at the end
		sort2->numList = numList;					// store numberList in struct
		sort2->half = 2;							// store as second half


		pthread_create(&thread1, NULL,
			sortList, (void *) sort1);				// send first half to be sorted
		pthread_create(&thread2, NULL,
			sortList, (void *) sort2);				// send second half to be sorted
		pthread_join(thread1, NULL);				// let thread1 finish
		pthread_join(thread2, NULL);				// let thread2 finish
	}
	else											// if there's 4 threads
	{
		// store data for first thread
		sort1->beginSort = 0;						// start at beginning of array
		sort1->endSort = numLines/4;				// end sorting quarter through
		sort1->numList = numList;					// store numberList in struct
		sort1->half = 1;							// store as first half
		// store data for second thread
		sort2->beginSort = ((numLines/4));			// start at second half
		sort2->endSort = (numLines/2);				// end sorting at the half
		sort2->numList = numList;					// store numberList in struct
		sort2->half = 2;							// store as second half
		// store data for third thread
		sort3->beginSort = (numLines/2);			// start at beginning of array
		sort3->endSort = (numLines-(numLines/4));	// end sorting 3/4 through
		sort3->numList = numList;					// store numberList in struct
		sort3->half = 3;							// store as first half
		// store data for fourth thread
		sort4->beginSort = (numLines-(numLines/4));	// start at second half
		sort4->endSort = numLines;					// end sorting at the end
		sort4->numList = numList;					// store numberList in struct
		sort4->half = 4;							// store as second half

		pthread_create(&thread1, NULL,
			sortList, (void *) sort1);				// send first half to be sorted
		pthread_create(&thread2, NULL,
			sortList, (void *) sort2);				// send second half to be sorted
		pthread_create(&thread3, NULL,
			sortList, (void *) sort3);				// send first half to be sorted
		pthread_create(&thread4, NULL,
			sortList, (void *) sort4);				// send second half to be sorted
		pthread_join(thread1, NULL);				// let thread1 finish
		pthread_join(thread2, NULL);				// let thread2 finish
		pthread_join(thread3, NULL);				// let thread3 finish
		pthread_join(thread4, NULL);				// let thread4 finish
	}

	int finalList[numLines];						// array w/ final sorted list
	if (numThreads == 2)							// if 2 threads
	{
		int i = 0;									// start from left at 0
		int j = numLines/2;							// start from right at half
		int k = 0;									// how far in final list
		while ((i < numLines/2) and (j < numLines))	// while i < half and j < numLines
		{
			if (numList[i] <= numList[j])			// if the left number <= right number
			{
				finalList[k] = numList[i];			// finalList gets left number
				i++;								// go to next number on left
			}
			else									// if right < left
			{
				finalList[k] = numList[j];			// finalList gets right number
				j++;								// go to next number on right
			}
			k++;									// go to next spot of finalList
		}
		if (i == numLines/2)						// if left side finishes
		{
			for (; j < numLines; j++, k++)			// for each value on right until done
			{
				finalList[k] = numList[j];			// finalList gets that value
			}
		}
		else										// if right side finishes
		{
			for (; i < numLines/2; i++, k++)		// for each value on left until done
			{
				finalList[k] = numList[i];			// finalList gets that value
			}
		}
	}
	else											// if 4 threads
	{
		int half = numLines/2;						// half = halfway point
		int quarter = numLines/4;					// quarter = quarterway point
		int halfList[half];							// make array for first half sorted
		int quarter3List[3*quarter];				// make array for 3/4 of list sorted
		int i = 0;									// start from left at 0
		int j = quarter;							// start from right at quarter
		int k = 0;									// start halfway array at 0
		while ((i < quarter) and (j < half))		// while left hasn't reached quarter way and j hasn't reached halfway
		{
			if (numList[i] <= numList[j])			// if the left <= right
			{
				halfList[k] = numList[i];			// half sorted list gets left val
				i++;								// go to next val on left
			}
			else									// if right < left
			{
				halfList[k] = numList[j];			// half sorted list gets right val
				j++;								// go to next val on right
			}
			k++;									// go to next half sorted spot
		}
		if (i == quarter)							// if left finishes first
		{
			for (; j < numLines; j++, k++)			// for each val on right until done
			{
				halfList[k] = numList[j];			// half sorted list gets right val
			}
		}
		else										// if right finishes first
		{
			for (; i < quarter; i++, k++)			// for each val on left until done
			{
				halfList[k] = numList[i];			// half sorted list gets left val
			}
		}
		i = 0;										// reset left to index 0
		j = half;									// set right val to halfway point
		k = 0;										// reset new array val to 0
		while ((i < half) and (j < (3*quarter)))	// while left is under half and right is under 3/4 through
		{
			if (halfList[i] <= numList[j])			// if left <= right val
			{
				quarter3List[k] = halfList[i];		// 3/4 sorted list gets left val
				i++;								// go to next spot on left
			}
			else									// if right < left
			{
				quarter3List[k] = numList[j];		// 3/4 sorted list gets right val
				j++;								// go to next spot on right
			}
			k++;									// go to next spot in 3/4 list
		}
		if (i == half)								// if left finishes first
		{
			for (; j < 3*quarter; j++, k++)			// for each val on right
			{
				quarter3List[k] = numList[j];		// put into 3/4 sorted list
			}
		}
		else										// if right finishes first
		{
			for (; i < half; i++, k++)				// for each val on left
			{
				quarter3List[k] = halfList[i];		// put into 3/4 sorted list
			}
		}
		i = 0;										// reset left to index 0
		j = numLines-quarter;						// set right to start at 3/4 point
		k = 0;										// set finalList to start at 0
		while ((i < (numLines-quarter)) and (j < numLines))		// while left isn't 3/4 and right isn't done
		{
			if (quarter3List[i] <= numList[j])		// if left <= right
			{
				finalList[k] = quarter3List[i];		// finalList gets left val
				i++;								// go to next spot on left
			}
			else									// if right < left
			{
				finalList[k] = numList[j];			// finalList gets right val
				j++;								// go to next spot on right
			}
			k++;									// go to next spot in finalList
		}
		if (i == (numLines-quarter))				// if left finishes first
		{
			for (; j < numLines; j++, k++)			// for each val on right
			{
				finalList[k] = numList[j];			// finalList gets that val
			}
		}
		else										// if right finishes first
		{
			for (; i < (numLines-quarter); i++, k++)// for each val on left
			{
				finalList[k] = quarter3List[i];		// put into finalList
			}
		}
	}

	ofstream sortedFile;										//
	sortedFile.open("finalAnswer.txt");							// open file to write list
	for (int i = 0; i < numLines; i++)							//
		sortedFile << finalList[i] << "\n";						// write sorted list to file
	sortedFile.close();											// close file

	cout << "Sorted file saved as: finalAnswer.txt\n";			// let user know finished
}