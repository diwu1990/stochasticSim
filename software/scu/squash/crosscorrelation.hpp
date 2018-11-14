#pragma once
#ifndef CROSSCORRELATION_H
#define CROSSCORRELATION_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class CrossCorrelation
{
    vector<vector<char>> bitVec;
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
        void Help();
        void Init(vector<vector<char>>, float, string);
        void Calc();
        void Report();
        void CCPrint();
        vector<float> OutCC();
        float MaxCC();
        float MinCC();
};

#endif