using namespace std;
class Job
{
	public:
		Job(int startTime, int pid, int prob, int length);
		~Job();
		int getPID();
	private:
		int startTime, pid, prob, timeLeft;
};