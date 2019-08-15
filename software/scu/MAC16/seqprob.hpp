#pragma once
#ifndef SEQPROB_H
#define SEQPROB_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SeqProb
{
    vector<char> inSeq;
    string m_name;
    unsigned int seqLength;
    float outProb;

    public:
        SeqProb();
        ~SeqProb();
        void Help();
        void Init(vector<char>, string);
        void Report();
        void ProbPrint();
        void Calc();
        unsigned int SeqLen();
        float OutProb();
};

#endif