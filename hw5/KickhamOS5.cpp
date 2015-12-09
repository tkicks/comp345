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
void runSim(char omega[], int omegalength, int delta, int& pageFault);
void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window, int& pageFault);

int main ()
{
   char omega [MAX];
   int omegalength;
   int pageFault = 0;
   setupstring (omega, omegalength);

   for (int delta = 1; delta <= maxDelta; delta++)
   {
      runSim(omega, omegalength, delta, pageFault);
      cout << "pageFault: " << pageFault << endl;
      pageFault = 0;
   }
   // for debugging
   // for (int i = 0; i < omegalength; i++)
   //    cout << omega[i];
   // cout << endl;
}

void runSim(char omega[], int omegalength, int delta, int& pageFault)
//
{
   vector<char> window;
   int omegaLoc = 0;
   while (omegaLoc < omegalength)
   {
      setWindow(omega, omegalength, delta, omegaLoc, window, pageFault);
   }
}

void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window, int& pageFault)
//
{
   // for (int k = 0; k < window.size(); k++)
   //    cout << window.at(k);
   // cout << endl;
   char newData = omega[omegaLoc];
   bool newPage = true;
   bool eraseBegin = true;

   if (window.size() < delta)
      eraseBegin = false;

   for (int i = 0; i < window.size(); i++)
   {
      if (window.at(i) == newData)
      {
         newPage = false;
         // cout << newData << " already at " << omegaLoc << " " << window.at(i) << endl;
      }
   }
   if (newPage)
      pageFault += 1;

   window.push_back(newData);

   if (eraseBegin)
      window.erase(window.begin());

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