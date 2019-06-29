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
#include "cfadd.hpp"
#include "perfsim.hpp"

int main()
{
    srand(time(NULL));
    // **************************************************************
    // different modes for random number generator
    // **************************************************************
    // string mode = "incremental";
    // string mode = "delayed";
    string mode = "random";
    unsigned int randSeqNum = 4; // number of random number sequences


    // **************************************************************
    // configuration for evaluation
    // **************************************************************
    unsigned int randBitLen = 8; // number of bits for random number
    unsigned int seqLength = (unsigned int)pow(2,randBitLen); // bit sequence length to be evaluated
    float thdBias = 0.05; // threhold to consider convergence
    unsigned int wSize = seqLength; // window size to monitor accuracy

    // total run number is totalRound * totalIter.
    unsigned int totalRound = 1000; // each round uses different random number generator
    unsigned int totalIter = 1000; // each iteration uses evaluate different value for a given round


    // **************************************************************
    // configuration for computing units
    // **************************************************************
    unsigned int inBSNum = 2; // number of input bit streams
    
    // used in some units with supported architecture
    unsigned int depth = 1; // depth of other buffer
    unsigned int depthSync = 8; // depth of synchronizer


    // **************************************************************
    // recorder definition
    // **************************************************************
    unsigned int foldNum = 6; // evaluate the accuracy of different output ranges (segments)
    vector<vector<float>> SegmentedSquaredErr(foldNum); // squared error for each segment
    vector<vector<float>> SegmentedConvergenceTime(foldNum); // convergence time for each segment
    vector<vector<unsigned int>> SegmentedNum(foldNum); // number of runs for each segment
    vector<float> SegmentedAvgSquaredErr(foldNum); // average squared error (MSE) for each segment
    vector<float> SegmentedAvgConvergenceTime(foldNum); // average convergence time for each segment

    vector<float> SquaredErrMax(1);
    vector<float> SquaredErrMin(1);
    vector<float> SquaredErrMaxIndex(foldNum);
    vector<float> SquaredErrMinIndex(foldNum);

    vector<float> ConvergenceTimeMax(1);
    vector<float> ConvergenceTimeMin(1);
    vector<float> ConvergenceTimeMaxIndex(foldNum);
    vector<float> ConvergenceTimeMinIndex(foldNum);

    // initialize the recorder
    for (int foldIdx = 0; foldIdx < foldNum; ++foldIdx)
    {
        SegmentedSquaredErr[foldIdx].resize(totalRound);
        SegmentedConvergenceTime[foldIdx].resize(totalRound);
        SegmentedNum[foldIdx].resize(totalRound);
        SegmentedAvgSquaredErr[foldIdx] = 0;
        SegmentedAvgConvergenceTime[foldIdx] = 0;
    }

    vector<char> iBit(inBSNum);
    vector<vector<unsigned int>> RandSeq(2);
    RandSeq[0].resize(seqLength);
    RandSeq[1].resize(seqLength);
    vector<unsigned int> iRandNum(2);
    
    vector<unsigned int> bitLengthVec(inBSNum);
    vector<float> probVec(inBSNum);
    vector<float> val(inBSNum);
    vector<vector<unsigned int>> inRandNum(inBSNum);

    clock_t begin = clock();
    for (int roundIdx = 0; roundIdx < totalRound; ++roundIdx)
    {
        // initialize the recorder
        for (int foldIdx = 0; foldIdx < foldNum; ++foldIdx)
        {
            SegmentedSquaredErr[foldIdx][roundIdx] = 0;
            SegmentedNum[foldIdx][roundIdx] = 0;
            SegmentedConvergenceTime[foldIdx][roundIdx] = 0;
        }
        unsigned int seedInitIdx = 1+roundIdx;
        unsigned int delay = 0;
        // random number generator
        // SystemRandMulti rngInst;
        // SOBOLMulti rngInst;
        LFSRMulti rngInst;
        rngInst.Init(randSeqNum,seedInitIdx,delay,randBitLen,mode,"rngInst");
        rngInst.SeqGen();


        for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
        {
            inRandNum[inIdx].resize(seqLength);
        }

        for (int iter = 0; iter < totalIter; ++iter)
        {
            // generate binary input probabilistic data, with required precision
            for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
            {
                val[inIdx] = (float)((float)(rand()%(int)pow(2,randBitLen))/(float)pow(2,randBitLen));
                bitLengthVec[inIdx] = randBitLen;
                probVec[inIdx] = val[inIdx];
            }
            
            // generate random number for input bit stream or regeneration
            for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
            {
                for (int seqIdx = 0; seqIdx < seqLength; ++seqIdx)
                {
                    inRandNum[inIdx][seqIdx] = rngInst.OutSeq()[inIdx][seqIdx%(unsigned int)(pow(2,randBitLen))];
                }
            }

            // generate random bits of each input BS
            RandNum2BitMulti num2bitMultiInst;
            num2bitMultiInst.Init(probVec,bitLengthVec,inRandNum,"num2bitMultiInst");
            num2bitMultiInst.SeqGen();

            // generate random sequence for depth and depthsync
            for (int seqIdx = 0; seqIdx < seqLength; ++seqIdx)
            {
                RandSeq[0][seqIdx] = rngInst.OutSeq()[inBSNum][seqIdx%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - depthSync);
                RandSeq[1][seqIdx] = rngInst.OutSeq()[inBSNum+1][seqIdx%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - (unsigned int)log2(depth));
            }

            CFADD computeInst;
            computeInst.Init(probVec, wSize, thdBias, "computeInst");
            for (int seqIdx = 0; seqIdx < seqLength; ++seqIdx)
            {
                for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
                {
                    iBit[inIdx] = num2bitMultiInst.OutSeq()[inIdx][seqIdx];
                }
                iRandNum[0] = RandSeq[0][seqIdx];
                iRandNum[1] = RandSeq[1][seqIdx];
                computeInst.Calc(iBit);
            }
            printf("input prob       (%f)\n", probVec[0]);
            printf("theoretical prob (%f)\n", computeInst.TheoProb()[0]);
            printf("window prob      (%f)\n", computeInst.WProb()[0]);
            printf("window bias      (%f)\n", computeInst.WBias()[0]);
            printf("converge cTime   (%d)\n", computeInst.CTime()[0]);
            

            SegmentedSquaredErr[(unsigned int)floor(computeInst.TheoProb()[0]*5)][roundIdx] += computeInst.WBias()[0] * computeInst.WBias()[0];
            SegmentedNum[(unsigned int)floor(computeInst.TheoProb()[0]*5)][roundIdx] += 1;
            SegmentedConvergenceTime[(unsigned int)floor(computeInst.TheoProb()[0]*5)][roundIdx] += computeInst.CTime()[0];
        }
        for (int foldIdx = 0; foldIdx < foldNum; ++foldIdx)
        {
            SegmentedSquaredErr[foldIdx][roundIdx] = sqrt(SegmentedSquaredErr[foldIdx][roundIdx] / SegmentedNum[foldIdx][roundIdx]);
            SegmentedConvergenceTime[foldIdx][roundIdx] = SegmentedConvergenceTime[foldIdx][roundIdx] / SegmentedNum[foldIdx][roundIdx];
        }
    }
    clock_t end = clock();

    for (int foldIdx = 0; foldIdx < foldNum; ++foldIdx)
    {
        SquaredErrMax[0] = SegmentedSquaredErr[foldIdx][0];
        SquaredErrMin[0] = SegmentedSquaredErr[foldIdx][0];
        ConvergenceTimeMax[0] = SegmentedConvergenceTime[foldIdx][0];
        ConvergenceTimeMin[0] = SegmentedConvergenceTime[foldIdx][0];

        SquaredErrMaxIndex[foldIdx] = 0;
        SquaredErrMinIndex[foldIdx] = 0;

        ConvergenceTimeMaxIndex[foldIdx] = 0;
        ConvergenceTimeMinIndex[foldIdx] = 0;

        for (int roundIdx = 0; roundIdx < totalRound; ++roundIdx)
        {
            if (isnan(SegmentedSquaredErr[foldIdx][roundIdx]))
            {
                SegmentedAvgSquaredErr[foldIdx] += SegmentedAvgSquaredErr[foldIdx]/(float)(roundIdx+1);
            }
            else
            {
                SegmentedAvgSquaredErr[foldIdx] += SegmentedSquaredErr[foldIdx][roundIdx];
            }
            if (isnan(SegmentedConvergenceTime[foldIdx][roundIdx]))
            {
                SegmentedAvgConvergenceTime[foldIdx] += SegmentedAvgConvergenceTime[foldIdx]/(float)(roundIdx+1);
            }
            else
            {
                SegmentedAvgConvergenceTime[foldIdx] += SegmentedConvergenceTime[foldIdx][roundIdx];
            }

            if (SquaredErrMax[0] < SegmentedSquaredErr[foldIdx][roundIdx])
            {
                SquaredErrMax[0] = SegmentedSquaredErr[foldIdx][roundIdx];
                SquaredErrMaxIndex[foldIdx] = roundIdx;
            }
            if (SquaredErrMin[0] > SegmentedSquaredErr[foldIdx][roundIdx])
            {
                SquaredErrMin[0] = SegmentedSquaredErr[foldIdx][roundIdx];
                SquaredErrMinIndex[foldIdx] = roundIdx;
            }

            if (ConvergenceTimeMax[0] < SegmentedConvergenceTime[foldIdx][roundIdx])
            {
                ConvergenceTimeMax[0] = SegmentedConvergenceTime[foldIdx][roundIdx];
                ConvergenceTimeMaxIndex[foldIdx] = roundIdx;
            }
            if (ConvergenceTimeMin[0] > SegmentedConvergenceTime[foldIdx][roundIdx])
            {
                ConvergenceTimeMin[0] = SegmentedConvergenceTime[foldIdx][roundIdx];
                ConvergenceTimeMinIndex[foldIdx] = roundIdx;
            }
        }
        SegmentedAvgSquaredErr[foldIdx] /= totalRound;
        SegmentedAvgConvergenceTime[foldIdx] /= totalRound;
    }

    double elasped_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("Total execution time: %f\n\n", elasped_secs);

    printf("Range, Max Error Rate, Min Error Rate, avg Error Rate:\n");
    for (int i = 0; i < foldNum; ++i)
    {
        printf("%*.1f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/5.0), 14, SegmentedSquaredErr[i][SquaredErrMaxIndex[i]], 14, SegmentedSquaredErr[i][SquaredErrMinIndex[i]], 14, SegmentedAvgSquaredErr[i]);
    }
    printf("\n");

    printf("Range,  Max ConvergenceTime,  Min ConvergenceTime, avg ConvergenceTime:\n");
    for (int i = 0; i < foldNum; ++i)
    {
        printf("%*.1f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/5.0), 14, SegmentedConvergenceTime[i][ConvergenceTimeMaxIndex[i]], 14, SegmentedConvergenceTime[i][ConvergenceTimeMinIndex[i]], 14, SegmentedAvgConvergenceTime[i]);
    }
    printf("\n");
}
