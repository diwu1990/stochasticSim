#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "perfsim.hpp"
#include "gdiv.hpp"
#include "cordiv.hpp"
#include "iscbdiv.hpp"
#include "gsqrt.hpp"
#include "jkdivbisqrt.hpp"
#include "iscbdivbisqrt.hpp"
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SQUASH
{
    // initial input
    vector<float> iProb;
    unsigned int depthSync;
    unsigned int depth;
    unsigned int wSize;
    float thdBias;
    string m_name;

    // calc input
    vector<char> iBit;
    vector<unsigned int> randNum;

    // internal
    unsigned int iDim;
    unsigned int oDim;
    #ifdef PERFSIM
        unsigned int iLen;
    #endif

    ISCBDIV divInst[iDim];
    // CORDIV divInst[iDim];
    // GDIV divInst[iDim];

    ISCBDIVBISQRT sqrtInst;
    // JKDIVBISQRT sqrtInst;
    // GSQRT sqrtInst;

    vector<char> sqreBit;
    vector<char> sumBit;
    vector<char> sqrtBit;

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
    SQUASH(unsigned int);
    
    void Help();
    void Init(vector<float>, unsigned int, unsigned int, unsigned int, float, string);
    void Calc(vector<char>, vector<unsigned int>);
    vector<char> OutBit();

    #ifdef PERFSIM
        vector<vector<char>> OutBS();
        vector<float> WProb();
        vector<float> TheoProb();
        vector<float> WBias();
        vector<unsigned int> Speed();
    #endif
};