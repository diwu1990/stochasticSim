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
#include "gsqrt.hpp"
#include "perfsim.hpp"

int main()
{
    srand(time(NULL));
    unsigned int randSeqNum = 4;
    unsigned int randBitLen = 8;
    // string mode = "incremental";
    // string mode = "delayed";
    string mode = "random";
    unsigned int totalIter = 1000;
    clock_t begin = clock();
    unsigned int seqLength = (unsigned int)pow(2,randBitLen);

    unsigned int totalRound = 1000;

    unsigned int foldNum = 6;
    vector<vector<float>> tenFoldMSE(foldNum);
    vector<vector<unsigned int>> tenFoldNum(foldNum);
    vector<vector<float>> tenFoldLowErrLen(foldNum);
    vector<float> tenFoldAvgMSE(foldNum);
    vector<float> tenFoldAvgLowErrLen(foldNum);

    vector<float> MSEMax(1);
    vector<float> MSEMin(1);
    vector<float> MSEMaxIndex(foldNum);
    vector<float> MSEMinIndex(foldNum);

    vector<float> LowErrLenMax(1);
    vector<float> LowErrLenMin(1);
    vector<float> LowErrLenMaxIndex(foldNum);
    vector<float> LowErrLenMinIndex(foldNum);

    for (int i = 0; i < foldNum; ++i)
    {
        tenFoldMSE[i].resize(totalRound);
        tenFoldNum[i].resize(totalRound);
        tenFoldLowErrLen[i].resize(totalRound);
        tenFoldAvgMSE[i] = 0;
        tenFoldAvgLowErrLen[i] = 0;
    }

    float thdBias = 0.05;
    unsigned int wSize = seqLength/2;
    
    unsigned int inBS = 1;
    
    for (int index = 0; index < totalRound; ++index)
    {

        for (int i = 0; i < foldNum; ++i)
        {
            tenFoldMSE[i][index] = 0;
            tenFoldNum[i][index] = 0;
            tenFoldLowErrLen[i][index] = 0;
        }
        unsigned int seedInitIdx = 1+index;
        unsigned int delay = 0;
        SystemRandMulti rngInst;
        // SOBOLMulti rngInst;
        // LFSRMulti rngInst;
        rngInst.Init(randSeqNum,seedInitIdx,delay,randBitLen,mode,"rngInst");
        rngInst.SeqGen();

        vector<unsigned int> bitLengthVec(inBS);
        vector<float> probVec(inBS);
        vector<float> val(inBS);
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
            for (int l = 0; l < inBS; ++l)
            {
                bitLengthVec[l] = randBitLen;
                probVec[l] = val[l];
            }
            
            vector<vector<unsigned int>> inRandNum(inBS);
            for (int i = 0; i < inBS; ++i)
            {
                inRandNum[i].resize(seqLength);
                for (int z = 0; z < seqLength; ++z)
                {
                    inRandNum[i][z] = rngInst.OutSeq()[i][z%(unsigned int)(pow(2,randBitLen))];
                }
            }

            RandNum2BitMulti num2bitMultiInst;
            num2bitMultiInst.Init(probVec,bitLengthVec,inRandNum,"num2bitMultiInst");
            num2bitMultiInst.SeqGen();

            vector<vector<unsigned int>> RandSeq(2);
            RandSeq[0].resize(seqLength);
            RandSeq[1].resize(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                RandSeq[0][z] = rngInst.OutSeq()[1][z%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - depthSync);
                RandSeq[1][z] = rngInst.OutSeq()[2][z%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - (unsigned int)log2(depth));
            }

            vector<char> iBit(1);
            vector<unsigned int> iRandNum(2);
            GSQRT computeInst;
            computeInst.Init(probVec, depthSync, depth, wSize, thdBias, "computeInst");
            for (int j = 0; j < seqLength; ++j)
            {
                iBit[0] = num2bitMultiInst.OutSeq()[0][j];
                iRandNum[0] = RandSeq[0][j];
                iRandNum[1] = RandSeq[1][j];
                computeInst.Calc(iBit,iRandNum);
            }
            // printf("input prob       (%f)\n", probVec[0]);
            // printf("theoretical prob (%f)\n",computeInst.TheoProb()[0]);
            // printf("window prob      (%f)\n",computeInst.WProb()[0]);
            // printf("window bias      (%f)\n",computeInst.WBias()[0]);
            // printf("converge ctime   (%d)\n",computeInst.CTime()[0]);
            

            tenFoldMSE[(unsigned int)floor(computeInst.TheoProb()[0]*5)][index] += computeInst.WBias()[0] * computeInst.WBias()[0];
            tenFoldNum[(unsigned int)floor(computeInst.TheoProb()[0]*5)][index] += 1;
            tenFoldLowErrLen[(unsigned int)floor(computeInst.TheoProb()[0]*5)][index] += computeInst.CTime()[0];
        }
        for (int y = 0; y < foldNum; ++y)
        {
            tenFoldMSE[y][index] = sqrt(tenFoldMSE[y][index] / tenFoldNum[y][index]);
            tenFoldLowErrLen[y][index] = tenFoldLowErrLen[y][index] / tenFoldNum[y][index];
        }
    }

    for (int y = 0; y < foldNum; ++y)
    {
        MSEMax[0] = tenFoldMSE[y][0];
        MSEMin[0] = tenFoldMSE[y][0];
        LowErrLenMax[0] = tenFoldLowErrLen[y][0];
        LowErrLenMin[0] = tenFoldLowErrLen[y][0];

        MSEMaxIndex[y] = 0;
        MSEMinIndex[y] = 0;

        LowErrLenMaxIndex[y] = 0;
        LowErrLenMinIndex[y] = 0;

        for (int index = 0; index < totalRound; ++index)
        {
            tenFoldAvgMSE[y] += tenFoldMSE[y][index];
            tenFoldAvgLowErrLen[y] += tenFoldLowErrLen[y][index];

            if (MSEMax[0] < tenFoldMSE[y][index])
            {
                MSEMax[0] = tenFoldMSE[y][index];
                MSEMaxIndex[y] = index;
            }
            if (MSEMin[0] > tenFoldMSE[y][index])
            {
                MSEMin[0] = tenFoldMSE[y][index];
                MSEMinIndex[y] = index;
            }

            if (LowErrLenMax[0] < tenFoldLowErrLen[y][index])
            {
                LowErrLenMax[0] = tenFoldLowErrLen[y][index];
                LowErrLenMaxIndex[y] = index;
            }
            if (LowErrLenMin[0] > tenFoldLowErrLen[y][index])
            {
                LowErrLenMin[0] = tenFoldLowErrLen[y][index];
                LowErrLenMinIndex[y] = index;
            }

        }
        tenFoldAvgMSE[y] /= totalRound;
        tenFoldAvgLowErrLen[y] /= totalRound;
    }

    clock_t end = clock();
    double elasped_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("Total execution time: %f\n\n", elasped_secs);

    printf("Range, Max Error Rate, Min Error Rate, avg Error Rate:\n");
    for (int i = 0; i < foldNum; ++i)
    {
        printf("%*.1f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/5.0), 14, tenFoldMSE[i][MSEMaxIndex[i]], 14, tenFoldMSE[i][MSEMinIndex[i]], 14, tenFoldAvgMSE[i]);
    }
    printf("\n");

    printf("Range,  Max LowErrLen,  Min LowErrLen, avg LowErrLen:\n");
    for (int i = 0; i < foldNum; ++i)
    {
        printf("%*.1f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/5.0), 14, tenFoldLowErrLen[i][LowErrLenMaxIndex[i]], 14, tenFoldLowErrLen[i][LowErrLenMinIndex[i]], 14, tenFoldAvgLowErrLen[i]);
    }
    printf("\n");
}