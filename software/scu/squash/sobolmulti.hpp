#pragma once
#ifndef SOBOLMULTI_H
#define SOBOLMULTI_H

#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SOBOLMulti
{
    unsigned int sobolLen;
    unsigned int dimNum;
    unsigned int delayIndex;
    unsigned int dimIndex;
    unsigned int outputLen;
    string mode;
    string m_name;
    vector<vector<unsigned int>> outSeq;
    vector<vector<unsigned int>> dirVec;
    vector<vector<unsigned int>> dirMem;
    SOBOL sobolInst;

    public:
        SOBOLMulti();
        ~SOBOLMulti();
        void Help();
        void Init(unsigned int, unsigned int, unsigned int, unsigned int, string, string);
        void Report();
        void SeqGen();
        void SeqPrint();
        void VecPrint();
        void MemPrint();
        unsigned int SeqLen();
        vector<vector<unsigned int>> OutSeq();
        vector<vector<unsigned int>> DirMem();
        vector<vector<unsigned int>> DirVec();

};

#endif