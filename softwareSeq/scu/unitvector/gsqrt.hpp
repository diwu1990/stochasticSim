#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class GSQRT
{
    vector<char> inSeq;
    float inProb;
    float inAC;
    float outAC;
    vector<unsigned int> randNum;
    unsigned int bitLength;
    unsigned int depth;

    unsigned int seqLength;
    vector<char> outSeq;
    float theoProb;
    vector<float> realProb;
    vector<float> errRate;
    string m_name;
    unsigned int lowErrLen;
    unsigned int ppStage;

    public:
        GSQRT();
        ~GSQRT();
        void Help();
        void Init(vector<char>, vector<unsigned int>, unsigned int, string);
        void Report();
        void Calc();
        void OutPrint();
        vector<char> OutSeq();
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