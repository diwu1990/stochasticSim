#include "main.hpp"
#include "autocorrelation.hpp"
#include "crosscorrelation.hpp"
#include "lfsr.hpp"
#include "sobol.hpp"
#include "xshrr1608.hpp"
#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
#include "mul.hpp"
#include "sobolmulti.hpp"
#include "lfsrmulti.hpp"
#include "seqsearch.hpp"
#include <cstdlib>
#include <ctime>
#include "sobolmerge.hpp"
#include "synchronizer.hpp"
#include "div.hpp"
#include "sqrt.hpp"

int main()
{
    
    srand(time(NULL));
    unsigned int sobolNum = 3;
    unsigned int sobolBitLen = 8;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 100000;
    unsigned int seqLength = (unsigned int)pow(2,sobolBitLen);
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
        unsigned int sobolInitIdx = 1+index;
        unsigned int delay = 1;
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
                probVec[l] = val[l];
            }

            vector<unsigned int> inRandNum(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                inRandNum[z] = sobolinst.OutSeq()[0][z%(unsigned int)(pow(2,sobolBitLen))];
            }
            vector<unsigned int> RandSeq;
            RandSeq.resize(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                RandSeq[z] = sobolinst.OutSeq()[1][z%(unsigned int)(pow(2,sobolBitLen))];
            }
            RandNum2Bit num2bitInst;
            num2bitInst.Init(prob0,bitLengthVec[0],inRandNum,"num2bitInst");
            num2bitInst.SeqGen();
            // num2bitInst.Report();
            // num2bitInst.SeqPrint();

            // for (int ooo = 0; ooo < sobolinst.SeqLen(); ++ooo)
            // {
            //     printf("%d: %u, %u", i, sobolinst.OutSeq()[2][ooo], sobolinst.OutSeq()[2][ooo]);
            //     /* code */
            // }
            SQRT sqrtInst;
            sqrtInst.Init(num2bitInst.OutSeq(),RandSeq,sobolBitLen,depth,"sqrtInst");
            // sqrtInst.Report();
            sqrtInst.Calc();
            // sqrtInst.OutPrint();
            // if (prob0 < 0.2)
            // {
                // sqrtInst.OutPrint();
            // }

            tenFoldErr[(unsigned int)floor(sqrtInst.TheoProb()*10)] += sqrtInst.FinalErrRate() * sqrtInst.FinalErrRate();
            // tenFoldErr[(unsigned int)floor(sqrtInst.TheoProb()*10)] += ((sqrtInst.FinalRealProb()-sqrt(prob0))/sqrt(prob0)) * ((sqrtInst.FinalRealProb()-sqrt(prob0))/sqrt(prob0));
            tenFoldNum[(unsigned int)floor(sqrtInst.TheoProb()*10)] += 1;
            // printf("%u\n", sqrtInst.LowErrLen());
            tenFoldLowErrLen[(unsigned int)floor(sqrtInst.TheoProb()*10)] += sqrtInst.LowErrLen();
            // printf("%u\n", tenFoldLowErrLen[(unsigned int)floor(sqrtInst.TheoProb()*10)]);
        }
        for (int y = 0; y < foldNum; ++y)
        {
            // printf("11111\n");
            tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
            tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
        }
        

        printf("Ten Fold with Depth %u, initial sobolIdx %u, delay %u.\n", depth, sobolInitIdx, delay);
        printf("Value range index, Freq, Final Error, LowErrLen:\n");
        for (int i = 0; i < foldNum; ++i)
        {
            printf("%3d, %5u, %-.3f, %-3.3f\n", i, tenFoldNum[i], tenFoldErr[i], tenFoldLowErrLen[i]);
        }
        printf("\n");
    }
    
}
