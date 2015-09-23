#include <string>
using namespace std;

void readFile(string filename, int* numList[]);		// definition of readFile
void * sortList(void *);		// definition of sortList
void MergeSort(void * original, int size);
void Merge(int* Left, int* Right, void * original, int sizeL, int sizeR);