#pragma once
#ifndef SQUASH_H
#define SQUASH_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "perfsim.hpp"
#include "square.hpp"
#include "gdiv.hpp"
#include "cordiv.hpp"
#include "iscbdiv.hpp"
#include "gsqrt.hpp"
#include "jkdivbisqrt.hpp"
#include "iscbdivbisqrt.hpp"
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SQUASH
{
    // initial input
    vector<float> iProb;
    float scale;
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

    SQUARE* squareInstPtr;

    MUXADD addSqreInst;

    ISCBDIVBISQRT sqrtInst;
    // JKDIVBISQRT sqrtInst;
    // GSQRT sqrtInst;

    MUXADD add1Inst;

    ISCBDIV divInst;
    // CORDIV divInst;
    // GDIV divInst;

    ANDMUL* mulInstPtr;

    vector<vector<char>> sqreIBit;
    // vector<vector<char>> sqreOBit;
    vector<char> sumIBit;
    // vector<char> sumOBit;
    vector<char> sqrtIBit;
    // vector<char> sqrtOBit;
    vector<char> add1IBit;
    // vector<char> add1OBit;
    vector<char> divIBit;
    // vector<char> divOBit;
    vector<vector<char>> mulIBit;
    // vector<vector<char>> mulOBit;

    vector<vector<float>> sqreIProb;
    vector<float> sumIProb;
    vector<float> sqrtIProb;
    vector<float> add1IProb;
    vector<float> divIProb;
    vector<vector<float>> mulIProb;

    // output
    vector<char> oBit;

    // perfsim output
    #ifdef PERFSIM
        vector<vector<char>> oBS;
        vector<float> wProb;
        vector<float> theoProb;
        vector<float> wBias;
        vector<unsigned int> speed;
    #endif

public:
    SQUASH();
    ~SQUASH();
    
    void Help();
    void Init(vector<float>, float, unsigned int, unsigned int, unsigned int, float, string);
    void Calc(vector<char>, vector<unsigned int>);
    vector<char> OutBit();

    #ifdef PERFSIM
        vector<vector<char>> OutBS();
        vector<float> WProb();
        vector<float> TheoProb();
        vector<float> WBias();
        vector<unsigned int> Speed();
    #endif
};

#endif