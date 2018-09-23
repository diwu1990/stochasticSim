#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
#include "sobol.hpp"
#include "sobolmulti.hpp"
#include "lfsr.hpp"
#include "lfsrmulti.hpp"
#include <cstdlib>
#include <ctime>
#include "relu.hpp"

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

        unsigned int bitLength;
        float probVec;
        unsigned int depth;
        // depth = (unsigned int)pow(2,3);
        depth = 3;
        for (int iter = 0; iter < totalIter; ++iter)
        {
            /* code */
            bitLength = sobolBitLen;
            probVec = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));

            vector<unsigned int> inRandNum(seqLength);
            for (int z = 0; z < seqLength; ++z)
            {
                inRandNum[z] = sobolinst.OutSeq()[0][z%(unsigned int)(pow(2,sobolBitLen))];
            }
            vector<unsigned int> RandSeq(seqLength);
        	for (int j = 0; j < seqLength; ++j)
        	{
            	RandSeq[j] = sobolinst.OutSeq()[1][j%(unsigned int)(pow(2,sobolBitLen))] >> (sobolBitLen - depth);
        	}
            RandNum2Bit num2bitInst;
            num2bitInst.Init(probVec,bitLength,inRandNum,"num2bitInst");
            num2bitInst.SeqGen();
            // num2bitInst.Report();
            // num2bitInst.SeqPrint();

            // for (int ooo = 0; ooo < sobolinst.SeqLen(); ++ooo)
            // {
            //     printf("%d: %u, %u", i, sobolinst.OutSeq()[2][ooo], sobolinst.OutSeq()[2][ooo]);
            //     /* code */
            // }
            RELU reluInst;
            // reluInst.Init(num2bitInst.OutSeq(),RandSeq,depth,"reluInst");
            reluInst.Init(num2bitInst.OutSeq(),depth,"reluInst");
            // reluInst.Report();
            reluInst.Calc();
            // printf("%.3f,%.3f,%.3f,%.3f\n",reluInst.InProb(),reluInst.TheoProb(),reluInst.FinalRealProb(),reluInst.FinalErrRate());
            // reluInst.OutPrint();
            // if (prob0 < 0.2)
            // {
                // reluInst.OutPrint();
            // }

            tenFoldErr[(unsigned int)floor(reluInst.TheoProb()*10)] += reluInst.FinalErrRate() * reluInst.FinalErrRate();
            // tenFoldErr[(unsigned int)floor(reluInst.TheoProb()*10)] += ((reluInst.FinalRealProb()-sqrt(prob0))/sqrt(prob0)) * ((reluInst.FinalRealProb()-sqrt(prob0))/sqrt(prob0));
            tenFoldNum[(unsigned int)floor(reluInst.TheoProb()*10)] += 1;
            // printf("%u\n", reluInst.LowErrLen());
            tenFoldLowErrLen[(unsigned int)floor(reluInst.TheoProb()*10)] += reluInst.LowErrLen();
            // printf("%u\n", tenFoldLowErrLen[(unsigned int)floor(reluInst.TheoProb()*10)]);
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
