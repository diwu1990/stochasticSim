#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "perfsim.hpp"
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class CORDIV
{
    // initial input
    vector<float> iProb;
    unsigned int depth;
    unsigned int wSize;
    float thdBias;
    string m_name;

    // calc input
    vector<char> iBit;
    unsigned int randNum;

    // internal
    unsigned int iDim;
    unsigned int oDim;
    #ifdef PERFSIM
        unsigned int iLen;
    #endif
    unsigned int satCnt;
    unsigned int upperbound;

    // output
    vector<char> oBit;

    // perfsim output
    #ifdef PERFSIM
        vector<vector<char>> oBS;
        vector<float> wProb;
        vector<float> theoProb;
        vector<float> wBias;
        vector<unsigned int> speed;
    #endif

public:
    void Help();
    void Init(vector<float>, unsigned int, unsigned int, float, string);
    void Calc(vector<char>, unsigned int);
    vector<char> OutBit();

    #ifdef PERFSIM
        vector<vector<char>> OutBS();
        vector<float> WProb();
        vector<float> TheoProb();
        vector<float> WBias();
        vector<unsigned int> Speed();
    #endif
};