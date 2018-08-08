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
#include "div.hpp"
#include "synchronizer.hpp"
#include "desynchronizer.hpp"

int main()
{
    srand(time(NULL));
    unsigned int sobolNum = 3;
    unsigned int sobolBitLen = 8;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 10000;
    unsigned int seqLength = (unsigned int)pow(2,sobolBitLen);
    unsigned int foldNum = 11;
    vector<float> tenFoldErr(foldNum);
    vector<float> tenFoldBias(foldNum);
    vector<unsigned int> tenFoldNum(foldNum);
    vector<float> tenFoldLowErrLen(foldNum);
    vector<float> tenFoldCorr(foldNum);
    for (int index = 0; index < 10; ++index)
    {
        for (int i = 0; i < foldNum; ++i)
        {
            tenFoldErr[i] = 0;
            tenFoldCorr[i] = 0;
            tenFoldBias[i] = 0;
            tenFoldNum[i] = 0;
            tenFoldLowErrLen[i] = 0;
        }
        unsigned int sobolInitIdx = 1+index;
        unsigned int delay = 0;
        // SystemRandMulti sobolinst;
        SOBOLMulti sobolinst;
        // LFSRMulti sobolinst;
        sobolinst.Init(sobolNum,sobolInitIdx,delay,sobolBitLen,mode,"sobolinst1");
        sobolinst.SeqGen();
        // sobolinst.SeqPrint();

        vector<unsigned int> bitLengthVec(2);
        vector<float> probVec(2);
        vector<float> val(2);
        unsigned int depth;
        unsigned int depthSync;
        // depth = (unsigned int)pow(2,3);
        depth = 2;
        depthSync = 3;
        for (int iter = 0; iter < totalIter; ++iter)
        {
            /* code */
            float prob0 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
            float prob1 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
            val[0] = min(prob0,prob1);
            val[1] = max(prob0,prob1);
            for (int l = 0; l < 2; ++l)
            {
                bitLengthVec[l] = sobolBitLen;
                probVec[l] = val[l];
            }

            vector<vector<unsigned int>> inRandNum(2);
            inRandNum[0].resize(seqLength);
            inRandNum[1].resize(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                inRandNum[0][z] = sobolinst.OutSeq()[0][z%(unsigned int)(pow(2,sobolBitLen))];
                inRandNum[1][z] = sobolinst.OutSeq()[1][z%(unsigned int)(pow(2,sobolBitLen))];
            }
            // for (int z = 0; z < seqLength; ++z)
            // {
            //     inRandNum[0][z] = (unsigned int)(rand()%(unsigned int)(pow(2,sobolBitLen)));
            //     inRandNum[1][z] = (unsigned int)(rand()%(unsigned int)(pow(2,sobolBitLen)));
            //     // printf("(%u,%u,%u,%u,)", sobolBitLen, (unsigned int)(pow(2,sobolBitLen)), inRandNum[0][z], inRandNum[1][z]);
            // }
            // // printf("\n");
            vector<unsigned int> RandSeq;
            RandSeq.resize(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                RandSeq[z] = sobolinst.OutSeq()[2][z%(unsigned int)(pow(2,sobolBitLen))];
            }
            RandNum2BitMulti num2bitMultiInst;
            num2bitMultiInst.Init(probVec,bitLengthVec,inRandNum,"num2bitMultiInst");
            num2bitMultiInst.SeqGen();
            
            // DeSynchronizer syncInst;
            Synchronizer syncInst;
            syncInst.Init(num2bitMultiInst.OutSeq(),1,"syncInst");
            syncInst.SeqGen();

            DIV divInst;
            divInst.Init(syncInst.OutSeq(),RandSeq,sobolBitLen,depth,depthSync,"divInst");
            // divInst.Report();
            divInst.Calc();

            tenFoldErr[(unsigned int)floor(divInst.TheoProb()*10)] += divInst.FinalErrRate() * divInst.FinalErrRate();
            tenFoldBias[(unsigned int)floor(divInst.TheoProb()*10)] += divInst.FinalErrRate();
            tenFoldNum[(unsigned int)floor(divInst.TheoProb()*10)] += 1;
            tenFoldLowErrLen[(unsigned int)floor(divInst.TheoProb()*10)] += divInst.LowErrLen();
            tenFoldCorr[(unsigned int)floor(divInst.TheoProb()*10)] += divInst.InCC();
        }
        for (int y = 0; y < foldNum; ++y)
        {
            // printf("11111\n");
            tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
            tenFoldBias[y] = tenFoldBias[y]/tenFoldNum[y];
            tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
            tenFoldCorr[y] = tenFoldCorr[y]/tenFoldNum[y];
        }
        

        printf("Ten Fold with Depth %u, initial sobolIdx %u, delay %u.\n", depth, sobolInitIdx, delay);
        printf("Range, Freq, Correlation, Error Rate, Stat Bias, LowErrLen:\n");
        for (int i = 0; i < foldNum; ++i)
        {
            printf("%*.1f, %*u, %*.4f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/10.0), 4, tenFoldNum[i], 11, tenFoldCorr[i], 10, tenFoldErr[i], 9, tenFoldBias[i], 9, tenFoldLowErrLen[i]);
        }
        printf("\n");
    }
    
}
