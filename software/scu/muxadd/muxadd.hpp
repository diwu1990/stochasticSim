#pragma once
#ifndef MUXADD_H
#define MUXADD_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "perfsim.hpp"
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class MUXADD
{
    // initial input
    vector<float> iProb;
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

    // output
    vector<char> oBit;

    // perfsim output
    #ifdef PERFSIM
        vector<vector<char>> oBS;
        vector<float> wProb;
        vector<float> theoProb;
        vector<float> wBias;
        vector<unsigned int> ctime;
    #endif

public:
    void Help();
    void Init(vector<float>, unsigned int, float, string);
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