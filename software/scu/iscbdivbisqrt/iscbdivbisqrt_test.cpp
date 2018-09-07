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
#include "iscbdivbisqrt.hpp"

int main()
{
    
    srand(time(NULL));
    unsigned int sobolNum = 2;
    unsigned int sobolBitLen = 8;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 10000;
    unsigned int seqLength = (unsigned int)pow(2,sobolBitLen);
    unsigned int foldNum = 11;
    vector<float> tenFoldErr(foldNum);
    vector<float> tenFoldBias(foldNum);
    vector<float> tenFoldCorr(foldNum);
    vector<unsigned int> tenFoldNum(foldNum);
    vector<float> tenFoldLowErrLen(foldNum);
    unsigned int initialDim = 0;
    for (int index = initialDim; index < initialDim+foldNum-1; ++index)
    {
        for (int i = 0; i < foldNum; ++i)
        {
            tenFoldErr[i] = 0;
            tenFoldBias[i] = 0;
            tenFoldCorr[i] = 0;
            tenFoldNum[i] = 0;
            tenFoldLowErrLen[i] = 0;
        }
        unsigned int sobolInitIdx = 1+index;
        unsigned int delay = 1;
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
        // depth = (unsigned int)pow(2,3);
        depth = 2;
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
                probVec[l] = val[0];
            }

            vector<unsigned int> inRandNum(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                inRandNum[z] = sobolinst.OutSeq()[0][z%(unsigned int)(pow(2,sobolBitLen))];
                // printf("%d,", inRandNum[z]);
            }
            // printf("\n");
            vector<unsigned int> RandSeq;
            RandSeq.resize(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                RandSeq[z] = sobolinst.OutSeq()[1][z%(unsigned int)(pow(2,sobolBitLen))];
            }
            RandNum2Bit num2bitInst;
            num2bitInst.Init(probVec[0],bitLengthVec[0],inRandNum,"num2bitInst");
            num2bitInst.SeqGen();
            // num2bitInst.Report();
            // num2bitInst.SeqPrint();

            // for (int ooo = 0; ooo < sobolinst.SeqLen(); ++ooo)
            // {
            //     printf("%d: %u, %u", i, sobolinst.OutSeq()[2][ooo], sobolinst.OutSeq()[2][ooo]);
            //     /* code */
            // }
            ISCBDIVBISQRT sqrtInst;
            sqrtInst.Init(num2bitInst.OutSeq(),RandSeq,sobolBitLen,"sqrtInst");
            // sqrtInst.Report();
            sqrtInst.Calc();
            // sqrtInst.OutPrint();
            // if (prob0 < 0.2)
            // {
                // sqrtInst.OutPrint();
            // }

            tenFoldErr[(unsigned int)floor(sqrtInst.TheoProb()*10)] += sqrtInst.FinalErrRate() * sqrtInst.FinalErrRate();
            tenFoldBias[(unsigned int)floor(sqrtInst.TheoProb()*10)] += sqrtInst.FinalErrRate();
            tenFoldCorr[(unsigned int)floor(sqrtInst.TheoProb()*10)] += sqrtInst.InAC();
            tenFoldNum[(unsigned int)floor(sqrtInst.TheoProb()*10)] += 1;
            tenFoldLowErrLen[(unsigned int)floor(sqrtInst.TheoProb()*10)] += sqrtInst.LowErrLen();
        }
        for (int y = 0; y < foldNum; ++y)
        {
            tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
            tenFoldCorr[y] = tenFoldCorr[y]/tenFoldNum[y];
            tenFoldBias[y] = tenFoldErr[y]/tenFoldNum[y];
            tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
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
