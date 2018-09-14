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
    vector<vector<vector<unsigned int>>> iWeight;
    vector<vector<vector<unsigned int>>> iFeature;
    vector<vector<float>> iWeightProb;
    vector<vector<float>> iFeatureProb;
    vector<vector<float>> oFeatureProb;
    vector<vector<unsigned int>> randNum;
    unsigned int sameFlag;

    unsigned int weightDim;
    unsigned int featureDim;
    unsigned int seqLength;
    vector<vector<vector<unsigned int>>> oFeature;
    vector<float> inCC;
    vector<vector<float>> theoPRrob;
    vector<vector<vector<float>>> realProb;
    vector<vector<float>> finalRealProb;
    vector<vector<vector<float>>> errRate;
    vector<vector<float>> finalErrRate;
    vector<float> mse;
    string m_name;
    vector<unsigned int> lowErrLen;
    float finalMSE;
    float avgLowErrLen;

    public:
        CONV2D();
        ~CONV2D();
        void Help();
        void Init(vector<vector<unsigned int>>, vector<unsigned int>, unsigned int, string);
        void Report();
        void Calc();
        vector<vector<vector<unsigned int>>> OutSeq();
        vector<vector<float>> InWtProb();
        vector<vector<float>> InFmProb();
        vector<vector<float>> TheoProb();
        vector<vector<vector<float>>> RealProb();
        vector<vector<vector<float>>> ErrRate();
        vector<vector<float>> FinalRealProb();
        vector<vector<float>> FinalErrRate();
        vector<float> MSE();
        vector<vector<unsigned int>> LowErrLen();
        float FinalMSE();
        float AvgLowErrLen();
};