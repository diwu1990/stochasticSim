#pragma once
#ifndef SEQSEARCH_H
#define SEQSEARCH_H

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

class SeqSearch
{
    /*find the combination of sequences with low discrepency less than input threshold*/
    vector<vector<unsigned int>> inSeq;
    unsigned int bitLength;
    float threshold;
    unsigned int inDim;
    unsigned int inLen;
    vector<vector<unsigned int>> outComb;
    unsigned int unipolar;
    string m_name;
    unsigned int combNum;
    unsigned int totalComb;
    float msCC;
    vector<float> meansquareCC;

    public:
        SeqSearch();
        ~SeqSearch();
        void Help();
        void Init(vector<vector<unsigned int>>, unsigned int, float, unsigned int, string);
        void Report();
        void Calc();
        void CombPrint();
        void MSCCPrint();
        vector<vector<unsigned int>> OutComb();
        unsigned int CombNum();
        float MSCC();
        vector<float> MSCCVec();
};

#endif