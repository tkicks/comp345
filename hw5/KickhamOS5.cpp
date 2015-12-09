/*
page faults working for test deltas
*/

#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

const int MAX = 2000;
const int N   = 10;
const int maxDelta = 5;

void setupstring (char [], int&);
void runSim(char omega[], int omegalength, int delta, float& pageFault, float& avgSize);
void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window, float& pageFault, float& avgSize);

int main ()
{
   char omega [MAX];
   int omegalength;
   setupstring (omega, omegalength);
   // cout << omegalength << endl;

   for (int delta = 1; delta <= maxDelta; delta++)
   {
      float pageFault = 0;
      float avgSize = 0;
      runSim(omega, omegalength, delta, pageFault, avgSize);
      float faultRate = pageFault/omegalength;
      cout << "page fault: " << pageFault << endl;
      cout << "average size: " << avgSize << endl;
      cout << "average page fault rate: " << faultRate << endl;
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
   // cout << "avgSize " << avgSize << " numSets " << numSets << endl; 
   avgSize /= numSets;
}

void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window, float& pageFault, float& avgSize)
//
{
   // for (int k = 0; k < window.size(); k++)
   //    cout << window.at(k);
   // cout << endl;
   char newData = omega[omegaLoc];
   bool newPage = true;
   bool eraseBegin = true;
   // int eraseFirst = -1;

   if (window.size() < delta)
      eraseBegin = false;

   for (int i = 0; i < window.size(); i++)
   {
      if (window.at(i) == newData)
      {
         newPage = false;
         // eraseFirst = i;
         // if (eraseFirst == 0)
         //    eraseBegin = false;
         // cout << newData << " already at " << omegaLoc << " " << window.at(i) << endl;
      }
   }

   if (newPage)
      pageFault += 1;
   // else
   //    window.erase(window.begin()+eraseFirst);

   if (eraseBegin)
      window.erase(window.begin());

   window.push_back(newData);
   avgSize += window.size();
   // cout << window.size() << endl;
   omegaLoc += 1;
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