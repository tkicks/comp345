#include<iostream>
#include<string.h>

using namespace std;

const int MAX = 2000;
const int N   = 10;
const int maxDelta = 2;

void setupstring (char [], int&);
void runSim(char omega[], int omegalength);
void setWindow(char omega[], char window[], int omegalength, int delta, int& omegaLoc);
void fixWindow(char window[], int delta, int i, char newData);

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
   for (int delta = 1; delta <= maxDelta; delta++)
   {
      omegaLoc = 0;     // reset omegalocation to start
      char window[delta];
      while (omegaLoc <= omegalength)
      {
         setWindow(omega, window, omegalength, delta, omegaLoc);
      }
   }
}

void setWindow(char omega[], char window[], int omegalength, int delta, int& omegaLoc)
//
{
   char newData = omega[omegaLoc];
   int tempLoc = omegaLoc - delta;
   int i = 0;
   bool newPage = true;
   while (i < delta)
   {
      if (window[i] == newData)
      {
         cout << omegaLoc << "/" << omegalength << " page exists\n";
         newPage = false;
         break;
      }
      if (window[i] == '\0')
         break;
      i++;
      tempLoc++;
   }

   if (!newPage)
   {
      fixWindow(window, delta, i, newData);
   }

   else
      window[i] = newData;
   
   // debugging ------------------------
   if (omegaLoc == omegalength)
   {
      cout << delta << ": ";
      for (int k = 0; k < delta; k++)
         cout << window[k];
      cout << endl;
   }
   // ----------------------------------
   omegaLoc += 1;
}

void fixWindow(char window[], int delta, int i, char newData)
//
{
   while (i < delta-1 and window[i] != '\0')
   {
      window[i] = window[i+1];
      i += 1;
   }
   window[i] = newData;
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