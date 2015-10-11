/*
Name: 	 Tyler Kickham
Program: Multithreaded text formatting
Purpose: The purpose of this program is to format text in three steps,
		 each taking place in a separate thread. 1: read/store text
		 2: format text  3: align/display text
Input:   The input is the name of a text file typed in from the keyboard,
	     contains text of <= 200 lines of <= 60 chars
Output:  The output is the text displayed aligned/formatted

Todo: format text
	  align
	  display
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

char textArray[200][60];
int startOverwrite[2];	// where to start read/write in array
int linelength = 60;

void readFile()
// INPUT: none
// OUTPUT: none
// reads file in to character array and signals formatText when to start
{
	string line;							// var to hold line read in
	// ask user for file -----------
	string filename;						// var for filename
	cout << "Enter filename: ";				// prompt for filename
	cin >> filename;						// store input into filename var
	// -----------------------------
	ifstream textFile(filename.c_str());	// filename turned into input stream
	if (!textFile.good())					// if it's not found
	{
		cout << "File not found\n";			// tell user it's not found
		exit(1);							// exit
	}

	int row=0;								// iterator for textArray array
	int column=0;
	while (!textFile.eof())			// while there is another line
	{
		while (row < 200)
		{
			while (column < linelength)
			{
				textArray[row][column] << textFile;			// copy it into the array
				column++;
			}
			formatText(row);						// while not threading/signalling
			row++;
		}
		// writeText(i);
	}
}

void formatText(int row)
// INPUT: none
// OUTPUT: none
// formats all control sequences in text segment allowed to work on
// int j is necessary to remove control sequence characters from array
{
	//int i = 0;								// var for iteration
	int column=0;								// var for rewriting in array

	//while (formatLocked);					// semaphore for locking formatText
	while (column < linelength)							// while not done with line (can't for b/c of return)
	{
		if (textArray[row][column] == '\\')			// if the character is backslash
		{
			formatter(row, column);					// send location to be formatted
			column = startOverwrite[0];
			row = startOverwrite[1];
		}
		else
		{
			textArray[row][column] = textArray[row][column];	// rewrite at next spot of array
			row += 1;							// keep track of preformatted iteration
			column += 1;							// keep track of next spot to write to
		}
	}
	writeText();							// replace with a semaphore
}

void formatter(int location, int locationJ)
// INPUT: int of location to read and location to write respectively
// OUTPUT: changes int location to write[0] and read[1] in array
// performs formatting at current location in array
{
	int startingLoc = location;
	char nextChar = textArray[location+1];	// holds next char in array
	switch (nextChar)
	{
		case 'c':
			textArray[location] = toupper(textArray[location+2]);	// capitalize the char after control sequence
			startOverwrite[0] = location+1;
			startOverwrite[1] = location+3;
			break;
		case 'C':
			location += 2;											// start checking after C
			while (isalpha(textArray[location]))								// while location is a letter
			{
				textArray[locationJ] = toupper(textArray[location]);	// capitalize char at location
				textArray[location] = textArray[location+1];
				location += 1;
				locationJ += 1;										// go to next iteration of array
			}
			startOverwrite[0] = locationJ;
			startOverwrite[1] = location;
			break;									// return the location of the first nonletter char
		case 'u':
			textArray[locationJ] = '_';
			location += 2;
			locationJ += 1;
			while (isalpha(textArray[location]))								// while location is a letter
			{
				textArray[locationJ] = textArray[location];		// store in array
				// textArray[location] = textArray[location+1];
				location += 1;
				locationJ += 1;										// go to next iteration of array
			}
			textArray[locationJ] = '_';
			startOverwrite[0] = locationJ+1;
			startOverwrite[1] = location;
			break;
		default:
			break;
	}
}

void writeText()
// INPUT: none
// OUTPUT: formatted and aligned text
// right and left align text then display it
{
	// cout << max << endl;
	for (int j=0; j<linelength; j++)
		cout << textArray[j];

	cout << endl;
}

int main()
{
	// char textArray[100];					// array for lines of text, line of text
	readFile();					// call function to read in file
	//formatText();
	//writeText();					// part 3 of program
}