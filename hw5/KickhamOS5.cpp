#include<iostream>
#include<string.h>

using namespace std;

const int MAX = 2000;
const int N   = 10;

void setupstring (char [], int&);
void runSim(char omega[], int omegalength);
void setWindow(char omega[], char window[], int omegalength, int winSize, int& omegaLoc);

int main ()
{
   char omega [MAX];
   int omegalength;
   setupstring (omega, omegalength);

   runSim(omega, omegalength);
   // for debugging
   for (int i = 0; i < omegalength; i++)
      cout << omega [i];
   cout << endl;
   return 0;
}

void runSim(char omega[], int omegalength)
//
{
   int omegaLoc = 0;
   int winSize = 0;
   // for (int delta = 1; delta <= 200; delta++)
   while (omegaLoc <= omegalength)
   {
      if (winSize < 5)
         winSize += 1;

      char window[winSize];
      setWindow(omega, window, omegalength, winSize, omegaLoc);
   }
}

void setWindow(char omega[], char window[], int omegalength, int winSize, int& omegaLoc)
//
{
   int tempLoc = omegaLoc - winSize;
   int i = 0;
   while (i < winSize)
   {
      window[i] = omega[tempLoc];
      i++;
      tempLoc++;
   }
   // debugging
   for (int k = 0; k < winSize; k++)
      cout << window[k];
   omegaLoc += 1;
   cout << endl;
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