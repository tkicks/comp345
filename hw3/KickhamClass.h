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
		void setLeft(int timeLeft);
	private:
		int startTime, pid, prob, timeLeft;
};