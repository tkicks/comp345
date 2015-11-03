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
		int getTotalIO();
		void setLeft(int timeLeft);
		void setIOLen(int ioLen);
		void setTotalIO(int ioLen);
		void setTurnaround(int turnAround);
		void setWaitTime(int waitTime);
		void setLength(int length);
	private:
		int startTime, pid, prob, length, timeLeft, ioLen, totalIO, turnaround, waitTime;
};