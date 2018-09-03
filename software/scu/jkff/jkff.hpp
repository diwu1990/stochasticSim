#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class JKFF
{
    vector<vector<unsigned int>> inSeq;

    vector<float> inProb;
    unsigned int inDim;

    unsigned int seqLength;
    vector<unsigned int> outSeq;
    float inCC;
    vector<float> inAC;
    float outAC;
    vector<float> outCC;
    float theoProb;
    vector<float> realProb;
    vector<float> errRate;
    string m_name;
    unsigned int lowErrLen;
    unsigned int ppStage;

    public:
        JKFF();
        ~JKFF();
        void Help();
        void Init(vector<vector<unsigned int>>, string);
        void Report();
        void Calc();
        void OutPrint();
        vector<unsigned int> OutSeq();
        float InCC();
        vector<float> InAC();
        float OutAC();
        vector<float> OutCC();
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