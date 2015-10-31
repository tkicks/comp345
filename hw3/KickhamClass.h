using namespace std;
class Job
{
	public:
		Job();
		Job(int startTime, int pid, int prob, int length);
		~Job();
		int getStart();
		int getPID();
		int getProb();
		int getLeft();
		int getIOLen();
		int getTurnaround();
		void setLeft(int timeLeft);
		void setIOLen(int ioLen);
		void setTotalIO(int ioLen);
		void setTurnaround(int turnaround);
	private:
		int startTime, pid, prob, timeLeft, ioLen, totalIO, turnaround;
};