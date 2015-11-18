#include "KickhamSimClass.h"
#include <vector>

simulation::simulation()
{
	// construct by assigning vars
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
			processes.push_back(*newJob);
		}
		else if (op == 'D')
			memory.deallocate(processes, PID);
		else
			break;
		
	} while (op != 'Q');
}

void simulation::deallocate(vector<Job> &processes, int PID)
// deallocate process from memory
{
	cout << processes.size() << endl;
}