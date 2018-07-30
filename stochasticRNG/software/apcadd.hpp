#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class APCADD
{
    vector<vector<unsigned int>> inSeq;
    vector<float> inProb;
    unsigned int inDim;

    unsigned int seqLength;
    vector<unsigned int> outSeq;
    vector<float> inCC;
    float theoProb;
    vector<float> realProb;
    vector<float> errRate;
    string m_name;
    unsigned int lowErrLen;
    unsigned int ppStage;

    public:
        APCADD();
        ~APCADD();
        void Help();
        void Init(vector<vector<unsigned int>>, string);
        void Report();
        void Calc();
        void OutPrint();
        vector<unsigned int> OutSeq();
        vector<float> InCC();
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