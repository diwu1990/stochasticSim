#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SkewedSynchronizer
{
    vector<vector<unsigned int>> inSeq;
    vector<vector<unsigned int>> outSeq;
    unsigned int depth;
    unsigned int inDim;
    unsigned int inLen;
    vector<float> inProb;
    vector<float> outProb;
    float inCC;
    float outCC;
    float errRate;
    unsigned int ppStage;
    string m_name;

public:
    SkewedSynchronizer();
    ~SkewedSynchronizer();
    void Help();
    void Init(vector<vector<unsigned int>>, unsigned int, string);
    void Report();
    void SeqGen();
    void CCPrint();
    void ProbPrint();
    void SeqPrint();
    void ErrPrint();
    vector<vector<unsigned int>> OutSeq();
    float InCC();
    float OutCC();
    float ErrRate();
    vector<float> InProb();
    vector<float> OutProb();
    unsigned int PPStage();
    unsigned int SeqLen();
};