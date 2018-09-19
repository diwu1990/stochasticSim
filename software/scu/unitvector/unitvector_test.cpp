#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
#include "sobol.hpp"
#include "sobolmulti.hpp"
#include "lfsr.hpp"
#include "lfsrmulti.hpp"
#include "systemrand.hpp"
#include "systemrandmulti.hpp"
#include <cstdlib>
#include <ctime>
#include "gdiv.hpp"
#include "iscbdiv.hpp"
#include "gsqrt.hpp"
#include "jkdivbisqrt.hpp"
#include "iscbdivbisqrt.hpp"
#include "unitvector.hpp"

int main()
{
    srand(time(NULL));
    unsigned int bitLength = 10;
    unsigned int SeqDim = 16;
    unsigned int RandNumDim = SeqDim+2;
    unsigned int totalSeqNum = SeqDim+RandNumDim;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 10000;
    unsigned int seqLength = (unsigned int)pow(2,bitLength);
    float avgErr = 0;
    float avgLen = 0;
    float avgSumMse = 0;
    for (int index = 2; index < 3; ++index)
    {
        avgErr = 0;
        avgLen = 0;
        unsigned int rngInitIdx = 1+index;
        unsigned int delay = 1;
        SystemRandMulti rngInst;
        // SOBOLMulti rngInst;
        // LFSRMulti rngInst;
        rngInst.Init(totalSeqNum,rngInitIdx,delay,bitLength,mode,"rngInst");
        rngInst.SeqGen();
        // rngInst.SeqPrint();

        vector<unsigned int> bitLengthVec(SeqDim);
        vector<float> probVec(SeqDim);
        unsigned int divDepth = 2;
        unsigned int divDepthSync = 2;
        unsigned int sqrtDepth = 2;

        for (int iter = 0; iter < totalIter; ++iter)
        {
            if (iter % (totalIter/100) == 0)
            {
                printf("%.1f%\n", (float)iter/(float)(totalIter/100));
            }
            for (int l = 0; l < SeqDim; ++l)
            {
                bitLengthVec[l] = bitLength;
                probVec[l] = (float)((float)(rand()%(int)pow(2,bitLength))/(float)pow(2,bitLength));
            }


            vector<vector<unsigned int>> inRandNum(SeqDim);
            for (int i = 0; i < SeqDim; ++i)
            {
                inRandNum[i].resize(seqLength);
                for (int z = 0; z < seqLength; ++z)
                {
                    inRandNum[i][z] = rngInst.OutSeq()[i][z%(unsigned int)(pow(2,bitLength))];
                }
            }

            vector<vector<unsigned int>> RandSeq(RandNumDim);
            for (int i = 0; i < RandNumDim; ++i)
            {
                RandSeq[i].resize(seqLength);
                for (int z = 0; z < seqLength; ++z)
                {
                    RandSeq[i][z] = rngInst.OutSeq()[i+SeqDim][z%(unsigned int)(pow(2,bitLength))];
                }
            }

            vector<unsigned int> randAdd(seqLength);
            for (int i = 0; i < seqLength; ++i)
            {
                // randAdd[i] = (RandSeq[0][i] >> (bitLength-(unsigned int)log2(SeqDim)));
                randAdd[i] = (RandSeq[0][i] % SeqDim);
            }
            vector<unsigned int> randSqrt(seqLength);
            for (int i = 0; i < seqLength; ++i)
            {
                // randSqrt[i] = (RandSeq[1][i] >> (bitLength-(unsigned int)log2(sqrtDepth)));
                randSqrt[i] = (RandSeq[1][i] % sqrtDepth);
            }
            vector<vector<unsigned int>> randDiv(SeqDim);
            for (int i = 0; i < SeqDim; ++i)
            {
                randDiv[i].resize(seqLength);
                for (int j = 0; j < seqLength; ++j)
                {
                    // randDiv[i][j] = (RandSeq[i+2][j] >> (bitLength-(unsigned int)log2(divDepth)));
                    randDiv[i][j] = (RandSeq[i+2][j] % divDepth);
                }
            }
            
            RandNum2BitMulti num2bitInst;
            num2bitInst.Init(probVec,bitLengthVec,inRandNum,"num2bitInst");
            num2bitInst.SeqGen();

            UNITVECTOR instance;
            instance.Init(num2bitInst.OutSeq(),randAdd,randSqrt,randDiv,sqrtDepth,divDepth,divDepthSync,"instance");
            instance.Calc();
            // instance.InProb();
            // instance.TheoProb();
            // instance.FinalErrRate();
            // instance.FinalRealProb();
            // instance.LowErrLen();
            // instance.AvgLowErrLen();
            // instance.FinalMSE();
            avgErr += instance.FinalMSE();
            avgLen += instance.AvgLowErrLen();
            avgSumMse += instance.SqrtMSE();
        }
        avgErr /= totalIter;
        avgLen /= totalIter;
        avgSumMse /= totalIter;
        printf("\nAVG ERR: %f, AVG LEN: %f, AVG SUM MESE: %f\n", avgErr, avgLen, avgSumMse);
    }
    
}
