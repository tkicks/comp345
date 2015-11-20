#include "KickhamSimClass.h"
#include <vector>


const int memSize = 1048576;												// size of memory (bytes)


simulation::simulation()
// INPUT: none
// OUTPUT: none
// constructor for simulation initializes everything to 0
{
	this->memUsed = 0;														// init amount of memory being used to 0
	memArray = new int[memSize];											// create array representing memory (fill with PID of process in space)
	for (int i = 0; i < memSize; i++)										// for each space in memory
		this->memArray[i] = 0;												// default it to empty (0)
}

simulation::~simulation()
{
	// deconstructor
}

void simulation::getFile(vector<Job> &processes, simulation memory)
// INPUT: vector of processes, memory simulation object
// OUTPUT: none
// reads from file and allocates/deallocates processes from memory
{
	Job *newJob;															// create new job object
	vector<int> skippedJobs;												// create a vector for the jobs that get skipped
	string filename;														// string to hold filename
	char op;																// holds the op read in
	int PID, size;															// holds PID and size read in
	cout << "Enter filename: ";												// ask for filename
	cin >> filename;														// receive filename
	ifstream dataFile(filename.c_str());									// convert to string
	if (!dataFile.good())													// if the file doesn't exist
	{
		cout << "File not found\n";											// say not found
		exit(1);															// exit
	}

	do {																	// do following at least once
		dataFile >> op >> PID;												// read in op and PID from file
		if (op == 'A')														// if the op is 'A' there is a size
		{
			dataFile >> size;												// read in size
			newJob = new Job(op, PID, size);								// create new job object
			memory.allocate(newJob, processes, skippedJobs);				// allocate to memory
		}
		else if (op == 'D')													// if op is 'D'
			memory.deallocate(processes, PID);								// deallocate from memory
		else
			break;															// otherwise break
		
	} while (op != 'Q');													// do above while op isn't 'Q'
	
	cout << "Number of skipped processes: " << skippedJobs.size() << endl;	// display number of skipped jobs
	cout << "Skipped processes: ";											//
	for (int i = 0; i < skippedJobs.size(); i++)							// for each process skipped
		cout << skippedJobs[i] << " ";										// display PID
	cout << endl;															//
}

void simulation::allocate(Job *newJob, vector<Job> &processes, vector<int> &skippedJobs)
// INPUT: job object, vector of process objects, vector of skipped processes
// OUTPUT: none
// allocates process to memory if they fit
{
	bool freeSpace = false;													// set free space for process to false
	bool placed = false;													// set process placed in memory to false
	int i = 0;																// set iterator to 0 (not in for so that i can be incremented on demand)
	int j;																	// declare iterator to be used if free space is found (not in for for same as above)
	if (memUsed + newJob->getSize() <= memSize)								// if there is enough free space in memory to fit the process
	{
		while (!placed and i < memSize)										// while placed is false and iterator is less than size of memory
		{
			if (this->memArray[i] == 0)										// if there is free space at current memory location
			{
				freeSpace = true;											// set free space to true
				j = i;														// set second iterator to current location
				while (j < i + newJob->getSize() and freeSpace)				// while second iterator is less than current location+size of process, and there is still free space
				{
					if (memArray[j] != 0)									// if there is not free space in the next spot
					{
						i = j;												// set iterator to current location
						freeSpace = false;									// set free space to false
					}
					else													// otherwise
						j++;												// increase second iterator
				}
				if (freeSpace == true)										// if there is enough free space for process
				{
					placed = true;											// set placed to true
					newJob->setStart(i);									// set the start of the process to the first free space location
					processes.push_back(*newJob);							// push process to back of processes vector
					memUsed += newJob->getSize();							// increase memory being used by size of process
					int PID = newJob->getPID();								// set PID to the PID of process
					for (int k = i; k < newJob->getSize() + i; k++)			// for iterator starting at start, and less than size of process
						this->memArray[k] = PID;							// set memory location to PID
				}
			}
			i++;															// increase iterator
		}
		if (!placed)														// if process never got placed
			skippedJobs.push_back(newJob->getPID());						// push PID to skipped vector
	}
	else																	// if there is not enough space in memory for it
		skippedJobs.push_back(newJob->getPID());							// push PID to skipped vector
}

