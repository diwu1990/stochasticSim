#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class CONV2D
{
    vector<vector<vector<char>>> iWeight;
    vector<vector<vector<char>>> iFeature;
    vector<vector<float>> iWeightProb;
    vector<vector<float>> iFeatureProb;
    vector<vector<float>> oFeatureProb;
    vector<vector<unsigned int>> randNum;

    unsigned int weightDim;
    unsigned int featureDim;
    unsigned int seqLength;
    vector<vector<vector<char>>> oFeature;
    vector<vector<float>> theoPRrob;
    vector<vector<vector<float>>> realProb;
    vector<vector<float>> finalRealProb;
    vector<vector<vector<float>>> errRate;
    vector<vector<float>> finalErrRate;
    vector<float> mse;
    float finalMSE;
    vector<unsigned int> lowErrLen;
    float avgLowErrLen;
    string m_name;

    public:
        CONV2D();
        ~CONV2D();
        void Help();
        void Init(vector<vector<vector<char>>>, vector<vector<vector<char>>>, vector<vector<unsigned int>>, string);
        void Report();
        void Calc();
        vector<vector<vector<char>>> OutSeq();
        vector<vector<float>> InWtProb();
        vector<vector<float>> InFmProb();
        vector<vector<float>> TheoProb();
        vector<vector<vector<float>>> RealProb();
        vector<vector<vector<float>>> ErrRate();
        vector<vector<float>> FinalRealProb();
        vector<vector<float>> FinalErrRate();
        vector<float> MSE();
        float FinalMSE();
        vector<vector<unsigned int>> LowErrLen();
        float AvgLowErrLen();
};