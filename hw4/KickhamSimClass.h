using namespace std;
class simulation {
	public:
		simulation();
		~simulation();
		void getFile(vector<Job> &processes, simulation memory);
		void allocate(Job *newJob, vector<Job> &processes, vector<int> &skippedJobs);
		void deallocate(vector<Job> &processes, int PID);
		int getUsed(int location);
		void getTable(vector<Job> &processes, simulation memory);
		void writeTable(int pid, int start, int end, int size);
		void writeData(vector<Job> &processes, simulation memory, int contSpace);
	private:
		int memUsed, *memArray;
};