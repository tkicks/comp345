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
		void setLeft(int timeLeft);
		void setIOLen(int ioLen);
		void setTotalIO(int ioLen);
	private:
		int startTime, pid, prob, timeLeft, ioLen, totalIO;
};