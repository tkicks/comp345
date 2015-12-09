/*
page faults working for test deltas
*/

#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>

using namespace std;

const int MAX = 2000;
const int N   = 10;
const int maxDelta = 5;

void setupstring (char [], int&);
void writeHeader();
void runSim(char omega[], int omegalength, int delta, float& pageFault, float& avgSize);
void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window, float& pageFault, float& avgSize);
void writeTable(int delta, float pageFault, float avgSize, float faultRate);

int main ()
{
   char omega [MAX];
   int omegalength;
   setupstring (omega, omegalength);
   writeHeader();

   for (int delta = 1; delta <= maxDelta; delta++)
   {
      float pageFault = 0;
      float avgSize = 0;
      runSim(omega, omegalength, delta, pageFault, avgSize);
      float faultRate = pageFault/omegalength;
      writeTable(delta, pageFault, avgSize, faultRate);
   }
   // for debugging
   // for (int i = 0; i < omegalength; i++)
   //    cout << omega[i];
   // cout << endl;
}

void runSim(char omega[], int omegalength, int delta, float& pageFault, float& avgSize)
//
{
   vector<char> window;
   int omegaLoc = 0;
   int numSets = 0;
   while (omegaLoc < omegalength)
   {
      setWindow(omega, omegalength, delta, omegaLoc, window, pageFault, avgSize);
      numSets += 1;
   }
   avgSize /= numSets;
}

void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window, float& pageFault, float& avgSize)
//
{
   char newData = omega[omegaLoc];
   bool newPage = true;
   bool eraseBegin = true;
   // int eraseFirst = 0;

   if (window.size() < delta and omegaLoc < delta)
      eraseBegin = false;

   for (int i = 0; i < window.size(); i++)
   {
      if (window.at(i) == newData)
      {
         newPage = false;
         // eraseFirst = i;
         // if (eraseFirst == 0)
         //    eraseBegin = false;
         // window.erase(window.begin()+eraseFirst);
      }
   }

   if (newPage)
      pageFault += 1;
   // else if (!eraseBegin)
   //    window.erase(window.begin()+eraseFirst);

   if (eraseBegin)
      window.erase(window.begin());

   window.push_back(newData);
   avgSize += window.size();
   omegaLoc += 1;
}

void writeHeader()
{
   cout << "Delta";
   cout.width(11);
   cout << "Faults";
   cout.width(22);
   cout << "Avg Size WS";
   cout.width(22);
   cout << "Fault Rate\n";
}

void writeTable(int delta, float pageFault, float avgSize, float faultRate)
{
   cout.precision(0);
   cout << fixed;
   cout.width(3);
   cout << delta;
   cout.width(12);
   cout << pageFault;

   cout << fixed;
   cout.precision(3);
   cout.width(20);
   cout << avgSize;
   cout.width(20);
   cout << faultRate << endl;
}

void setupstring (char omega[], int& omegaleng)
{
   int i, j;
   strcpy (omega, "272722");
   for (i = 0; i < N; i++)
      strcat (omega, "28272272927222");
   strcat (omega, "272722");
   for (i = 0; i < N; i++) {
      strcat (omega, "272733733");
      for (j = i; j < N; j++)
         strcat (omega, "373338393373737333");
      strcat (omega, "3637322");
   }
   omegaleng = strlen (omega);
}