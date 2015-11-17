using namespace std;
class Job {
	public:
		Job();
		Job(char op, int PID, int size);
		~Job();
		char getOp();
		int getPID();
		int getSize();
	private:
		char op;
		int PID, size;
};