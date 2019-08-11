#pragma once
#ifndef CFADD_H
#define CFADD_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "perfsim.hpp"
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class CFADD
{
    // initial input
    vector<float> iProb; // input prob, have to be 2^N
    unsigned int scaled;
    unsigned int depthSync;
    unsigned int wSize;
    float thdBias; // threshold to consider convergence
    unsigned int unipolar;
    string m_name;

    // calc input
    vector<char> iBit;

    // internal
    unsigned int iDim;
    unsigned int oDim;
    unsigned int parallel_cnt;
    unsigned int accumulator;
    unsigned int upper;

    int signed_accumulator;
    int offset_accumulator;
    int theoOutOne;
    int offset;
    int outOneCnt;
    
    #ifdef PERFSIM
        unsigned int iLen;
    #endif

    // output
    vector<char> oBit;

    // perfsim output
    #ifdef PERFSIM
        vector<vector<char>> oBS;
        vector<float> wProb;
        vector<float> theoProb;
        vector<float> wBias;
        vector<unsigned int> cTime;
    #endif

public:
    void Help();
    void Init(vector<float>, unsigned int, unsigned int, unsigned int, float, unsigned int, string);
    void Calc(vector<char>);
    vector<char> OutBit();

    #ifdef PERFSIM
        vector<vector<char>> OutBS();
        vector<float> WProb();
        vector<float> TheoProb();
        vector<float> WBias();
        vector<unsigned int> CTime();
    #endif
};

#endif