#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class EXP
{
    vector<unsigned int> inSeq; // input operand x with 3 stage delay
    vector<vector<unsigned int>> inSel; // input selection signal, here the sequence is random source, not binary
    unsigned int bitLength;
    vector<unsigned int> outSeq;

    float inProb;
    unsigned int seqLength;
    float inAC;
    float theoProb;
    vector<float> realProb;
    vector<float> errRate;
    string m_name;
    unsigned int lowErrLen;

    public:
        EXP();
        ~EXP();
        void Help();
        void Init(vector<unsigned int>, vector<vector<unsigned int>>, unsigned int, string);
        void Report();
        void Calc();
        void OutPrint();
        vector<unsigned int> OutSeq();
        float InAC();
        float InProb();
        float TheoProb();
        vector<float> RealProb();
        float FinalRealProb();
        vector<float> ErrRate();
        float FinalErrRate();
        unsigned int SeqLen();
        unsigned int LowErrLen();
};