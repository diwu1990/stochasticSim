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
#include "squash.hpp"
#include "perfsim.hpp"

int main()
{
    srand(time(NULL));
    unsigned int inBS = 16;
    unsigned int inRand = 2;
    unsigned int randSeqNum = inBS + inRand;
    unsigned int randBitLen = 8;
    string mode = "incremental";
    // string mode = "delayed";

    unsigned int foldNum = 11;
    vector<float> tenFoldErr(foldNum);
    vector<float> tenFoldBias(foldNum);
    vector<unsigned int> tenFoldNum(foldNum);
    vector<float> tenFoldLowErrLen(foldNum);
    
    unsigned int seqLength = (unsigned int)pow(2,randBitLen);
    float thdBias = 0.05;
    unsigned int wSize = seqLength/2;
    
    unsigned int totalRound = 1;
    unsigned int totalIter = 1;

    unsigned int depth;
    unsigned int depthSync;

    vector<unsigned int> bitLengthVec(inBS);
    vector<float> probVec(inBS);
    vector<float> val(inBS);
    vector<vector<unsigned int>> inRandNum(inBS);
    vector<vector<unsigned int>> RandSeq(inRand);

    vector<char> iBit(inBS);
    vector<unsigned int> iRandNum(inRand);

    clock_t begin = clock();

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

        depth = 2;
        depthSync = 5;
        for (int iter = 0; iter < totalIter; ++iter)
        {
            /* code */
            float prob0 = (float)((float)(rand()%(int)pow(2,randBitLen))/(float)pow(2,randBitLen));
            float prob1 = (float)((float)(rand()%(int)pow(2,randBitLen))/(float)pow(2,randBitLen));
            val[0] = prob0;
            val[1] = prob1;

            for (int l = 0; l < inBS; ++l)
            {
                bitLengthVec[l] = randBitLen;
                probVec[l] = (float)((float)(rand()%(int)pow(2,randBitLen))/(float)pow(2,randBitLen));
<<<<<<< HEAD
                // probVec[l] = 10;
=======
                probVec[l] = 0.9;
>>>>>>> fb40e4a48a27a85e3dd4ddb351a5e5f34e38b39e
            }
            
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

            for (int i = 0; i < inRand; ++i)
            {
                RandSeq[i].resize(seqLength);
            }

            // *****************************************************************************
            // mannually config
            // *****************************************************************************
            for (int z = 0; z < seqLength; ++z)
            {
                RandSeq[0][z] = rngInst.OutSeq()[inBS][z%(unsigned int)(pow(2,randBitLen))] >> depthSync;
                RandSeq[1][z] = rngInst.OutSeq()[inBS+1][z%(unsigned int)(pow(2,randBitLen))] >> (randBitLen - (unsigned int)log2(depth));
            }

            // // binary
            // vector<float> sqreTemp(inBS);
            // vector<float> sumTemp(1);
            // vector<float> sqrtTemp(1);
            // vector<float> divTemp(1);
            // vector<float> mulTemp(inBS);
            // sumTemp[0] = 0;
            // for (int cnt = 0; cnt < inBS; ++cnt)
            // {
            //     sqreTemp[cnt] = probVec[cnt] * probVec[cnt];
            //     printf("sqre: %f\n", sqreTemp[cnt]);
            //     sumTemp[0] += sqreTemp[cnt];
            // }
            // printf("sum: %f\n", sumTemp[0]);
            // sqrtTemp[0] = sqrt(sumTemp[0]);
            // printf("sqrt: %f\n", sqrtTemp[0]);
            // divTemp[0] = sqrtTemp[0]/(1+sumTemp[0]);
            // printf("div: %f\n", divTemp[0]);
            // for (int cnt = 0; cnt < inBS; ++cnt)
            // {
            //     mulTemp[cnt] = divTemp[0] * probVec[cnt];
            //     printf("output: %f\n", mulTemp[cnt]);
            // }
            // printf("\n");


            SQUASH computeInst;
<<<<<<< HEAD
            computeInst.Init(probVec, 1/(float)inBS/(float)inBS, depthSync, depth, wSize, thdBias, "computeInst");
            // for (int j = 0; j < seqLength; ++j)
            // {
            //     for (int z = 0; z < inBS; ++z)
            //     {
            //         iBit[z] = num2bitMultiInst.OutSeq()[z][j];
            //     }
            //     for (int i = 0; i < inRand; ++i)
            //     {
            //         iRandNum[i] = RandSeq[i][j];
            //     }
            //     // printf("%d,%d\n", iRandNum[0], iRandNum[1]);
            //     computeInst.Calc(iBit,iRandNum);
            //     // printf("%d: (%u)=>(%u)\n", j, iBit[0], computeInst.OutBit()[0]);
            // }
=======
            computeInst.Init(probVec, 0.5/(float)inBS, depthSync, depth, wSize, thdBias, "computeInst");
            for (int j = 0; j < seqLength; ++j)
            {
                for (int z = 0; z < inBS; ++z)
                {
                    iBit[z] = num2bitMultiInst.OutSeq()[z][j];
                }
                for (int i = 0; i < inRand; ++i)
                {
                    iRandNum[i] = RandSeq[i][j];
                }
                // printf("%d,%d\n", iRandNum[0], iRandNum[1]);
                computeInst.Calc(iBit,iRandNum);
                // printf("%d: (%u)=>(%u)\n", j, iBit[0], computeInst.OutBit()[0]);
            }
>>>>>>> fb40e4a48a27a85e3dd4ddb351a5e5f34e38b39e
            // printf("input prob       (%f)\n", probVec[0]);
            // printf("theoretical prob (%f)\n",computeInst.TheoProb()[0]);
            // printf("window prob      (%f)\n",computeInst.WProb()[0]);
            // printf("window bias      (%f)\n",computeInst.WBias()[0]);
            // printf("converge speed   (%d)\n",computeInst.Speed()[0]);

            // tenFoldErr[(unsigned int)floor(computeInst.TheoProb()[0]*10)] += computeInst.WBias()[0] * computeInst.WBias()[0];
            // tenFoldBias[(unsigned int)floor(computeInst.TheoProb()[0]*10)] += computeInst.WBias()[0];
            // tenFoldNum[(unsigned int)floor(computeInst.TheoProb()[0]*10)] += 1;
            // tenFoldLowErrLen[(unsigned int)floor(computeInst.TheoProb()[0]*10)] += computeInst.Speed()[0];
        }
    }
    clock_t end = clock();
    double elasped_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("Elasped Seconds: %f\n", elasped_secs);

    for (int y = 0; y < foldNum; ++y)
    {
        tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
        tenFoldBias[y] = tenFoldBias[y]/tenFoldNum[y];
        tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
        // tenFoldCorr[y] = tenFoldCorr[y]/tenFoldNum[y];
    }
    
    // printf("Range, Freq, Correlation, Error Rate, Stat Bias, LowErrLen:\n");
    // printf("Range, Freq, Error Rate, Stat Bias, LowErrLen:\n");
    // for (int i = 0; i < foldNum; ++i)
    // {
    //     // printf("%*.1f, %*u, %*.4f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/10.0), 4, tenFoldNum[i], 11, tenFoldCorr[i], 10, tenFoldErr[i], 9, tenFoldBias[i], 9, tenFoldLowErrLen[i]);
    //     printf("%*.1f, %*u, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/10.0), 4, tenFoldNum[i], 10, tenFoldErr[i], 9, tenFoldBias[i], 9, tenFoldLowErrLen[i]);
    // }
    // printf("\n");
}