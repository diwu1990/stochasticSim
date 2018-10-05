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
#include "jkdivbisqrt.hpp"
#include "synchronizer.hpp"
#include "desynchronizer.hpp"
#include "perfsim.hpp"

int main()
{
    srand(time(NULL));
    unsigned int randSeqNum = 4;
    unsigned int randBitLen = 8;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 10000;
    clock_t begin = clock();
    unsigned int seqLength = (unsigned int)pow(2,randBitLen);

    unsigned int foldNum = 11;
    vector<float> tenFoldErr(foldNum);
    vector<float> tenFoldBias(foldNum);
    vector<unsigned int> tenFoldNum(foldNum);
    vector<float> tenFoldLowErrLen(foldNum);
    
    float thdBias = 0.05;
    unsigned int wSize = seqLength/2;
    
    unsigned int totalRound = 10;
    for (int index = 0; index < totalRound; ++index)
    {
        for (int i = 0; i < foldNum; ++i)
        {
            tenFoldErr[i] = 0;
            tenFoldBias[i] = 0;
            tenFoldNum[i] = 0;
            tenFoldLowErrLen[i] = 0;
        }
        unsigned int seedInitIdx = 1+index;
        unsigned int delay = 0;
        SystemRandMulti rngInst;
        // SOBOLMulti rngInst;
        // LFSRMulti rngInst;
        rngInst.Init(randSeqNum,seedInitIdx,delay,randBitLen,mode,"rngInst");
        rngInst.SeqGen();

        vector<unsigned int> bitLengthVec(2);
        vector<float> probVec(2);
        vector<float> val(2);
        unsigned int depth;
        unsigned int depthSync;
        depth = 2;
        depthSync = 5;
        for (int iter = 0; iter < totalIter; ++iter)
        {
            /* code */
            float prob0 = (float)((float)(rand()%(int)pow(2,randBitLen))/(float)pow(2,randBitLen));
            float prob1 = (float)((float)(rand()%(int)pow(2,randBitLen))/(float)pow(2,randBitLen));
            val[0] = prob0;
            val[1] = max(prob0,prob1);
            // val[0] = 0.3;
            // val[1] = 0.7;
            for (int l = 0; l < 2; ++l)
            {
                bitLengthVec[l] = randBitLen;
                probVec[l] = val[l];
            }
            
            vector<vector<unsigned int>> inRandNum(2);
            inRandNum[0].resize(seqLength);
            inRandNum[1].resize(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                inRandNum[0][z] = rngInst.OutSeq()[0][z%(unsigned int)(pow(2,randBitLen))];
                inRandNum[1][z] = rngInst.OutSeq()[0][z%(unsigned int)(pow(2,randBitLen))];
            }

            RandNum2BitMulti num2bitMultiInst;
            num2bitMultiInst.Init(probVec,bitLengthVec,inRandNum,"num2bitMultiInst");
            num2bitMultiInst.SeqGen();

            vector<vector<unsigned int>> RandSeq(2);
            RandSeq[0].resize(seqLength);
            RandSeq[1].resize(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                RandSeq[0][z] = rngInst.OutSeq()[1][z%(unsigned int)(pow(2,randBitLen))] >> depthSync;
                RandSeq[1][z] = rngInst.OutSeq()[2][z%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - (unsigned int)log2(depth));
            }

            vector<char> iBit(1);
            vector<unsigned int> iRandNum(1);
            JKDIVBISQRT computeInst;
            computeInst.Init(probVec, depthSync, depth, wSize, thdBias, "computeInst");
            for (int j = 0; j < seqLength; ++j)
            {
                iBit[0] = num2bitMultiInst.OutSeq()[0][j];
                iRandNum[0] = RandSeq[0][j];
                iRandNum[1] = RandSeq[1][j];
                computeInst.Calc(iBit,iRandNum);
                // printf("%d: (%u)=>(%u)\n", j, iBit[0], computeInst.OutBit()[0]);
            }
            // printf("input prob       (%f)\n", probVec[0]);
            // printf("theoretical prob (%f)\n",computeInst.TheoProb()[0]);
            // printf("window prob      (%f)\n",computeInst.WProb()[0]);
            // printf("window bias      (%f)\n",computeInst.WBias()[0]);
            // printf("converge speed   (%d)\n",computeInst.Speed()[0]);
            

            tenFoldErr[(unsigned int)floor(computeInst.TheoProb()[0]*10)] += computeInst.WBias()[0] * computeInst.WBias()[0];
            tenFoldBias[(unsigned int)floor(computeInst.TheoProb()[0]*10)] += computeInst.WBias()[0];
            tenFoldNum[(unsigned int)floor(computeInst.TheoProb()[0]*10)] += 1;
            tenFoldLowErrLen[(unsigned int)floor(computeInst.TheoProb()[0]*10)] += computeInst.Speed()[0];
        }
    }
    clock_t end = clock();
    double elasped_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", elasped_secs);

    for (int y = 0; y < foldNum; ++y)
    {
        tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
        tenFoldBias[y] = tenFoldBias[y]/tenFoldNum[y];
        tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
        // tenFoldCorr[y] = tenFoldCorr[y]/tenFoldNum[y];
    }
    
    // printf("Range, Freq, Correlation, Error Rate, Stat Bias, LowErrLen:\n");
    printf("Range, Freq, Error Rate, Stat Bias, LowErrLen:\n");
    for (int i = 0; i < foldNum; ++i)
    {
        // printf("%*.1f, %*u, %*.4f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/10.0), 4, tenFoldNum[i], 11, tenFoldCorr[i], 10, tenFoldErr[i], 9, tenFoldBias[i], 9, tenFoldLowErrLen[i]);
        printf("%*.1f, %*u, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/10.0), 4, tenFoldNum[i], 10, tenFoldErr[i], 9, tenFoldBias[i], 9, tenFoldLowErrLen[i]);
    }
    printf("\n");
}