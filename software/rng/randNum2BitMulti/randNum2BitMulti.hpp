#pragma once
#ifndef RANDNUM2BITMULTI_H
#define RANDNUM2BITMULTI_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class RandNum2BitMulti
{
    vector<float> expectation;
    vector<unsigned int> bitLength;
    unsigned int seqDim;
    vector<vector<unsigned int>> NumVec;
    vector<vector<char>> bitVec;
    unsigned int unipolar;
    string m_name;

public:
    RandNum2BitMulti();
    ~RandNum2BitMulti();
    void Help();
    void Report();
    void SeqPrint();
    void Init(vector<float>, vector<unsigned int>, vector<vector<unsigned int>>, unsigned int, string);
    void SeqGen();
    vector<vector<char>> OutSeq();
};

#endif