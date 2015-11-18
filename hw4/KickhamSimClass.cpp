#include "KickhamSimClass.h"
#include <vector>


const int memSize = 1048576;							// size of memory (bytes)


simulation::simulation()
{
	this->memUsed = 0;
	memArray = new int[memSize];
	for (int i = 0; i < memSize; i++)
		this->memArray[i] = 0;
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
	bool freeSpace = false;
	bool placed = false;
	int i = 0;
	int j;
	if (memUsed + newJob->getSize() <= memSize)
	{
		while (!placed and i < memSize)
		{
			if (this->memArray[i] == 0)
			{
				freeSpace = true;
				j = i;
				while (j < i + newJob->getSize() and freeSpace)
				{
					if (memArray[j] != 0)
					{
						i = j;
						freeSpace = false;
					}
					else
						j++;
				}
				if (freeSpace == true)
				{
					placed = true;
					newJob->setStart(i);
					processes.push_back(*newJob);
					cout << memUsed << " + " << newJob->getSize() << " = " << memUsed + newJob->getSize() << endl;
					memUsed += newJob->getSize();
					for (int k = i; k < newJob->getSize(); k++)
						this->memArray[k] = 1;
				}
			}
			i++;
		}
	}
}

void simulation::deallocate(vector<Job> &processes, int PID)
// deallocate process from memory
{
	for (int i = 0; i < processes.size(); i++)
	{
		if (processes.at(i).getPID() == PID)
		{
			int start = processes.at(i).getStart();
			int size = processes.at(i).getSize();
			for (int j = start; j < size + start; j++)
				memArray[j] = 0;
			cout << memUsed << " - " << size << " = " << memUsed - size << endl;
			memUsed -= processes.at(i).getSize();
			processes.erase(processes.begin()+i);
		}
	}
}

int simulation::getUsed(int location)
// return if there is a process there
{
	return this->memArray[location];
}