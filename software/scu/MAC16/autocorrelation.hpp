#pragma once
#ifndef AUTOCORRELATION_H
#define AUTOCORRELATION_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class AutoCorrelation
{
    vector<char> bitVec;
    unsigned int offset;
    float expectation;
    float outAC;
    string m_name;

    public:
        void Help();
        void Init(vector<char>, unsigned int, float, string);
        void Report();
        void Calc();
        float OutAC();
    
};

#endif