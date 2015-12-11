/*
Name:    Tyler Kickham
Program: Working Set Memory Management Simulation (OS5)
Purpose: Learn how the working set can minimize page faults by figuring out an ideal window size for the working set
Input:   User will input an integer value for the maximum delta to be allowed in the simulation
Output:  A table will be displayed showing the delta for each row, as well as (for each delta) the int number of page faults,
         the float average working set size, and the float average page fault rate (floats are rounded to 3 decimal places)
*/

#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>

using namespace std;

const int MAX = 2000;          // max size of omega array
const int N   = 10;            // number of times per loop when creating string w

void setupstring (char [], int&);
void writeHeader();
void runSim(char omega[], int omegalength, int delta, float& pageFault, float& avgSize);
void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window, float& pageFault, float& avgSize);
void writeTable(int delta, float pageFault, float avgSize, float faultRate);

int main ()
{
   int maxDelta;                                               // max delta to compute
   char omega [MAX];                                           // char array omega to hold pages
   int omegalength;                                            // length of omega
   cout << "Enter max delta: ";                                //
   cin >> maxDelta;                                            // request user to enter a max delta
   setupstring (omega, omegalength);                           // set up the omega string
   writeHeader();                                              // write the table header

   for (int delta = 1; delta <= maxDelta; delta++)             // for each delta up to max delta
   {
      float pageFault = 0;                                     // set init num page faults to 0
      float avgSize = 0;                                       // set init average working set size to 0
      runSim(omega, omegalength, delta, pageFault, avgSize);   // run the simulation
      float faultRate = pageFault/omegalength;                 // calculate average page fault rate
      writeTable(delta, pageFault, avgSize, faultRate);        // write the row in the table
   }
}

void runSim(char omega[], int omegalength, int delta, float& pageFault, float& avgSize)
// INPUT: char array omega, int length of omega, int current delta, float pointer to num page faults, float pointer to average set size
// OUTPUT: none
// runs the base of the simulation
{
   vector<char> window;                                        // create a vector of characters to represent the window
   int omegaLoc = 0;                                           // set the current index of omega to 0
   int numSets = 0;                                            // set the number of sets to 0
   while (omegaLoc < omegalength)                              // for each set in the string
   {
      setWindow(omega, omegalength, delta, omegaLoc, window,
               pageFault, avgSize);                            // set the current window and get most stats
      numSets += 1;                                            // increase number of sets by 1
   }
   avgSize /= numSets;                                         // average set size is the total size of all sets / number of sets
}

void setWindow(char omega[], int omegalength, int delta, int& omegaLoc, vector<char> &window, float& pageFault, float& avgSize)
// INPUT: char array omega, int length of omega, int current delta, int pointer to index of omega, char vector pointer for current window,
//        float pointer to num page faults, float pointer to average set size
// OUTPUT: none
// sets the window for the current working set (currently very slow)
{
   char newData = omega[omegaLoc];                             // new page to put into working set
   bool newPage = true;                                        // bool for if it's a new page
   bool eraseBegin = true;                                     // bool for if the first index of set will still be in
   bool doubles;                                               // bool to catch if there are multiple instances of a page in the set

   if (window.size() < delta and omegaLoc < delta)             // if the window size is less than delta but there haven't been enough pages to fill up set
      eraseBegin = false;                                      // do not erase first element

   for (int i = 0; i < window.size(); i++)                     // for each element of the window
   {
      if (window.at(i) == newData)                             // if the element is the same as the new page
         newPage = false;                                      // set new page to false
   }

   if (newPage)                                                // if there is a new page
      pageFault += 1;                                          // increase the number of page faults

   if (eraseBegin)                                             // if the first element is no longer in the working set
      window.erase(window.begin());                            // erase first element of window

   window.push_back(newData);                                  // push the new page to the end of the window
   for (int j = 0; j < window.size(); j++)                     // for each element in the window
   {
      doubles = false;                                         // set multi instances to false
      for (int k = j+1; k < window.size(); k++)                // for every other element
      {
         if (window.at(j) == window.at(k))                     // if the two elements are equal
         {
            doubles = true;                                    // set multi instances to true
            break;                                             // stop looking for more
         }
      }
      if (!doubles)                                            // if the current element is unique
         avgSize += 1;                                         // increase the average size of the set by one
   }
   omegaLoc += 1;                                              // increase the index in omega by one
}

void writeHeader()
// INPUT: none
// OUTPUT: none
// displays the header for the table to the user
{
   cout << "Delta";                                            // display delta
   cout.width(11);                                             // set width to 11
   cout << "Faults";                                           // display faults
   cout.width(22);                                             // set width to 22
   cout << "Avg Size WS";                                      // display average working set size
   cout.width(22);                                             // set width to 22
   cout << "Fault Rate\n";                                     // display average fault rate
}

void writeTable(int delta, float pageFault, float avgSize, float faultRate)
// INPUT: integer current delta, float number page faults, float average size, float fault rate
// OUTPUT: none
// displays the table to the user
{
   cout.precision(0);                                          //
   cout << fixed;                                              // do not display any decimals
   cout.width(3);                                              // set width to 3
   cout << delta;                                              // display delta
   cout.width(12);                                             // set width to 12
   cout << pageFault;                                          // display num page faults

   cout << fixed;                                              //
   cout.precision(3);                                          // set to 3 decimal places
   cout.width(20);                                             // set width to 20
   cout << avgSize;                                            // display average working set size
   cout.width(20);                                             // set width to 20
   cout << faultRate << endl;                                  // display average fault rate
}

void setupstring (char omega[], int& omegaleng)
// INPUT: char array omega, int pointer length of omega
// OUTPUT: none
// set up the omega string
{
   int i, j;                                                   // iterators for loops
   strcpy (omega, "272722");                                   // copy string to omega
   for (i = 0; i < N; i++)                                     // for const N at top num times
      strcat (omega, "28272272927222");                        // add string to omega
   strcat (omega, "272722");                                   // add string to omega
   for (i = 0; i < N; i++) {                                   // for const N at top num times
      strcat (omega, "272733733");                             // add string to omega
      for (j = i; j < N; j++)                                  // for const N at top num times
         strcat (omega, "373338393373737333");                 // add string to omega
      strcat (omega, "3637322");                               // add string to omega
   }
   omegaleng = strlen (omega);                                 // set length of omega to length of omega
}