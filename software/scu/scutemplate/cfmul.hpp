#pragma once
#ifndef CFMUL_H
#define CFMUL_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "perfsim.hpp"
#include "sobol.hpp"
#include "sobolmulti.hpp"
#include "lfsr.hpp"
#include "lfsrmulti.hpp"
#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class CFMUL
{
    // initial input
    vector<float> iProb;
    unsigned int cfree;
    unsigned int rngDepth;
    unsigned int inStream;
    unsigned int inSWindow;
    unsigned int wSize;
    float thdBias;
    unsigned int unipolar;
    string m_name;

    // calc input
    vector<char> iBit;

    // internal
    unsigned int iDim;
    unsigned int upperBound;
    unsigned int halfBound;
    unsigned int bound1;
    unsigned int bound2;

    unsigned int cnt;
    unsigned int cnt_inv;
    unsigned int rngIdx;
    unsigned int rngIdx_inv;
    char regenBit;
    char regenBit_inv;
    char lastBit;
    char lastBit_inv;
    unsigned int oDim;
    #ifdef PERFSIM
        unsigned int iLen;
    #endif
    SOBOLMulti rngInst;
    // LFSRMulti rngInst;

    RandNum2BitMulti num2bitMultiInst;

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
    void Init(vector<float>, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float, unsigned int, string);
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