#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class DeSynchronizer
{
    vector<vector<char>> inSeq;
    vector<vector<char>> outSeq;
    unsigned int depth;
    unsigned int inDim;
    unsigned int inLen;
    vector<float> inProb;
    vector<float> outProb;
    float inCC;
    float outCC;
    vector<float> errRate;
    unsigned int ppStage;
    string m_name;

public:
    DeSynchronizer();
    ~DeSynchronizer();
    void Help();
    void Init(vector<vector<char>>, unsigned int, string);
    void Report();
    void SeqGen();
    void CCPrint();
    void ProbPrint();
    void SeqPrint();
    void ErrPrint();
    vector<vector<char>> OutSeq();
    float InCC();
    float OutCC();
    vector<float> ErrRate();
    vector<float> InProb();
    vector<float> OutProb();
    unsigned int PPStage();
    unsigned int SeqLen();
};