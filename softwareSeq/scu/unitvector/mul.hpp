#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class MUL
{
    vector<vector<char>> inSeq;
    vector<float> inProb;
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
        MUL();
        ~MUL();
        void Help();
        void Init(vector<vector<char>>, string);
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