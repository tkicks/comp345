#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

const int MAX = 2000;
const int N   = 10;
const int maxDelta = 2;

void setupstring (char [], int&);
void runSim(char omega[], int omegalength, int delta);
void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window);
void fixWindow(vector<char> &window, int delta, int i, char newData);

int main ()
{
   char omega [MAX];
   int omegalength;
   setupstring (omega, omegalength);

   for (int delta = 1; delta <= maxDelta; delta++)
   {
      runSim(omega, omegalength, delta);
      cout << endl << endl;
   }
   // for debugging
   for (int i = 0; i < omegalength; i++)
      cout << omega[i];
   cout << endl;
}

void runSim(char omega[], int omegalength, int delta)
//
{
   vector<char> window;
   int omegaLoc = 0;
   while (omegaLoc <= omegalength)
   {
      setWindow(omega, omegalength, delta, omegaLoc, window);
   }
}

void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window)
//
{
   char newData = omega[omegaLoc];
   int i = 0;
   bool newPage = true;
   if (omegaLoc >= delta)
      window.erase(window.begin());
   window.push_back(newData);
   cout << window.front();
   // while (i < delta)
   // {
   //    // cout << "window[" << i << "] = " << window.at(i) << "     new = " << newData << endl;
   //    if (window.at(i) == newData)
   //    {
   //       cout << omegaLoc << "/" << omegalength << " page exists\n";
   //       newPage = false;
   //       break;
   //    }
   //    if (window.at(i) == '\0')
   //       break;
   //    i++;
   // }

   // if (!newPage)
   // {
   //    if (i < delta)
   //       cout << "forget it\n";
   // }

   // else
   //    window.push_back(newData);
   // cout << window.at(1) << endl;
   
   // debugging ------------------------
   // if (omegaLoc == omegalength)
   // {
   //    cout << delta << ": ";
   //    for (int k = 0; k < delta; k++)
   //       cout << window.at(k);
   //    cout << endl;
   // }
   // ----------------------------------
   omegaLoc += 1;
}

void fixWindow(vector<char> &window, int delta, int i, char newData)
//
{
   window.erase(window.begin());
   window.at(i) = newData;
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