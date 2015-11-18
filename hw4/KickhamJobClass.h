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
		void setStart(int location);
	private:
		char op;
		int PID, size, start;
};