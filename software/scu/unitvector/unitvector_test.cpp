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
#include "gdiv.hpp"
#include "iscbdiv.hpp"
#include "gsqrt.hpp"
#include "jkdivbisqrt.hpp"
#include "iscbdivbisqrt.hpp"
#include "unitvector.hpp"

int main()
{
    srand(time(NULL));
    unsigned int bitLength = 12;
    unsigned int SeqDim = 16;
    unsigned int RandNumDim = 5;
    unsigned int totalSeqNum = SeqDim+RandNumDim;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 1;
    unsigned int seqLength = (unsigned int)pow(2,bitLength);
    unsigned int foldNum = 1;
    vector<float> tenFoldErr(foldNum);
    vector<unsigned int> tenFoldNum(foldNum);
    vector<float> tenFoldLowErrLen(foldNum);
    for (int index = 0; index < 1; ++index)
    {
        for (int i = 0; i < foldNum; ++i)
        {
            tenFoldErr[i] = 0;
            tenFoldNum[i] = 0;
            tenFoldLowErrLen[i] = 0;
        }
        unsigned int rngInitIdx = 1+index;
        unsigned int delay = 1;
        SystemRandMulti rngInst;
        // SOBOLMulti rngInst;
        // LFSRMulti rngInst;
        rngInst.Init(totalSeqNum,rngInitIdx,delay,bitLength,mode,"rngInst");
        rngInst.SeqGen();
        // rngInst.SeqPrint();

        vector<unsigned int> bitLengthVec(SeqDim);
        vector<float> probVec(SeqDim);
        unsigned int depth;
        // depth = (unsigned int)pow(2,3);
        depth = 2;

        // [1.25430465][-1.06685638][0.000981966][0.000339819759][0.000149245345][-0.000163379416][-0.00037153][-0.00114523713][0]
        // [3.98658204][-3.1342392][0.00213337643][0.000707507716][0.000307342445][-0.000319205][-0.000741768163][-0.00247943401][1]
        // [7.52857876][-5.44600296][0.00426732376][0.00141658867][0.000625585904][-0.00051455549][-0.00121756934][-0.00415382581][2]

        for (int iter = 0; iter < totalIter; ++iter)
        {
            /* code */
            for (int l = 0; l < SeqDim; ++l)
            {
                bitLengthVec[l] = bitLength;
                // probVec[l] = (float)((float)(rand()%(int)pow(2,bitLength))/(float)pow(2,bitLength));
                probVec[l] = 0;
            }

            // probVec[0] = 7.52857876/4+0.5;
            // probVec[1] = -5.44600296/4+0.5;
            // probVec[2] = 0.00426732376/4+0.5;
            // probVec[3] = 0.00141658867/4+0.5;
            // probVec[4] = 0.000625585904/4+0.5;
            // probVec[5] = -0.00051455549/4+0.5;
            // probVec[6] = -0.00121756934/4+0.5;
            // probVec[7] = -0.00415382581/4+0.5;
            // probVec[8] = 0/4+0.5;
            // probVec[9] = 0/4+0.5;

            probVec[0] = 1.25430465/4+0.5;
            probVec[1] = -1.06685638/4+0.5;
            probVec[2] = 0.000981966/4+0.5;
            probVec[3] = 0.000339819759/4+0.5;
            probVec[4] = 0.000149245345/4+0.5;
            probVec[5] = -0.000163379416/4+0.5;
            probVec[6] = -0.00037153/4+0.5;
            probVec[7] = -0.00114523713/4+0.5;
            probVec[8] = 0/4+0.5;
            probVec[9] = 0/4+0.5;

            vector<vector<unsigned int>> inRandNum(SeqDim);
            for (int i = 0; i < SeqDim; ++i)
            {
                inRandNum[i].resize(seqLength);
                for (int z = 0; z < seqLength; ++z)
                {
                    inRandNum[i][z] = rngInst.OutSeq()[i][z%(unsigned int)(pow(2,bitLength))];
                }
            }

            // if RandNumDim > 0
            vector<vector<unsigned int>> RandSeq(RandNumDim);
            for (int i = 0; i < RandNumDim; ++i)
            {
                RandSeq[i].resize(seqLength);
                for (int z = 0; z < seqLength; ++z)
                {
                    RandSeq[i][z] = rngInst.OutSeq()[i+SeqDim][z%(unsigned int)(pow(2,bitLength))];
                }
            }
            
            
            RandNum2BitMulti num2bitInst;
            // RandNum2Bit num2bitInst;
            num2bitInst.Init(probVec,bitLengthVec,inRandNum,"num2bitInst");
            num2bitInst.SeqGen();
            // num2bitInst.Report();
            // num2bitInst.SeqPrint();

            // printf("aaaaa\n");
            SOFTMAX instance;
            instance.Init(num2bitInst.OutSeq(),RandSeq,bitLength,"instance");
            // instance.Report();
            instance.Calc();

            // tenFoldErr[(unsigned int)floor(theoreticalProb*10)] += (instance.FinalRealProb()-theoreticalProb)/theoreticalProb * (instance.FinalRealProb()-theoreticalProb)/theoreticalProb;
            // tenFoldNum[(unsigned int)floor(theoreticalProb*10)] += 1;
            // tenFoldLowErrLen[(unsigned int)floor(theoreticalProb*10)] += instance.LowErrLen();
        }
        // for (int y = 0; y < foldNum; ++y)
        // {
        //     // printf("11111\n");
        //     tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
        //     tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
        // }
        

        // printf("Ten Fold with Depth %u, initial sobolIdx %u, delay %u.\n", depth, rngInitIdx, delay);
        // printf("Value range index, Freq, Final Error, LowErrLen:\n");
        // for (int i = 0; i < foldNum; ++i)
        // {
        //     printf("%3d, %5u, %-.3f, %-3.3f\n", i, tenFoldNum[i], tenFoldErr[i], tenFoldLowErrLen[i]);
        // }
        // printf("\n");
    }
    
}
