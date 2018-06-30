#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class AutoCorrelation
{
    vector<unsigned int> bitVec;
    unsigned int offset;
    float expectation;
    float outAC;
    string m_name;

    public:
        AutoCorrelation();
        ~AutoCorrelation();
        void Help();
        void Init(vector<unsigned int>, unsigned int, float, string);
        void Report();
        void CalcAC();
        float& OutAC();
    
};