void simulation::deallocate(vector<Job> &processes, int PID)
// INPUT: vector of processes, PID
// OUTPUT: none
// deallocate process from memory
{
	for (int i = 0; i < processes.size(); i++)								// for each process in memory
	{
		if (processes.at(i).getPID() == PID)								// if the process's PID equals one being deallocated
		{
			int start = processes.at(i).getStart();							// set start to the start of its location in memory
			int size = processes.at(i).getSize();							// set size to its size
			for (int j = start; j < size + start; j++)						// for each space in memory from start to end
				memArray[j] = 0;											// set memory to empty
			memUsed -= processes.at(i).getSize();							// subtract size from memory being used
			processes.erase(processes.begin()+i);							// erase process from list of processes
		}
	}
}

void simulation::getTable(vector<Job> &processes, simulation memory)
// INPUT: vector of processes, simulation memory
// OUTPUT: none
// gets the data for the table and calls for it to be written
{
	int contSpace = 0;														// init number of contiguous blocks to 0
	cout.width(23);															//
	cout << "Memory Table\n";												// display table title
	cout << "PID";															// display header
	cout.width(10);															//
	cout << "Begin";														// display header
	cout.width(10);															//
	cout << "End";															// display header
	cout.width(10);															//
	cout << "Size\n";														// display header

	int i = 0;																// set iterator to 0 (not for so can be manipulated in loop)
	int j, k, pid, start, end, size;										// init 2 iterators, pid, start, end and size
	while (i < memSize)														// while iterator is less than size of memory
	{
		bool found = false;													// init if process found in memory to false
		if (this->memArray[i] == 0)											// if memory location is empty
		{
			pid = 0;														// set pid to default empty
			start = i;														// set start of block to current location
			j = i;															// set second iterator to current location
			while (memArray[j] == 0 and j < memSize)						// while memory location is empty and has not reached end of available memory
				j++;														// increase second iterator
			end = j - 1;													// set end to second iterator - 1
			size = end - start + 1;											// set size to the end - beginning -1
			memory.writeTable(pid, start, end, size);						// call to write to table
			contSpace += 1;													// increase number of contiguous spaces
			i = j;															// set first iterator to current location
		}
		else																// otherwise memory is being used
		{
			pid = this->memArray[i];										// set pid to PID (set as int at location)
			k = 0;															// init third iterator to 0
			while (k < processes.size() and !found)							// while iterator less than 0 and not found in process list
			{
				if (processes.at(k).getPID() == pid)						// if the process in vector equals the PID
				{
					start = processes.at(k).getStart();						// set start to start
					size = processes.at(k).getSize();						// set size to size
					end = start + size - 1;									// set end to start + size - 1
					memory.writeTable(pid, start, end, size);				// call write table
					i = i + size;											// set iterator to end of process location
					found = true;											// set found to true
				}
				k++;														// increase second iterator
			}
		}
	}
	cout << endl;															// put space between table and rest of data
	memory.writeData(processes, memory, contSpace);							// call to display the rest of data
}

void simulation::writeTable(int pid, int start, int end, int size)
// INPUT: pid, start location, end location, size
// OUTPUT: none
// displays memory table to user
{
	if (pid != 0)															// if memory isn't empty
		cout << right << setw(4) << pid;									// display the PID
	else																	// otherwise
		cout << right << setw(4) << "Free";									// display memory is free
	cout.width(10);															//
	cout << start;															// display start
	cout.width(10);															//
	cout << end;															// display end
	cout.width(10);															//
	cout << size << endl;													// display size
}

void simulation::writeData(vector<Job> &processes, simulation memory, int contSpace)
// INPUT: vector of processes, simulation of memory, number of contiguous spaces
// OUTPUT: none
// write the rest of the data
{
	// get size of used mem and by contrast free mem
	int usedMem = 0;														// init used memory to 0
	for (int i = 0; i < processes.size(); i++)								// for each process
	{
		if (processes.at(i).getPID() != 0)									// if the space isn't free
			usedMem += processes.at(i).getSize();							// get the size of memory being used
	}
	int freeMem = memSize - usedMem;										// set free memory to free memory
	// ---------------------------------------------
	cout << "Memory in use: " << usedMem << " bytes\n";							// display memory being used
	cout << "Memory remaining: " << freeMem << " bytes\n";						// display memory free
	cout << "Number of contiguous blocks: " << contSpace << endl;				// display number of contiguous blocks
	cout << "Avg size contiguous blocks: " << freeMem/contSpace << " bytes\n";	// display avg size of contiguous blocks
}