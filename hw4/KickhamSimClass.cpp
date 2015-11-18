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

void simulation::getFile(vector<Job> &processes)
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
		if (op != 'D')
			dataFile >> size;
		if (op == 'Q')
			break;
		newJob = new Job(op, PID, size);
		processes.push_back(*newJob);
	} while (op != 'Q');
}