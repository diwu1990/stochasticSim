#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
#include "sobol.hpp"
#include "sobolmulti.hpp"
#include "systemrandmulti.hpp"
#include "lfsr.hpp"
#include "lfsrmulti.hpp"
#include <cstdlib>
#include <ctime>
#include "apcadd.hpp"

int main()
{
    srand(time(NULL));
    unsigned int bitLength = 8;
    unsigned int SeqDim = 16;
    unsigned int RandNumDim = 1;
    unsigned int totalSeqNum = SeqDim+RandNumDim;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 10000;
    unsigned int seqLength = (unsigned int)pow(2,bitLength);
    unsigned int foldNum = 11;
    vector<float> tenFoldErr(foldNum);
    vector<unsigned int> tenFoldNum(foldNum);
    vector<float> tenFoldLowErrLen(foldNum);
    for (int index = 0; index < 10; ++index)
    {
        for (int i = 0; i < foldNum; ++i)
        {
            tenFoldErr[i] = 0;
            tenFoldNum[i] = 0;
            tenFoldLowErrLen[i] = 0;
        }
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
        unsigned int depth;
        // depth = (unsigned int)pow(2,3);
        depth = 2;
        for (int iter = 0; iter < totalIter; ++iter)
        {
            /* code */
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

            // if RandNumDim > 0
            vector<vector<unsigned int>> RandSeq(RandNumDim);
            RandSeq[0].resize(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                RandSeq[0][z] = rngInst.OutSeq()[SeqDim][z%(unsigned int)(pow(2,bitLength))] >> (bitLength-4);
            }
            for (int i = 1; i < RandNumDim; ++i)
            {
                RandSeq[i].resize(seqLength);
                for (int z = 0; z < seqLength; ++z)
                {
                    RandSeq[i][z] = rngInst.OutSeq()[i+SeqDim][z%(unsigned int)(pow(2,bitLength))];
                }
            }
            
            
            RandNum2BitMulti num2bitInst;
            // RandNum2Bit num2bitInst;
            num2bitInst.Init(probVec,bitLengthVec,inRandNum,"num2bitInst");
            num2bitInst.SeqGen();
            // num2bitInst.Report();
            // num2bitInst.SeqPrint();

            APCADD instance;
            instance.Init(num2bitInst.OutSeq(),RandSeq[0],"instance");
            // instance.Report();
            instance.Calc();

            float theoreticalProb = 0;
            for (int i = 0; i < SeqDim; ++i)
            {
                theoreticalProb += probVec[i];
            }
            theoreticalProb /= 16;
            tenFoldErr[(unsigned int)floor(theoreticalProb*10)] += (instance.FinalRealProb()-theoreticalProb)/theoreticalProb * (instance.FinalRealProb()-theoreticalProb)/theoreticalProb;
            tenFoldNum[(unsigned int)floor(theoreticalProb*10)] += 1;
            tenFoldLowErrLen[(unsigned int)floor(theoreticalProb*10)] += instance.LowErrLen();
        }
        for (int y = 0; y < foldNum; ++y)
        {
            // printf("11111\n");
            tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
            tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
        }
        

        printf("Ten Fold with Depth %u, initial sobolIdx %u, delay %u.\n", depth, rngInitIdx, delay);
        printf("Value range index, Freq, Final Error, LowErrLen:\n");
        for (int i = 0; i < foldNum; ++i)
        {
            printf("%3d, %5u, %-.3f, %-3.3f\n", i, tenFoldNum[i], tenFoldErr[i], tenFoldLowErrLen[i]);
        }
        printf("\n");
    }
    
}
