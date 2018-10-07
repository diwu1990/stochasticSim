#pragma once
#ifndef SOBOL_H
#define SOBOL_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

// I4 means Int 4 Byte, 32 bit in total
int i4_bit_hi1 ( int n ); // returns the position of the high 1 bit base 2 in an integer.
int i4_bit_lo0 ( int n ); // returns the position of the low 0 bit base 2 in an integer.
int*  i4_sobol ( int dim_num, int *seed, float quasi[ ] ); // generates a new quasirandom Sobol vector with each call.

class SOBOL
{
    unsigned int sobolLen;
    unsigned int dimNum;
    unsigned int outputLen;
    unsigned int delay;
    string m_name;
    vector<unsigned int> outSeq;
    vector<unsigned int> dirVec;
    vector<unsigned int> dirMem;

    public:
        SOBOL();
        ~SOBOL();
        void Help();
        // init params: dimNum, sobolLen, outputLen
        // the legal range of dimNum is 1~1111
        // the legal range of sobolLen is non-zero
        // the legal range of outputLen is power 2 value of sobolLen
        void Init(unsigned int, unsigned int, unsigned int, string);
        void Report();
        void SeqGen();
        void SeqPrint();
        void VecPrint();
        void MemPrint();
        unsigned int SeqLen();
        vector<unsigned int> OutSeq();
        vector<unsigned int> DirVec();
        vector<unsigned int> DirMem();
        float * i4_sobol_generate();
};

#endif