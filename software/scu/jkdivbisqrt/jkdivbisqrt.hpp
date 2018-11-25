#pragma once
#ifndef JKDIVBISQRT_H
#define JKDIVBISQRT_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "perfsim.hpp"
#include "muxadd.hpp"
#include "jkff.hpp"
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class JKDIVBISQRT
{
    // initial input
    vector<float> iProb;
    unsigned int depthSync;
    unsigned int depth;
    unsigned int wSize;
    float thdBias;
    string m_name;

    // calc input
    vector<char> iBit;
    vector<unsigned int> randNum;

    // internal
    unsigned int iDim;
    unsigned int oDim;
    #ifdef PERFSIM
        unsigned int iLen;
    #endif
    MUXADD muxInst;
    JKFF traceInst;
    vector<float> muxProb;
    vector<char> muxInBit;
    vector<float> traceProb;
    vector<char> traceInBit;
    vector<unsigned int> muxInNum;

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
    void Init(vector<float>, unsigned int, unsigned int, unsigned int, float, string);
    void Calc(vector<char>, vector<unsigned int>);
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