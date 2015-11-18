#include "KickhamSimClass.h"
#include <vector>


const int memSize = 1048576;							// size of memory (bytes)


simulation::simulation()
{
	this->memUsed = 0;
}

simulation::~simulation()
{
	// deconstructor
}

void simulation::getFile(vector<Job> &processes, simulation memory)
// read input file
{
	Job *newJob;
	string filename;
	char op;
	int PID, size;
	cout << "Enter filename: ";
	cin >> filename;
	ifstream dataFile(filename.c_str());
	if (!dataFile.good())
	{
		cout << "File not found\n";
		exit(1);
	}

	do {
		dataFile >> op >> PID;
		if (op == 'A')
		{
			dataFile >> size;
			newJob = new Job(op, PID, size);
			memory.allocate(newJob, processes);
		}
		else if (op == 'D')
			memory.deallocate(processes, PID);
		else
			break;
		
	} while (op != 'Q');
}

void simulation::allocate(Job *newJob, vector<Job> &processes)
// allocate process to memory if enough space
{
	if (memUsed + newJob->getSize() <= memSize)
	{
		processes.push_back(*newJob);
		cout << memUsed << " + " << newJob->getSize() << " = " << memUsed + newJob->getSize() << endl;
		memUsed += newJob->getSize();
	}
}

void simulation::deallocate(vector<Job> &processes, int PID)
// deallocate process from memory
{
	for (int i = 0; i < processes.size(); i++)
	{
		if (processes.at(i).getPID() == PID)
		{
			cout << memUsed << " - " << processes.at(i).getSize() << " = " << memUsed - processes.at(i).getSize() << endl;
			memUsed -= processes.at(i).getSize();
			processes.erase(processes.begin()+i);
		}
	}
}