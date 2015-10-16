/*
Name: 	 Tyler Kickham
Program: Multithreaded text formatting (OS2)
Purpose: Use semaphores to allow thread 1: read/store text  2: format text  3: align/display text
Input:   The input is the name of a text file typed in from the keyboard which contains text of <= 200 lines of <= 60 chars
Output:  The output is the text displayed aligned/formatted
*/

#include "KickhamOS2.h"
#include <string>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <stdlib.h>
using namespace std;

int lineLen = 50;																// cout length = 50
int maxlength = 60;																// max input line length
int charCounter, numWords;														// count number of characters per line, words per line
int startOverwrite[2];															// where to start read/write in array
char textArray[200][60];														// char array to store line in
bool stillReading = true;														// tell threads if there's more to do
pthread_mutex_t reading, formatting, writing;									// semaphores for reading, formatting, and writing

void * readFile(void *)
// INPUT: none
// OUTPUT: none
// reads file in to character array and unlocks formatting thread
{
	string line, filename;														// var to hold line read in, filename
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
				else
					charCounter++;												// add a character to the counter used to determine num spaces
				textArray[row][column] = line[column];							// store the char in textArray
				column++;														// go to next column in row
			}
		}
		for (int clear = line.length(); clear < maxlength; clear++)				// for each extra index
		{
			textArray[row][clear] = '\0';										// put null char as filler
		}
		row += 1;																// start new row
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
// calls formatter at every control sequence, unlocks writing thread
{
	pthread_mutex_lock(&formatting);											// lock the formatting thread so it can't go too early
	int row = 0;																// var for which row, init at 0

	while (stillReading)														// while there is still more to format
	{
		int readFrom = 0;														// start reading at index 0
		int writeHere = 0;														// start writing at index 0
		while (readFrom < maxlength)											// while there are more indices
		{
			if (textArray[row][readFrom] == '\\')								// if the character is backslash (marked w/ a)
			{
				formatter(readFrom, writeHere, row);							// send indices to read/write and which row of array to be formatted
				writeHere= startOverwrite[0];									// new write to index
				readFrom = startOverwrite[1];									// new read from index
			}
			else																// otherwise
			{
				textArray[row][writeHere] = textArray[row][readFrom];			// rewrite at next spot of array
				readFrom += 1;													// increase read index
				writeHere += 1;													// increase write index
			}
		}
		row += 1;																// move on to next set of rows
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
	location += 2;																// move location to after control sequence
	switch (nextChar)															// check which case the nextChar is
	{
		case 'c':																// when nextChar == 'c' capitalize the first char of word
			textArray[row][locationJ] = toupper(textArray[row][location]);		// capitalize the char after control sequence
			location += 1;														// increase read location
			locationJ += 1;														// increase write location
			break;
		case 'C':																// when nextChar == 'C' capitalize the entire word
			while (isalpha(textArray[row][location]))							// while location is a letter (means it's the same word)
			{
				textArray[row][locationJ] = toupper(textArray[row][location]);	// capitalize char at location
				location += 1;													// increase read location
				locationJ += 1;													// increase write location
			}
			break;
		case 'u':																// when nextChar == 'u' underline entire word
			charCounter += 2;													// add underscores to number of chars
			textArray[row][locationJ] = '_';									// replace the slash with _
			locationJ += 1;														// increase the write location by one
			while (isalpha(textArray[row][location]))							// while read location is a letter
			{
				textArray[row][locationJ] = textArray[row][location];			// write read location to write location
				location += 1;													// increase read location by one
				locationJ += 1;													// increase write location by one
			}
			textArray[row][locationJ] = '_';									// add _ at end to signify end of underline
			locationJ += 1;														// move write location to next index
			break;
	}
	startOverwrite[0] = locationJ;												// store write location
	startOverwrite[1] = location;												// store read location
}

void * writeText(void *)
// INPUT: none
// OUTPUT: formatted and aligned text displayed to console
// right and left align text by adding equal amounts of spaces between words w/ line length 50 then display it
{
	pthread_mutex_lock(&writing);												// lock writing so it can't go again
	int row = 0;																// start at row 1
	int spacesToAdd;															// vars for number of spaces total and number of spaces to cout at a time
	while (stillReading)														// while there is more to be written
	{
		if (textArray[row][0] != '\0')											// if it is not an empty line
		{
			spacesToAdd = (lineLen - charCounter)/numWords;						// spaces at a time = (length of line - num chars)/num words
			for (int j=0; j<lineLen; j++)										// for 0-maxlength of array
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
		row += 1;																// move on to next row for next time unlocked
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