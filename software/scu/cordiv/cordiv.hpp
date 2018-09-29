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

    // internal
    unsigned int iDim;
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
        
    vector<char> inSeq;
    vector<unsigned int> randNum;
    unsigned int depth;
    unsigned int depthSync;
    unsigned int inDim;

    unsigned int seqLength;
    vector<char> outSeq;
    float inCC;
    float theoProb;
    vector<float> realProb;
    vector<float> errRate;
    string m_name;
    unsigned int lowErrLen;
    unsigned int ppStage;

    public:
        void Help();
        void Init(vector<vector<char>>, vector<unsigned int>, unsigned int, unsigned int, string);
        void Report();
        void Calc();
        void OutPrint();
        vector<char> OutSeq();
        float InCC();
        vector<float> InProb();
        float TheoProb();
        vector<float> RealProb();
        float FinalRealProb();
        vector<float> ErrRate();
        float FinalErrRate();
        unsigned int SeqLen();
        unsigned int LowErrLen();
        unsigned int PPStage();
};