#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class Synchronizer
{
    vector<vector<unsigned int>> inSeq;
    vector<vector<unsigned int>> outSeq;
    unsigned int depth;
    unsigned int inDim;
    unsigned int inLen;
    float inCC;
    float outCC;
    unsigned int ppStage;
    string m_name;
public:
    Synchronizer();
    ~Synchronizer();
    void Help();
    void Init(vector<vector<unsigned int>>, unsigned int, string);
    void Report();
    void SeqGen();
    void CCPrint();
    vector<vector<unsigned int>> OutSeq();
    float InCC();
    float OutCC();
    unsigned int PPStage();
};