#pragma once
#ifndef LFSRMULTI_H
#define LFSRMULTI_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class LFSRMulti
{
    unsigned int lfsrLen;
    unsigned int dimNum;
    unsigned int delayIndex;
    unsigned int polyIndex;
    unsigned int outputLen;
    string mode;
    string m_name;
    vector<vector<unsigned int>> outSeq;
    vector<unsigned int> polyVal;

    public:
        LFSRMulti();
        ~LFSRMulti();
        void Help();
        void Init(unsigned int, unsigned int, unsigned int, unsigned int, string, string);
        void Report();
        void SeqGen();
        void SeqPrint();
        void PlyPrint();
        unsigned int SeqLen();
        vector<vector<unsigned int>> OutSeq();
        vector<unsigned int> OutPly();
};

#endif