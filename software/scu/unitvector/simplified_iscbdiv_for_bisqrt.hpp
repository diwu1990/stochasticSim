#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SIMPLIFIED_ISCBDIV_FOR_BISQRT
{
    vector<unsigned int> inSeq;
    float inProb;
    float inAC;
    float outAC;
    vector<unsigned int> randNum;
    unsigned int bitLength;
    unsigned int logDepth;

    unsigned int seqLength;
    vector<unsigned int> outSeq;
    float theoProb;
    vector<float> realProb;
    vector<float> errRate;
    string m_name;
    unsigned int lowErrLen;
    unsigned int ppStage;

    public:
        SIMPLIFIED_ISCBDIV_FOR_BISQRT();
        ~SIMPLIFIED_ISCBDIV_FOR_BISQRT();
        void Help();
        void Init(vector<unsigned int>, vector<unsigned int>, unsigned int, string);
        void Report();
        void Calc();
        void OutPrint();
        vector<unsigned int> OutSeq();
        float InProb();
        float InAC();
        float OutAC();
        float TheoProb();
        vector<float> RealProb();
        float FinalRealProb();
        vector<float> ErrRate();
        float FinalErrRate();
        unsigned int SeqLen();
        unsigned int LowErrLen();
        unsigned int PPStage();
};