#pragma once
#ifndef RACEL_H
#define RACEL_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

// length of race regs should be from 4 to 16.

class RACEL
{
    unsigned int racelLen;
    unsigned int outputLen;
    string m_name;
    vector<unsigned int> outSeq;

    public:
        RACEL();
        ~RACEL();
        void Help();
        // init params: racelLen, initState, polyIdx
        // the legal range of raceLen is 4~16
        void Init(unsigned int, unsigned int, unsigned int, unsigned int, string);
        void Report();
        void SeqGen();
        void SeqPrint();
        unsigned int SeqLen();
        vector<unsigned int> OutSeq();
};

#endif