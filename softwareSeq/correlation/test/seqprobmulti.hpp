#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class SeqProbMulti
{
    vector<vector<char>> inSeq;
    unsigned int inDim;
    string m_name;
    vector<unsigned int> seqLength;
    vector<float> outProb;

    public:
        SeqProbMulti();
        ~SeqProbMulti();
        void Help();
        void Init(vector<vector<char>>, string);
        void Calc();
        void Report();
        void ProbPrint();
        vector<unsigned int> SeqLen();
        vector<float> OutProb();
};