#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class ISCBDIV
{
    vector<vector<char>> inSeq;
    vector<float> inProb;
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
        ISCBDIV();
        ~ISCBDIV();
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