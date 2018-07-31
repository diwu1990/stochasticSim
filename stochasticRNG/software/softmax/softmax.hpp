#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SOFTMAX
{
    vector<vector<unsigned int>> inSeq;
    vector<vector<unsigned int>> randNum;
    vector<float> inProb;
    unsigned int bitLength;
    unsigned int inSeqDim;
    unsigned int inRandDim;

    unsigned int seqLength;
    vector<vector<unsigned int>> outSeq;
    vector<float> inCC;
    vector<float> theoProb;
    vector<vector<float>> realProb;
    vector<vector<float>> errRate;
    vector<float> mse;
    string m_name;
    unsigned int lowErrLen;

    public:
        SOFTMAX();
        ~SOFTMAX();
        void Init(vector<vector<unsigned int>>, vector<unsigned int>, unsigned int, string);
        void Calc();
        vecrot<vector<unsigned int>> OutSeq();
};