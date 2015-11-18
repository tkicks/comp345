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
					cout << newJob->getPID() << " starts at: " << i << " :: ends at: " << i + newJob->getSize() - 1 << " :: size: " << newJob->getSize() << endl;
					memUsed += newJob->getSize();
					int PID = newJob->getPID();
					for (int k = i; k < newJob->getSize() + i; k++)
						this->memArray[k] = PID;
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
			cout << processes.at(i).getPID() << " removed from: " << processes.at(i).getStart() << " -> " << processes.at(i).getStart() + processes.at(i).getSize() - 1 << " :: size: " << size << endl;
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

void simulation::getTable(std::vector<Job> &processes, simulation memory)
// write table
{
	cout.width(23);
	cout << "Memory Table\n";
	cout << "PID";
	cout.width(10);
	cout << "Begin";
	cout.width(10);
	cout << "End";
	cout.width(10);
	cout << "Size\n";

	// test vector (working)
	int i = 0;
	int j, k, pid, start, end, size;
	while (i < memSize)
	{
		bool found = false;
		if (this->memArray[i] == 0)
		{
			pid = 0;
			start = i;
			j = i;
			while (memArray[j] == 0 and j < memSize)
				j++;
			end = j - 1;
			size = end - start + 1;
			memory.writeTable(pid, start, end, size);
			i = j;
		}
		else
		{
			pid = this->memArray[i];
			k = 0;
			while (k < processes.size() and !found)
			{
				if (processes.at(k).getPID() == pid)
				{
					start = processes.at(k).getStart();
					size = processes.at(k).getSize();
					end = start + size - 1;
					memory.writeTable(pid, start, end, size);
					i = i + size;
					// cout << "back and i = " << i << endl;
					found = true;
				}
				k++;
			}
		}
	}	// cout << "process " << i << ": op: " << processes.at(i).getOp() << " PID: " << processes.at(i).getPID() << " size: " << processes.at(i).getSize() << " starting at: " << processes.at(i).getStart() << endl;
}

void simulation::writeTable(int pid, int start, int end, int size)
// write table
{
	if (pid != 0)
		cout << right << setw(4) << pid;
	else
		cout << right << setw(4) << "Free";
	cout.width(10);
	cout << start;
	cout.width(10);
	cout << end;
	cout.width(10);
	cout << size << endl;
}