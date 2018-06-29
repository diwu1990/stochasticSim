#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class CrossCorrelation
{
    vector<vector<unsigned int>> bitVec;
    unsigned int bitVecDim;
    unsigned int bitVecLen;
    vector<float> outCC;
    float maxCC;
    unsigned int maxIdx1;
    unsigned int maxIdx2;
    float minCC;
    unsigned int minIdx1;
    unsigned int minIdx2;
    float thresholdGood;
    string m_name;

    public:
        CrossCorrelation();
        ~CrossCorrelation();
        void Help();
        void Init(vector<vector<unsigned int>>, float, string);
        void CalcCC();
        void Report();
        void CCPrint();
        vector<float> OutCC();
        float MaxCC();
        float MinCC();
};