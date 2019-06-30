#pragma once
#ifndef RACELMULTI_H
#define RACELMULTI_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class RACELMulti
{
    unsigned int racelLen;
    unsigned int dimNum;
    unsigned int outputLen;
    string mode;
    string m_name;
    vector<vector<unsigned int>> outSeq;
    vector<unsigned int> arrayLen;

    public:
        RACELMulti();
        ~RACELMulti();
        void Help();
        void Init(unsigned int, unsigned int, unsigned int, unsigned int, string, string);
        void Report();
        void SeqGen();
        void SeqPrint();
        unsigned int SeqLen();
        vector<vector<unsigned int>> OutSeq();
};

#endif