#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class UNITVECTOR
{
    vector<vector<unsigned int>> inSeq;
    vector<vector<unsigned int>> randNum;
    unsigned int bitLength;
    vector<float> inProb;
    unsigned int inSeqDim;
    unsigned int inRandDim;

    unsigned int seqLength;
    vector<vector<unsigned int>> outSeq;
    vector<float> theoProb;
    vector<vector<float>> realProb;
    vector<float> finalRealProb;
    vector<vector<float>> errRate;
    vector<float> finalErrRate;
    vector<float> mse;
    string m_name;
    unsigned int lowErrLen;

    public:
        UNITVECTOR();
        ~UNITVECTOR();
        void Init(vector<vector<unsigned int>>, vector<vector<unsigned int>>, unsigned int, string);
        void Calc();
        vector<vector<unsigned int>> OutSeq();
        unsigned int LowErrLen();
};