/*
Name: 	 Tyler Kickham
Program: Multithreaded text formatting
Purpose: The purpose of this program is to format text in three steps,
		 each taking place in a separate thread. 1: read/store text
		 2: format text  3: align/display text
Input:   The input is the name of a text file typed in from the keyboard,
	     contains text of <= 200 lines of <= 60 chars
Output:  The output is the text displayed aligned/formatted

Todo: perfect alignment
	  shorten
*/


#include "KickhamOS2.h"
#include <string>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <stdlib.h>
using namespace std;


int maxlength = 60;																// max input line length
int charCounter = 0;															// count number of characters per line
int numWords = 0;																// count number of words per line
int startOverwrite[2];															// where to start read/write in array
char textArray[399][60];														// char array to store line in
bool stillReading = true;														// tell threads if there's more to do
pthread_mutex_t reading, formatting, writing;									// semaphores for reading, formatting, and writing

void * readFile(void *)
// INPUT: none
// OUTPUT: none
// reads file in to character array and tells formatting thread when to format
{
	string line;																// var to hold line read in
	string filename;															// var for filename
	cout << "Enter filename: ";													// prompt for filename
	cin >> filename;															// store input into filename var
	ifstream textFile(filename.c_str());										// filename turned into input stream
	if (!textFile.good())														// if it's not found
	{
		cout << "File not found\n";												// tell user it's not found
		exit(1);																// exit
	}

	int row = 0;																// start at row 0 in array
	while (getline(textFile, line))												// while there is another line of text
	{
		int column = 0;															// start at the beginning of the row/line
		if (line.length() > 0)													// if it's not a blank line
		{
			numWords += 1;														// so don't /0 if there is one word in the line
			while (column < line.length())										// while the column is w/in the length of text
			{
				if (line[column] == ' ')										// if the next char is a space
					numWords += 1;												// increase word count by one
				else															// otherwise
				{
					if (line[column] == '\\')									// if text char is slash
						textArray[row][column] = 'a';							// mark as control sequence (a)
					else														// otherwise
						textArray[row][column] = 'b';							// mark as not a control sequence (b)
					charCounter += 1;											// counter another character
				}
				textArray[row+1][column] = line[column];						// store the char in textArray
				column++;														// go to next column in row
			}
		}
		for (int clear = line.length(); clear < maxlength; clear++)				// for each extra index
		{
			textArray[row][clear] = 'e';										// mark as empty index for writing
			textArray[row+1][clear] = '\0';										// put space char as filler
		}
		row += 2;																// start new row
		pthread_mutex_unlock(&formatting);										// unlock formatting thread
		pthread_mutex_lock(&reading);											// lock reading thread
	}
	pthread_mutex_unlock(&formatting);											// unlock formatting thread for last time
	pthread_mutex_unlock(&writing);												// unlock writing thread for last time
	stillReading = false;														// tell threads to stop when done
}

void * formatText(void *)
// INPUT: none
// OUTPUT: none
// calls formatter at every control sequence, tells writing thread when to write
{
	pthread_mutex_lock(&formatting);											// lock the formatting thread so it can't go too early
	int row = 0;																// var for which row, init at 0

	while (stillReading)
	{
		int readFrom = 0;														// var for index to read from
		int writeHere = 0;														// var for index to write to
		while (readFrom < maxlength)											// while there are more indices
		{
			if (textArray[row][readFrom] == 'a')								// if the character is backslash (marked w/ a)
			{
				formatter(readFrom, writeHere, row+1);							// send indices to read/write and which row of array to be formatted
				writeHere= startOverwrite[0];									// new write to index
				readFrom = startOverwrite[1];									// new read from index
			}
			else																// otherwise
			{
				textArray[row+1][writeHere] = textArray[row+1][readFrom];		// rewrite at next spot of array
				readFrom += 1;													// increase read index
				writeHere += 1;													// increase write index
			}
		}
		row += 2;																// move on to next set of rows
		pthread_mutex_unlock(&writing);											// unlock writing thread
		pthread_mutex_lock(&formatting);										// lock formatting thread
	}
}

