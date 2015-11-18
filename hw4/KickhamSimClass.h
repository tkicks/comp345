using namespace std;
class simulation {
	public:
		simulation();
		~simulation();
		void getFile(vector<Job> &processes, simulation memory);
		void allocate(Job *newJob, vector<Job> &processes);
		void deallocate(vector<Job> &processes, int PID);
	private:
		int memUsed;
};