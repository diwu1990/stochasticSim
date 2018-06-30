#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

#define min(X, Y)  ((X) < (Y) ? (X) : (Y))
#define max(X, Y)  ((X) > (Y) ? (X) : (Y))

class XSHRR1608
{
    // input bit length should be 16, as each random number is a combination of two 8-bit numbers.
    unsigned int bitLen;
    // RR: A random (input-dependent) rotation, with output half the size of input. Given a 2^b-bit input word, the top b−1 bits are used for the rotate amount, the next-most-significant 2^(b−1) bits are rotated right and used as the output, and the low 2^(b−1)+1−b bits are discarded.
    unsigned int rrShift1;
    unsigned int rrShift2;
    unsigned int rrShift3;
    unsigned int outDim;
    vector<unsigned int> inSeq;
    vector<vector<unsigned int>> outSeq;
    unsigned int seqLength;
    string m_name;

    public:
        XSHRR1608();
        ~XSHRR1608();
        void Help();
        void Init(unsigned int, vector<unsigned int>, string);
        void Report();
        void SeqGen();
        void SeqPrint();
        unsigned int& SeqDim();
        unsigned int& SeqLen();
        vector<vector<unsigned int>>& OutSeq();
};