void formatter(int location, int locationJ, int row)
// INPUT: int of location to read, location to write, and which row
// OUTPUT: changes int location to write[0] and read[1] in textArray
// performs formatting at current location in textArray
{
	char nextChar = textArray[row][location+1];									// holds next char in array for switch
	charCounter -= 2;															// remove control sequence from character count
	switch (nextChar)															// check which case the nextChar is
	{
		case 'c':																// when nextChar == 'c' capitalize the first char of word
			textArray[row][locationJ] = toupper(textArray[row][location+2]);	// capitalize the char after control sequence
			startOverwrite[0] = locationJ+1;									// increase write location by one and store
			startOverwrite[1] = location+3;										// increase read location by 3 to get past char after control sequence and store
			break;
		case 'C':																// when nextChar == 'C' capitalize the entire word
			location += 2;														// move location to after control sequence
			while (isalpha(textArray[row][location]))							// while location is a letter (means it's the same word)
			{
				textArray[row][locationJ] = toupper(textArray[row][location]);	// capitalize char at location
				location += 1;													// increase read location
				locationJ += 1;													// increase write location
			}
			startOverwrite[0] = locationJ;										// store write location
			startOverwrite[1] = location;										// store read location
			break;
		case 'u':																// when nextChar == 'u' underline entire word
			charCounter += 2;													// add underscores to number of chars
			textArray[row][locationJ] = '_';									// replace the slash with _
			location += 2;														// increase read location to after control sequence
			locationJ += 1;														// increase the write location by one
			while (isalpha(textArray[row][location]))							// while read location is a letter
			{
				textArray[row][locationJ] = textArray[row][location];			// write read location to write location
				location += 1;													// increase read location by one
				locationJ += 1;													// increase write location by one
			}
			textArray[row][locationJ] = '_';									// add _ at end to signify end of underline
			startOverwrite[0] = locationJ+1;									// store write location (+1 to account for _ just written)
			startOverwrite[1] = location;										// store read location
			break;
		default:																// if it isn't one of those
			break;																// given that this will never occur no notification necessary
	}
}

void * writeText(void *)
// INPUT: none
// OUTPUT: formatted and aligned text displayed to console
// right and left align text then display it
{
	pthread_mutex_lock(&writing);												// lock writing so it can't go again
	int row = 1;																// start at row 1
	int numSpaces, spacesToAdd;													// vars for number of spaces total and number of spaces to cout at a time
	int lineLen = 50;															// cout length = 50
	while (stillReading)														// while there is more to be written
	{
		if (textArray[row-1][0] != 'e')											// if it is not an empty line
		{
			numSpaces = lineLen - charCounter;									// total num spaces = length of line - num chars
			spacesToAdd = numSpaces/numWords;									// spaces at a time = total num spaces/num words
			for (int j=0; j<maxlength; j++)										// for 0-maxlength of array
			{
				if (textArray[row][j] == ' ' && numWords > 0)					// if the next char is a space and there are more words left
				{
					for (int space = 0; space < spacesToAdd; space++)			// for 0-spaces at a time
						cout << " ";											// cout a space
					numWords -= 1;												// decrease words left by one
				}
				else															// otherwise
					cout << textArray[row][j];									// cout the char
			}
		}
		charCounter = 0;														// reset char counter
		numWords = 0;															// reset word counter
		cout << endl;															// break the line
		row += 2;																// move on to next row for next time unlocked
		pthread_mutex_unlock(&reading);											// unlock reading thread
		pthread_mutex_lock(&writing);											// lock writing thread
	}
}

int main()
{
	pthread_t thread1, thread2, thread3;										// thread1 = read, thread2 = format, thread3 = write
	pthread_mutex_init(&reading, NULL);											// initialize reading thread
	pthread_mutex_init(&formatting, NULL);										// initialize formatting thread
	pthread_mutex_init(&writing, NULL);											// initialize writing thread

	pthread_mutex_lock(&reading);												// lock reading thread 
	pthread_mutex_lock(&formatting);											// lock formatting thread 
	pthread_mutex_lock(&writing);												// lock writing thread 

	pthread_create(&thread1, NULL, readFile, (void *) NULL);					// create reading thread
	pthread_create(&thread2, NULL, formatText, (void *) NULL);					// create formatting thread
	pthread_create(&thread3, NULL, writeText, (void *) NULL);					// create writing thread

	pthread_join(thread1, NULL);												// wait for all threads to finish
	pthread_join(thread2, NULL);												// wait for all threads to finish
	pthread_join(thread3, NULL);												// wait for all threads to finish

	pthread_mutex_destroy(&reading);											// destroy reading thread
	pthread_mutex_destroy(&formatting);											// destroy formatting thread
	pthread_mutex_destroy(&writing);											// destroy writing thread
}