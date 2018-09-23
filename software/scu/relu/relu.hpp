#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class RELU
{
    vector<char> inSeq; // input operand x with 3 stage delay
    vector<unsigned int> randSeq; // input selection signal, here the sequence is random source, not binary
    unsigned int bitLength;
    unsigned int depth;
    vector<char> outSeq;

    float inProb;
    unsigned int seqLength;
    float inAC;
    float theoProb;
    vector<float> realProb;
    vector<float> errRate;
    string m_name;
    unsigned int lowErrLen;
    unsigned int ppStage;

    public:
        RELU();
        ~RELU();
        void Help();
        void Init(vector<char>, vector<unsigned int>, unsigned int, unsigned int, string);
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
        unsigned int PPStage();
};