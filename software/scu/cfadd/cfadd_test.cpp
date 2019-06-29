#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
#include "sobol.hpp"
#include "sobolmulti.hpp"
#include "lfsr.hpp"
#include "lfsrmulti.hpp"
#include "systemrand.hpp"
#include "systemrandmulti.hpp"
#include "synchronizer.hpp"
#include "desynchronizer.hpp"
#include <cstdlib>
#include <ctime>
#include "cfadd.hpp"
#include "perfsim.hpp"

int main()
{
    srand(time(NULL));
    // **************************************************************
    // configuration for computing units
    // **************************************************************
    unsigned int inBSNum = 2; // number of input bit streams
    
    // used in some units with supported architecture
    unsigned int depthSync = 1; // depth of synchronizer
    unsigned int depth = 1; // depth of other buffer

    // **************************************************************
    // different modes for random number generator
    // **************************************************************
    // string mode = "incremental";
    // string mode = "delayed";
    string mode = "random";

    // number of random number sequences
    unsigned int randSeqNum;
    randSeqNum = inBSNum + 2;

    // **************************************************************
    // configuration for evaluation
    // **************************************************************
    unsigned int randBitLen = 8; // number of bits for random number
    unsigned int seqLength = (unsigned int)pow(2,randBitLen); // bit sequence length to be evaluated
    float thdBias = 0.05; // threhold to consider convergence
    unsigned int wSize = seqLength; // window size to monitor accuracy

    // total run number is totalRound * totalIter.
    unsigned int totalRound = 100; // each round uses different random number generator
    unsigned int totalIter = 100; // each iteration uses evaluate different value for a given round


    unsigned int segmentNum = 5; // evaluate the accuracy of different output ranges (segments)


    // **************************************************************
    // recorder definition
    // **************************************************************
    segmentNum += 1; // evaluate the accuracy of different output ranges (segments)
    vector<vector<float>> SegmentedSquaredErr(segmentNum); // squared error for each segment
    vector<vector<float>> SegmentedConvergenceTime(segmentNum); // convergence time for each segment
    vector<vector<unsigned int>> SegmentedNum(segmentNum); // number of runs for each segment
    vector<float> SegmentedAvgSquaredErr(segmentNum); // average squared error (MSE) for each segment
    vector<float> SegmentedAvgConvergenceTime(segmentNum); // average convergence time for each segment

    vector<float> SquaredErrMax(1);
    vector<float> SquaredErrMin(1);
    vector<float> SquaredErrMaxIndex(segmentNum);
    vector<float> SquaredErrMinIndex(segmentNum);

    vector<float> ConvergenceTimeMax(1);
    vector<float> ConvergenceTimeMin(1);
    vector<float> ConvergenceTimeMaxIndex(segmentNum);
    vector<float> ConvergenceTimeMinIndex(segmentNum);

    // initialize the recorder
    for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
    {
        SegmentedSquaredErr[segmentIdx].resize(totalRound);
        SegmentedConvergenceTime[segmentIdx].resize(totalRound);
        SegmentedNum[segmentIdx].resize(totalRound);
        SegmentedAvgSquaredErr[segmentIdx] = 0;
        SegmentedAvgConvergenceTime[segmentIdx] = 0;
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
        // printf("Round: %d out of %d.\n", roundIdx+1, totalRound);
        // initialize the recorder
        for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
        {
            SegmentedSquaredErr[segmentIdx][roundIdx] = 0;
            SegmentedNum[segmentIdx][roundIdx] = 0;
            SegmentedConvergenceTime[segmentIdx][roundIdx] = 0;
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
                    // inRandNum[inIdx][seqIdx] = rngInst.OutSeq()[0][seqIdx%(unsigned int)(pow(2,randBitLen))];
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

            // sync/desync input bs
            Synchronizer SyncInst;
            // DeSynchronizer SyncInst;
            SyncInst.Init(val, 1, wSize, thdBias,"SyncInst");

            CFADD computeInst;
            computeInst.Init(probVec, wSize, thdBias, "computeInst");
            for (int seqIdx = 0; seqIdx < seqLength; ++seqIdx)
            {
                // set bit stream
                for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
                {
                    iBit[inIdx] = num2bitMultiInst.OutSeq()[inIdx][seqIdx];
                }
                // set depthsync and depth random number
                iRandNum[0] = RandSeq[0][seqIdx];
                iRandNum[1] = RandSeq[1][seqIdx];


                // when inBSNum is 2, we can do sync/desync
                // SyncInst.Calc(iBit);
                // computeInst.Calc(SyncInst.OutBit());

                // not doing sync/desync
                computeInst.Calc(iBit);
            }


            // for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
            // {
            //     printf("input prob %d     (%f)\n", inIdx, probVec[inIdx]);
            // }
            // printf("theoretical prob (%f)\n", computeInst.TheoProb()[0]);
            // printf("window prob      (%f)\n", computeInst.WProb()[0]);
            // printf("window bias      (%f)\n", computeInst.WBias()[0]);
            // printf("converge cTime   (%d)\n", computeInst.CTime()[0]);
            

            SegmentedSquaredErr[(unsigned int)floor(computeInst.TheoProb()[0]*(segmentNum-1))][roundIdx] += computeInst.WBias()[0] * computeInst.WBias()[0];
            SegmentedNum[(unsigned int)floor(computeInst.TheoProb()[0]*(segmentNum-1))][roundIdx] += 1;
            SegmentedConvergenceTime[(unsigned int)floor(computeInst.TheoProb()[0]*(segmentNum-1))][roundIdx] += computeInst.CTime()[0];
        }
        for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
        {
            SegmentedSquaredErr[segmentIdx][roundIdx] = sqrt(SegmentedSquaredErr[segmentIdx][roundIdx] / SegmentedNum[segmentIdx][roundIdx]);
            SegmentedConvergenceTime[segmentIdx][roundIdx] = SegmentedConvergenceTime[segmentIdx][roundIdx] / SegmentedNum[segmentIdx][roundIdx];
        }
    }
    clock_t end = clock();

    for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
    {
        SquaredErrMax[0] = SegmentedSquaredErr[segmentIdx][0];
        SquaredErrMin[0] = SegmentedSquaredErr[segmentIdx][0];
        ConvergenceTimeMax[0] = SegmentedConvergenceTime[segmentIdx][0];
        ConvergenceTimeMin[0] = SegmentedConvergenceTime[segmentIdx][0];

        SquaredErrMaxIndex[segmentIdx] = 0;
        SquaredErrMinIndex[segmentIdx] = 0;

        ConvergenceTimeMaxIndex[segmentIdx] = 0;
        ConvergenceTimeMinIndex[segmentIdx] = 0;

        for (int roundIdx = 0; roundIdx < totalRound; ++roundIdx)
        {
            if (isnan(SegmentedSquaredErr[segmentIdx][roundIdx]))
            {
                SegmentedAvgSquaredErr[segmentIdx] += SegmentedAvgSquaredErr[segmentIdx]/(float)(roundIdx+1);
            }
            else
            {
                SegmentedAvgSquaredErr[segmentIdx] += SegmentedSquaredErr[segmentIdx][roundIdx];
            }
            if (isnan(SegmentedConvergenceTime[segmentIdx][roundIdx]))
            {
                SegmentedAvgConvergenceTime[segmentIdx] += SegmentedAvgConvergenceTime[segmentIdx]/(float)(roundIdx+1);
            }
            else
            {
                SegmentedAvgConvergenceTime[segmentIdx] += SegmentedConvergenceTime[segmentIdx][roundIdx];
            }

            if (SquaredErrMax[0] < SegmentedSquaredErr[segmentIdx][roundIdx])
            {
                SquaredErrMax[0] = SegmentedSquaredErr[segmentIdx][roundIdx];
                SquaredErrMaxIndex[segmentIdx] = roundIdx;
            }
            if (SquaredErrMin[0] > SegmentedSquaredErr[segmentIdx][roundIdx])
            {
                SquaredErrMin[0] = SegmentedSquaredErr[segmentIdx][roundIdx];
                SquaredErrMinIndex[segmentIdx] = roundIdx;
            }

            if (ConvergenceTimeMax[0] < SegmentedConvergenceTime[segmentIdx][roundIdx])
            {
                ConvergenceTimeMax[0] = SegmentedConvergenceTime[segmentIdx][roundIdx];
                ConvergenceTimeMaxIndex[segmentIdx] = roundIdx;
            }
            if (ConvergenceTimeMin[0] > SegmentedConvergenceTime[segmentIdx][roundIdx])
            {
                ConvergenceTimeMin[0] = SegmentedConvergenceTime[segmentIdx][roundIdx];
                ConvergenceTimeMinIndex[segmentIdx] = roundIdx;
            }
        }
        SegmentedAvgSquaredErr[segmentIdx] /= totalRound;
        SegmentedAvgConvergenceTime[segmentIdx] /= totalRound;
    }

    double elasped_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("Total execution time: %f\n\n", elasped_secs);

    printf("Computing Unit Configuration\n");
    printf("Number of Input Bit Streams: %4d\n\n", inBSNum);

    printf("Evaluation Configuration:\n");
    printf("Random Number Length:        %4d\n", randBitLen);
    printf("Bit Stream Length:           %4d\n", seqLength);
    printf("Threshold for Convergence:   %*.2f\n", 4, thdBias);
    printf("Window Size for Convergence: %4d\n", wSize);
    printf("Total Round:                 %4d\n", totalRound);
    printf("Iteration Per Round:         %4d\n", totalIter);
    printf("Number of Segment:           %4d\n", segmentNum-1);

    printf("Range, Max Squared Error Rate, Min Squared Error Rate, avg Squared Error Rate:\n");
    for (int i = 0; i < segmentNum; ++i)
    {
        printf("%*.1f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/(segmentNum-1)), 22, SegmentedSquaredErr[i][SquaredErrMaxIndex[i]], 22, SegmentedSquaredErr[i][SquaredErrMinIndex[i]], 22, SegmentedAvgSquaredErr[i]);
    }
    printf("\n");

    printf("Range,   Max Convergence Time,   Min Convergence Time,   avg Convergence Time:\n");
    for (int i = 0; i < segmentNum; ++i)
    {
        printf("%*.1f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/(segmentNum-1)), 22, SegmentedConvergenceTime[i][ConvergenceTimeMaxIndex[i]], 22, SegmentedConvergenceTime[i][ConvergenceTimeMinIndex[i]], 22, SegmentedAvgConvergenceTime[i]);
    }
    printf("\n");
}
