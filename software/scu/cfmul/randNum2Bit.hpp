#pragma once
#ifndef RANDNUM2BIT_H
#define RANDNUM2BIT_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class RandNum2Bit
{
    float expectation;
    unsigned int bitLength;
    vector<unsigned int> NumVec;
    vector<char> bitVec;
    unsigned int unipolar;
    string m_name;

public:
    RandNum2Bit();
    ~RandNum2Bit();
    void Help();
    void Report();
    void SeqPrint();
    void Init(float, unsigned int, vector<unsigned int>, unsigned int, string);
    void SeqGen();
    vector<char> OutSeq();
};

#endif