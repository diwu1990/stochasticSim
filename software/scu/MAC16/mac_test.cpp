#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
#include "sobol.hpp"
#include "sobolmulti.hpp"
#include "racel.hpp"
#include "racelmulti.hpp"
#include "lfsr.hpp"
#include "lfsrmulti.hpp"
#include "systemrand.hpp"
#include "systemrandmulti.hpp"
#include "synchronizer.hpp"
#include "desynchronizer.hpp"
#include <cstdlib>
#include <ctime>
#include "cfadd.hpp"
#include "cfmul.hpp"
#include "muxadd.hpp"
#include "oradd.hpp"
#include "perfsim.hpp"


#define UNI
// #define BI

// #define SADD
#define NSADD

// #define UGEMM
#define GGEMM
// #define DGEMM
// #define TGEMM


#define RC
// #define TC




int main()
{
    #ifdef UNI
        printf("unipolar-");
    #endif
    #ifdef BI
        printf("bipolar-");
    #endif

    #ifdef SADD
        printf("scaled-");
    #endif
    #ifdef NSADD
        printf("nonscaled-");
    #endif

    #ifdef UGEMM
        printf("uGEMM-");
    #endif
    #ifdef DGEMM
        printf("dGEMM-");
    #endif
    #ifdef TGEMM
        printf("tGEMM-");
    #endif
    #ifdef GGEMM
        printf("gGEMM-");
    #endif

    #ifdef RC
        printf("rate\n");
    #endif
    #ifdef TC
        printf("temporal\n");
    #endif

    srand(time(NULL));
    vector<unsigned int> inBSNumVec{32};
    vector<unsigned int> randBitLenVec{8};
    // Addition option: scaled_add
    // 0 for non-scaled
    // 1 for scaled
    unsigned int scaled_add = 0;
    #ifdef SADD
        scaled_add = 1;
    #endif
    #ifdef NSADD
        scaled_add = 0;
    #endif
    // data format: unipolar
    // 0 for bipolar
    // 1 for unipolar
    unsigned int unipolar = 1;
    #ifdef UNI
        unipolar = 1;
    #endif
    #ifdef BI
        unipolar = 0;
    #endif


    // dont change code here!!!!!!!!!!!!!!!!!!!
    // instream option: always non-instream
    unsigned int instream = 0;
    // whether to set cfmul to correlation free mode
    // dGEMM and gGEMM are cfree, uGEMM and tGEMM are not
    unsigned int cfree = 0;
    #ifdef UGEMM
        cfree = 1;
    #endif
    #ifdef TGEMM
        cfree = 1;
    #endif
    // threhold to consider convergence
    float thdBias = 0.05;


    for (int inBSNumVecIdx = 0; inBSNumVecIdx < inBSNumVec.size(); ++inBSNumVecIdx)
    {
        for (int randBitLenVecIdx = 0; randBitLenVecIdx < randBitLenVec.size(); ++randBitLenVecIdx)
        {
            // **************************************************************
            // configuration for computing units
            // **************************************************************
            unsigned int inBSNum = inBSNumVec[inBSNumVecIdx]; // number of input bit streams
            unsigned int mulNum = inBSNum/2; // number of muls
            // printf("number of input is %d\n", inBSNum);
            // printf("number of multiplers is %d\n", mulNum);
            unsigned int depthSync = (unsigned int)log2(mulNum); // depth of synchronizer, used in some units with supported architecture
            depthSync = randBitLenVec[randBitLenVecIdx];
            // printf("depthSync is %d\n", depthSync);
            unsigned int inSWindow = ceil(depthSync/2); // shift offset for internal cnt
            unsigned int depth = inSWindow; // depth of other buffer, used in some units with supported architecture
            // **************************************************************
            // configuration for evaluation
            // **************************************************************
            unsigned int randBitLen = randBitLenVec[randBitLenVecIdx]; // number of bits for random number
            // printf("randBitLen is %d\n", randBitLen);
            // total run number is totalRound * totalIter.
            unsigned int totalRound = 1; // each round uses different random number generator
            unsigned int totalIter = 500; // each iteration uses evaluate different value for a given round

            // **************************************************************
            // different modes for random number generator
            // **************************************************************
            string mode = "random"; // "incremental", "delayed", "random"
            unsigned int seqLength = (unsigned int)pow(2,randBitLen); // bit sequence length to be evaluated
            // printf("seqLength is %d\n", seqLength);
            unsigned int wSize = seqLength; // window size to monitor accuracy
            unsigned int segmentNum = 1; // evaluate the accuracy of different output ranges (segments)
            unsigned int randSeqNum = 2; // number of random number sequences for depthSync/depth
            // **************************************************************
            // recorder definition
            // **************************************************************
            segmentNum += 1; // evaluate the accuracy of different output ranges (segments)
            unsigned int segNum; // check which output segment to store data
            vector<vector<float>> SegmentedRSE(segmentNum); // squared error for each segment
            vector<vector<float>> SegmentedConvergenceTime(segmentNum); // convergence time for each segment
            vector<vector<unsigned int>> SegmentedNum(segmentNum); // number of runs for each segment
            vector<float> SegmentedAvgRSE(segmentNum); // average squared error (MSE) for each segment
            vector<float> SegmentedAvgConvergenceTime(segmentNum); // average convergence time for each segment
            vector<unsigned int> SegmentedTotalNum(segmentNum); // average squared error (MSE) for each segment

            vector<float> CycleAvgRSE(seqLength);
            for (int seqIdx = 0; seqIdx < seqLength; ++seqIdx)
            {
                CycleAvgRSE[seqIdx] = 0;
            }

            vector<float> RSEMax(1);
            vector<float> RSEMin(1);
            vector<float> RSEMaxIndex(segmentNum);
            vector<float> RSEMinIndex(segmentNum);

            vector<float> ConvergenceTimeMax(1);
            vector<float> ConvergenceTimeMin(1);
            vector<float> ConvergenceTimeMaxIndex(segmentNum);
            vector<float> ConvergenceTimeMinIndex(segmentNum);

            // initialize the recorder
            for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
            {
                SegmentedRSE[segmentIdx].resize(totalRound);
                SegmentedConvergenceTime[segmentIdx].resize(totalRound);
                SegmentedNum[segmentIdx].resize(totalRound);
                SegmentedAvgRSE[segmentIdx] = 0;
                SegmentedAvgConvergenceTime[segmentIdx] = 0;
                SegmentedTotalNum[segmentIdx] = 0;
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
                    SegmentedRSE[segmentIdx][roundIdx] = 0;
                    SegmentedNum[segmentIdx][roundIdx] = 0;
                    SegmentedConvergenceTime[segmentIdx][roundIdx] = 0;
                }
                unsigned int seedInitIdx = 1+roundIdx;
                unsigned int delay = 0;
                // random number generator
                // SystemRandMulti rngInst;
                #ifdef TGEMM
                    RACELMulti rngInst;
                #else
                    #ifdef RC
                        LFSRMulti rngInst;
                        // SOBOLMulti rngInst;
                    #endif
                    #ifdef TC
                        RACELMulti rngInst;
                    #endif
                #endif
                // LFSRMulti rngInst;
                // RACELMulti rngInst;
                // printf("%d, %d, %d, %d, \n", GEMM_mode, scaled_add, rate_coding, unipolar);

                rngInst.Init(inBSNum,seedInitIdx,delay,randBitLen,mode,"rngInst");
                rngInst.SeqGen();

                // random number generator
                // SystemRandMulti rngSeqInst;
                SOBOLMulti rngSeqInst;
                // LFSRMulti rngSeqInst;
                rngSeqInst.Init(randSeqNum,seedInitIdx,delay,randBitLen,mode,"rngSeqInst");
                rngSeqInst.SeqGen();

                for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
                {
                    inRandNum[inIdx].resize(seqLength);
                }

                for (int iter = 0; iter < totalIter; ++iter)
                {
                    // generate binary input probabilistic data, with required precision
                    for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
                    {
                        if (unipolar == 0)
                        {
                            val[inIdx] = (float)(2*((float)(rand()%(int)pow(2,randBitLen))/(float)pow(2,randBitLen))-1);
                        }
                        else
                        {
                            val[inIdx] = (float)((float)(rand()%(int)pow(2,randBitLen))/(float)pow(2,randBitLen));
                            if (scaled_add == 0)
                            {
                                if (rand()%256>(256/(int)mulNum/4))
                                {
                                    val[inIdx] /= (float)max((int)mulNum+8,1);
                                    val[inIdx] = (float)((int)(val[inIdx]*pow(2,randBitLen)))/pow(2,randBitLen);
                                }
                            }
                        }
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
                            // printf("%d\n", inRandNum[inIdx][seqIdx]);
                        }
                    }

                    // generate random bits of each input BS
                    RandNum2BitMulti num2bitMultiInst;
                    num2bitMultiInst.Init(probVec,bitLengthVec,inRandNum,unipolar,"num2bitMultiInst");
                    num2bitMultiInst.SeqGen();

                    // generate random sequence for depth and depthsync
                    for (int seqIdx = 0; seqIdx < seqLength; ++seqIdx)
                    {
                        // RandSeq[0][seqIdx] = rngInst.OutSeq()[0][seqIdx%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - depthSync);
                        RandSeq[0][seqIdx] = rngSeqInst.OutSeq()[0][seqIdx%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - 4);
                        RandSeq[1][seqIdx] = rngSeqInst.OutSeq()[1][seqIdx%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - (unsigned int)log2(depth));
                    }

                    // all mul are included in mulInst, CFMUL
                    vector<CFMUL> mulInst(mulNum);
                    vector<float> mulProbVec(2);
                    for (int mulIdx = 0; mulIdx < mulNum; ++mulIdx)
                    {
                        mulProbVec[0] = probVec[mulIdx*2];
                        mulProbVec[1] = probVec[mulIdx*2+1];
                        mulInst[mulIdx].Init(mulProbVec, cfree, randBitLen, instream, inSWindow, wSize, thdBias, unipolar, "mulInst");
                        // printf("%.5f, ", mulInst[mulIdx].TheoProb()[0]);
                    }
                    // printf("\n");

                    // not all mul are included in computeInst, CFADD
                    vector<float> addProbVec(mulNum);
                    #ifdef UGEMM
                        CFADD computeInst;
                        for (int mulIdx = 0; mulIdx < mulNum; ++mulIdx)
                        {
                            addProbVec[mulIdx] = mulInst[mulIdx].TheoProb()[0];
                            // printf("%.5f, ", addProbVec[mulIdx]);
                        }
                        // printf("\n");
                        computeInst.Init(addProbVec, scaled_add, randBitLen, wSize, thdBias, unipolar, "computeInst");
                    #else
                        #ifdef SADD
                            MUXADD computeInst;
                            for (int mulIdx = 0; mulIdx < mulNum; ++mulIdx)
                            {
                                addProbVec[mulIdx] = mulInst[mulIdx].TheoProb()[0];
                            }
                            computeInst.Init(addProbVec, wSize, thdBias, unipolar, "computeInst");
                        #endif
                        #ifdef NSADD
                            #ifdef BI
                                printf("Error: Non UGEMM mode should not have bipolar input with nonscaled add!!!!!!\n");
                            #endif
                            ORADD computeInst;
                            for (int mulIdx = 0; mulIdx < mulNum; ++mulIdx)
                            {
                                addProbVec[mulIdx] = mulInst[mulIdx].TheoProb()[0];
                            }
                            computeInst.Init(probVec, wSize, thdBias, "computeInst");
                        #endif
                    #endif

                    vector<char> addInBit(mulNum);
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

                        vector<char> mulInBit(2);
                        for (int mulIdx = 0; mulIdx < mulNum; ++mulIdx)
                        {
                            mulInBit[0] = iBit[2*mulIdx];
                            mulInBit[1] = iBit[2*mulIdx+1];
                            mulInst[mulIdx].Calc(mulInBit);
                            addInBit[mulIdx] = mulInst[mulIdx].OutBit()[0];
                        }


                        #ifdef UGEMM
                            computeInst.Calc(addInBit);
                        #else
                            #ifdef SADD
                                computeInst.Calc(addInBit, iRandNum);
                            #endif
                            #ifdef NSADD
                                computeInst.Calc(addInBit);
                            #endif
                        #endif
                        
                        CycleAvgRSE[seqIdx] += computeInst.WBias()[0] * computeInst.WBias()[0];

                    }


                    // for (int inIdx = 0; inIdx < inBSNum; ++inIdx)
                    // {
                    //     printf("input prob %d     (%f)\n", inIdx, probVec[inIdx]);
                    // }
                    // printf("theoretical prob (%f)\n", computeInst.TheoProb()[0]);
                    // printf("window prob      (%f)\n", computeInst.WProb()[0]);
                    // printf("window bias      (%f)\n", computeInst.WBias()[0]);
                    // printf("converge cTime   (%d)\n", computeInst.CTime()[0]);
                    
                    if (unipolar == 0)
                    {
                        segNum = (unsigned int)floor((computeInst.TheoProb()[0]+1)/2*(segmentNum-1));
                    }
                    else
                    {
                        segNum = (unsigned int)floor(computeInst.TheoProb()[0]*(segmentNum-1));
                    }
                    SegmentedRSE[segNum][roundIdx] += computeInst.WBias()[0] * computeInst.WBias()[0];
                    SegmentedNum[segNum][roundIdx] += 1;
                    SegmentedConvergenceTime[segNum][roundIdx] += computeInst.CTime()[0];
                 

                }
                for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
                {
                    SegmentedRSE[segmentIdx][roundIdx] = sqrt(SegmentedRSE[segmentIdx][roundIdx] / SegmentedNum[segmentIdx][roundIdx]);
                    SegmentedConvergenceTime[segmentIdx][roundIdx] = SegmentedConvergenceTime[segmentIdx][roundIdx] / SegmentedNum[segmentIdx][roundIdx];
                }
            }
            clock_t end = clock();

            for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
            {
                RSEMax[0] = SegmentedRSE[segmentIdx][0];
                RSEMin[0] = SegmentedRSE[segmentIdx][0];
                ConvergenceTimeMax[0] = SegmentedConvergenceTime[segmentIdx][0];
                ConvergenceTimeMin[0] = SegmentedConvergenceTime[segmentIdx][0];

                RSEMaxIndex[segmentIdx] = 0;
                RSEMinIndex[segmentIdx] = 0;

                ConvergenceTimeMaxIndex[segmentIdx] = 0;
                ConvergenceTimeMinIndex[segmentIdx] = 0;

                for (int roundIdx = 0; roundIdx < totalRound; ++roundIdx)
                {
                    if (isnan(SegmentedRSE[segmentIdx][roundIdx]))
                    {
                        SegmentedAvgRSE[segmentIdx] += SegmentedAvgRSE[segmentIdx]/(float)(roundIdx+1);
                    }
                    else
                    {
                        SegmentedAvgRSE[segmentIdx] += SegmentedRSE[segmentIdx][roundIdx];
                    }
                    if (isnan(SegmentedConvergenceTime[segmentIdx][roundIdx]))
                    {
                        SegmentedAvgConvergenceTime[segmentIdx] += SegmentedAvgConvergenceTime[segmentIdx]/(float)(roundIdx+1);
                    }
                    else
                    {
                        SegmentedAvgConvergenceTime[segmentIdx] += SegmentedConvergenceTime[segmentIdx][roundIdx];
                    }
                    SegmentedTotalNum[segmentIdx] += SegmentedNum[segmentIdx][roundIdx];

                    if (RSEMax[0] < SegmentedRSE[segmentIdx][roundIdx])
                    {
                        RSEMax[0] = SegmentedRSE[segmentIdx][roundIdx];
                        RSEMaxIndex[segmentIdx] = roundIdx;
                    }
                    if (RSEMin[0] > SegmentedRSE[segmentIdx][roundIdx])
                    {
                        RSEMin[0] = SegmentedRSE[segmentIdx][roundIdx];
                        RSEMinIndex[segmentIdx] = roundIdx;
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
                SegmentedAvgRSE[segmentIdx] /= totalRound;
                SegmentedAvgConvergenceTime[segmentIdx] /= totalRound;
            }

            double elasped_secs = double(end - begin) / CLOCKS_PER_SEC;
            printf("============================================================================================\n");
            printf("Total execution time: %f\n\n", elasped_secs);

            printf("Computing Unit Configuration\n");
            printf("Number of Input Bit Streams: %4d\n", inBSNum);
            printf("Depth of Sync:               %4d\n", depthSync);
            printf("Depth of Buffer:             %4d\n", depth);
            printf("Correlation Free Enbale:     %4d\n", cfree!=0);
            printf("In-Stream Enbale:            %4d\n", instream!=0);
            printf("Unipolar Enbale:             %4d\n", unipolar!=0);
            printf("Scaled Addition Enbale:      %4d\n\n", scaled_add!=0);

            printf("Evaluation Configuration:\n");
            printf("Random Number Length:        %4d\n", randBitLen);
            printf("Bit Stream Length:           %4d\n", seqLength);
            printf("Threshold for Convergence:   %*.2f\n", 4, thdBias);
            printf("Window Size for Convergence: %4d\n", wSize);
            printf("Total Round:                 %4d\n", totalRound);
            printf("Iteration Per Round:         %4d\n", totalIter);
            printf("Number of Segment:           %4d\n\n", segmentNum-1);

            printf("Range,  Total Num, Max Squared Error Rate,  Min Squared Error Rate,  avg Squared Error Rate:\n");
            for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
            {
                if (unipolar == 0)
                {
                    printf("%*.1f, %10d, %*.5f, %*.5f, %*.5f\n", 5, 2*((float)segmentIdx/(segmentNum-1))-1, SegmentedTotalNum[segmentIdx], 23, SegmentedRSE[segmentIdx][RSEMaxIndex[segmentIdx]], 23, SegmentedRSE[segmentIdx][RSEMinIndex[segmentIdx]], 23, SegmentedAvgRSE[segmentIdx]);
                }
                else
                {
                    printf("%*.1f, %10d, %*.5f, %*.5f, %*.5f\n", 5, ((float)segmentIdx/(segmentNum-1)), SegmentedTotalNum[segmentIdx], 23, SegmentedRSE[segmentIdx][RSEMaxIndex[segmentIdx]], 23, SegmentedRSE[segmentIdx][RSEMinIndex[segmentIdx]], 23, SegmentedAvgRSE[segmentIdx]);
                }
            }
            printf("\n");

            printf("Range,  Total Num,    Max Convergence Time,    Min Convergence Time,    avg Convergence Time:\n");
            for (int segmentIdx = 0; segmentIdx < segmentNum; ++segmentIdx)
            {
                if (unipolar == 0)
                {
                    printf("%*.1f, %10d, %*.5f, %*.5f, %*.5f\n", 5, 2*((float)segmentIdx/(segmentNum-1))-1, SegmentedTotalNum[segmentIdx], 23, SegmentedConvergenceTime[segmentIdx][ConvergenceTimeMaxIndex[segmentIdx]], 23, SegmentedConvergenceTime[segmentIdx][ConvergenceTimeMinIndex[segmentIdx]], 23, SegmentedAvgConvergenceTime[segmentIdx]);
                }
                else
                {
                    printf("%*.1f, %10d, %*.5f, %*.5f, %*.5f\n", 5, ((float)segmentIdx/(segmentNum-1)), SegmentedTotalNum[segmentIdx], 23, SegmentedConvergenceTime[segmentIdx][ConvergenceTimeMaxIndex[segmentIdx]], 23, SegmentedConvergenceTime[segmentIdx][ConvergenceTimeMinIndex[segmentIdx]], 23, SegmentedAvgConvergenceTime[segmentIdx]);
                }
            }
            printf("============================================================================================\n");
            printf("\n");

            for (int seqIdx = 0; seqIdx < seqLength; ++seqIdx)
            {
                CycleAvgRSE[seqIdx] = sqrt(CycleAvgRSE[seqIdx] / totalRound / totalIter);
                printf("%f\n", CycleAvgRSE[seqIdx]);
            }
        }
    }
}
