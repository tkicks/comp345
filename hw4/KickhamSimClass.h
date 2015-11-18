using namespace std;
class simulation {
	public:
		simulation();
		~simulation();
		void getFile(vector<Job> &processes, simulation memory);
		void allocate(Job *newJob, vector<Job> &processes);
		void deallocate(vector<Job> &processes, int PID);
		int getUsed(int location);
		void getTable(vector<Job> &processes, simulation memory);
		void writeTable(int pid, int start, int end, int size);
	private:
		int memUsed, *memArray;
};