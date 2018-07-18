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
    vector<vector<unsigned int>> inSeq; // input operand x with 3 stage delay
    vector<vector<unsigned int>> inSel; // input selection signal, here the sequence is random source, not binary


    vector<float> inProb;
    unsigned int inDim;

    unsigned int seqLength;
    vector<unsigned int> outSeq;
    float inCC;
    float theoProb;
    vector<float> realProb;
    vector<float> errRate;
    string m_name;
    unsigned int lowErrLen;

    public:
        EXP();
        ~EXP();
        void Help();
        void Init(vector<vector<unsigned int>>, vector<vector<unsigned int>>, string);
        void Report();
        void Calc();
        void OutPrint();
        vector<unsigned int> OutSeq();
        float InCC();
        vector<float> InProb();
        float TheoProb();
        vector<float> RealProb();
        float FinalRealProb();
        vector<float> ErrRate();
        float FinalErrRate();
        unsigned int SeqLen();
        unsigned int LowErrLen();
};