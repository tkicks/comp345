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
		int getLength();
		int getWaitTime();
		void setLeft(int timeLeft);
		void setIOLen(int ioLen);
		void setTotalIO(int ioLen);
		void setTurnaround(int turnaround);
		void setWaitTime(int waitTime);
	private:
		int startTime, pid, prob, length, timeLeft, ioLen, totalIO, turnaround, waitTime;
};