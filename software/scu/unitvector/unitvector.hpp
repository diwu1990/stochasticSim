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
    vector<unsigned int> randAdd;
    vector<unsigned int> randSqrt;
    vector<vector<unsigned int>> randDiv;
    unsigned int depthSqrt;
    unsigned int depthDiv;
    unsigned int depthDivSync;
    vector<float> inProb;
    unsigned int seqDim;

    unsigned int seqLength;
    vector<vector<unsigned int>> outSeq;
    vector<float> theoProb;
    vector<vector<float>> realProb;
    vector<float> finalRealProb;
    vector<vector<float>> errRate;
    vector<float> finalErrRate;
    vector<float> mse;
    string m_name;
    vector<unsigned int> lowErrLen;
    float finalMSE;
    float avgLowErrLen;

    public:
        UNITVECTOR();
        ~UNITVECTOR();
        void Init(vector<vector<unsigned int>>, vector<unsigned int>, vector<unsigned int>, vector<vector<unsigned int>>, unsigned int, unsigned int, unsigned int, string);
        void Calc();
        vector<vector<unsigned int>> OutSeq();
        vector<float> InProb();
        vector<float> TheoProb();
        vector<vector<float>> RealProb();
        vector<vector<float>> ErrRate();
        vector<float> FinalRealProb();
        vector<float> FinalErrRate();
        vector<float> MSE();
        vector<unsigned int> LowErrLen();
        float FinalMSE();
        float AvgLowErrLen();
};