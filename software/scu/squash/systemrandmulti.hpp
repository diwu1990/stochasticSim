#pragma once
#ifndef SYSTEMRANDMULTI_H
#define SYSTEMRANDMULTI_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SystemRandMulti
{
    unsigned int bitLength;
    unsigned int dimNum;
    unsigned int delayIndex;
    unsigned int dimIndex;
    unsigned int outputLen;
    string mode;
    string m_name;
    vector<vector<unsigned int>> outSeq;

    public:
        SystemRandMulti();
        ~SystemRandMulti();
        void Help();
        void Init(unsigned int, unsigned int, unsigned int, unsigned int, string, string);
        void Report();
        void SeqGen();
        void SeqPrint();
        unsigned int SeqLen();
        vector<vector<unsigned int>> OutSeq();

};

#endif