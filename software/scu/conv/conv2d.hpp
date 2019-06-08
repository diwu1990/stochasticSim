#pragma once
#ifndef CONV2D_H
#define CONV2D_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "perfsim.hpp"
using namespace std;

class CONV2D
{
    // same convolution
    // follow the algorithm from easyCNN convolution2d_same
    // https://github.com/xylcbd/EasyCNN/blob/master/src/MathFunctions.cpp

    // without batch
    // input feature map size is  (ifm channel,              ifm height, ifm width)
    // weight size            is  (ifm channel, ofm channel, wgt height, wgt width)
    // output feature map size is (             ofm channel, ofm height, ofm width)

    // input
    vector<vector<vector<char>>> iFeature;
    vector<vector<vector<float>>> iFeatureProb;

    vector<vector<vector<vector<char>>>> iWeight;
    vector<vector<vector<vector<float>>>> iWeightProb;
    
    vector<vector<vector<char>>> oFeature;
    vector<vector<vector<float>>> oFeatureProb;

    unsigned int ifm_c;
    unsigned int ifm_h;
    unsigned int ifm_w;

    unsigned int ofm_c;
    unsigned int ofm_h;
    unsigned int ofm_w;

    unsigned int wgt_h;
    unsigned int wgt_w;

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

#endif