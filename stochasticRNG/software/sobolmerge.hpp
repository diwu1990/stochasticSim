#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SOBOLMerge
{
    vector<vector<unsigned int>> inDirVec;
    unsigned int bitLength;
    unsigned int inDim;
    unsigned int inLen;
    vector<unsigned int> outMemVec;
    string m_name;

    public:
        SOBOLMerge();
        ~SOBOLMerge();
        void Help();
        void Init(vector<vector<unsigned int>>, unsigned int, string);
        void Report();
        void Calc();
        void VecPrint();
        void MemPrint();
        vector<unsigned int> OutMem();
        unsigned int MemLen();
};