using namespace std;
class Job {
	public:
		Job();
		Job(char op, int PID, int size);
		~Job();
		char getOp();
		int getPID();
		int getSize();
		int getStart();
		bool getSkipped();
		void setStart(int location);
		void setSkipped();
	private:
		char op;
		int PID, size, start;
		bool skipped;
};