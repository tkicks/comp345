/*
Name: 	 Tyler Kickham
Program: Multithreaded text formatting
Purpose: The purpose of this program is to format text in three steps,
		 each taking place in a separate thread. 1: read/store text
		 2: format text  3: align/display text
Input:   The input is the name of a text file typed in from the keyboard,
	     contains text of <= 200 lines of <= 60 chars
Output:  The output is the text displayed aligned/formatted

Todo: align
	  thread
*/


#include "KickhamOS2.h"
#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
using namespace std;

int startOverwrite[2];	// where to start read/write in array
int maxlength = 60;
char textArray[1][60];

void readFile()
// INPUT: none
// OUTPUT: none
// reads file in to character array and signals formatText when to start
{
	string line;							// var to hold line read in
	string filename;						// var for filename
	cout << "Enter filename: ";				// prompt for filename
	cin >> filename;						// store input into filename var
	ifstream textFile(filename.c_str());	// filename turned into input stream
	if (!textFile.good())					// if it's not found
	{
		cout << "File not found\n";			// tell user it's not found
		exit(1);							// exit
	}

	while (getline(textFile, line))			// while there is another line
	{
		int column = 0;
		if (line.length() > 0)
		{
			while (column < line.length())
			{
				if (line[column] == '\\')
					textArray[0][column] = 'a';
				else
					textArray[0][column] = 'b';
				textArray[1][column] = line[column];
				column++;
			}
		}
		for (int clear = line.length(); clear < maxlength; clear++)
		{
			textArray[0][clear] = 'b';
			textArray[1][clear] = ' ';
		}
		formatText();						// while not threading/signalling
		// writeText();
	}
}

void formatText()
// INPUT: none
// OUTPUT: none
// formats all control sequences in text segment allowed to work on
// int j is necessary to remove control sequence characters from array
{
	int readFrom=0;								// var for rewriting in array
	int writeHere=0;

	//while (formatLocked);					// semaphore for locking formatText
	while (readFrom < maxlength)							// while not done with line (can't for b/c of return)
	{
		if (textArray[0][readFrom] == 'a')			// if the character is backslash
		{
			formatter(readFrom, writeHere);					// send location to be formatted
			writeHere= startOverwrite[0];
			readFrom = startOverwrite[1];
		}
		else
		{
			textArray[1][writeHere] = textArray[1][readFrom];	// rewrite at next spot of array
			readFrom += 1;							// keep track of preformatted iteration
			writeHere += 1;							// keep track of next spot to write to
		}
		// cout << textArray[1][writeHere-1];
	}
	// cout << endl;
	writeText();							// replace with a semaphore
}

void formatter(int location, int locationJ)
// INPUT: int of location to read and location to write respectively
// OUTPUT: changes int location to write[0] and read[1] in textArray
// performs formatting at current location in textArray
{
	char nextChar = textArray[1][location+1];								// holds next char in array for switch
	switch (nextChar)														// check which case the nextChar is
	{
		case 'c':															// when nextChar == 'c' capitalize the first char of word
			textArray[1][locationJ] = toupper(textArray[1][location+2]);	// capitalize the char after control sequence
			startOverwrite[0] = locationJ+1;								// increase write location by one and store
			startOverwrite[1] = location+3;									// increase read location by 3 to get past char after control sequence and store
			break;
		case 'C':															// when nextChar == 'C' capitalize the entire word
			location += 2;													// move location to after control sequence
			while (isalpha(textArray[1][location]))							// while location is a letter (means it's the same word)
			{
				textArray[1][locationJ] = toupper(textArray[1][location]);	// capitalize char at location
				location += 1;												// increase read location
				locationJ += 1;												// increase write location
			}
			startOverwrite[0] = locationJ;									// store write location
			startOverwrite[1] = location;									// store read location
			break;
		case 'u':															// when nextChar == 'u' underline entire word
			textArray[1][locationJ] = '_';									// replace the slash with _
			location += 2;													// increase read location to after control sequence
			locationJ += 1;													// increase the write location by one
			while (isalpha(textArray[1][location]))							// while read location is a letter
			{
				textArray[1][locationJ] = textArray[1][location];			// write read location to write location
				location += 1;												// increase read location by one
				locationJ += 1;												// increase write location by one
			}
			textArray[1][locationJ] = '_';									// add _ at end to signify end of underline
			startOverwrite[0] = locationJ+1;								// store write location (+1 to account for _ just written)
			startOverwrite[1] = location;									// store read location
			break;
		default:															// if it isn't one of those
			break;															// given that this will never occur no notification necessary
	}
}

void writeText()
// INPUT: none
// OUTPUT: formatted and aligned text
// right and left align text then display it
{
	for (int j=0; j<maxlength; j++)
		cout << textArray[1][j];

	cout << endl;
}

int main()
{
	// char textArray[100];					// array for lines of text, line of text
	readFile();					// call function to read in file
	//formatText();
	//writeText();					// part 3 of program
}