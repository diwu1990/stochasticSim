#pragma once
#ifndef SYSTEMRAND_H
#define SYSTEMRAND_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SystemRand
{
    unsigned int bitLength;
    unsigned int outputLen;
    unsigned int dimNum;
    unsigned int delay;
    string m_name;
    vector<unsigned int> outSeq;

    public:
        SystemRand();
        ~SystemRand();
        void Help();
        // init params: dimNum, sobolLen, outputLen
        // the legal range of dimNum is 1~1111
        // the legal range of sobolLen is non-zero
        // the legal range of outputLen is power 2 value of sobolLen
        void Init(unsigned int, unsigned int, unsigned int, string);
        void Report();
        void SeqGen();
        void SeqPrint();
        unsigned int SeqLen();
        vector<unsigned int> OutSeq();
};

#endif