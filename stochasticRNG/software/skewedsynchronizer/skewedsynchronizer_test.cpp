#include "autocorrelation.hpp"
#include "crosscorrelation.hpp"
#include "lfsr.hpp"
#include "lfsrmulti.hpp"
#include "sobol.hpp"
#include "sobolmulti.hpp"
#include "systemrand.hpp"
#include "systemrandmulti.hpp"
#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
#include <cstdlib>
#include <ctime>
#include "skewedsynchronizer.hpp"

int main()
{
    srand(time(NULL));
    unsigned int sobolNum = 2;
    unsigned int sobolBitLen = 8;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 10000;
    vector<float> inCC(1);
    vector<float> outCC(1);
    vector<float> mse(1);
    for (int index = 0; index < 1; ++index)
    {
    	inCC[index] = 0;
    	outCC[index] = 0;
    	mse[index] = 0;
        unsigned int sobolInitIdx = 1+index;
        unsigned int delay = 1;
        SOBOLMulti sobolinst;
        // LFSRMulti sobolinst;
        // SystemRandMulti sobolinst;
        sobolinst.Init(sobolNum,sobolInitIdx,delay,sobolBitLen,mode,"sobolinst1");
        sobolinst.SeqGen();
        // sobolinst.SeqPrint();

        vector<unsigned int> bitLengthVec(2);
        vector<float> probVec(2);
        vector<float> val(2);
        unsigned int depth;

        for (int i = 0; i < totalIter; ++i)
        {
            depth = 4;
            /* code */
            float prob0 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
            float prob1 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
            vector<float> val(2);
            val[0] = min(prob0,prob1);
            val[1] = max(prob0,prob1);

            for (int l = 0; l < 2; ++l)
            {
                bitLengthVec[l] = sobolBitLen;
                probVec[l] = val[l];
            }

            RandNum2BitMulti num2bitMultiInst;
            num2bitMultiInst.Init(probVec,bitLengthVec,sobolinst.OutSeq(),"num2bitMultiInst");
            num2bitMultiInst.SeqGen();

            SkewedSynchronizer syncInst;
            syncInst.Init(num2bitMultiInst.OutSeq(),depth,"syncInst");
            syncInst.SeqGen();
            // printf("\n");
            // printf("inProb0: %.5f, inProb1: %.5f\n", probVec[0], probVec[1]);
            // printf("inProb0: %.5f, inProb1: %.5f\n", syncInst.InProb()[0], syncInst.InProb()[1]);
            // printf("otProb0: %.5f, otProb1: %.5f\n", syncInst.OutProb()[0], syncInst.OutProb()[1]);
            // printf("Error 0: %.5f, Error 1: %.5f\n", syncInst.ErrRate()[0], syncInst.ErrRate()[1]);
            // printf("InCC:    %.5f, OutCC:   %.5f\n", syncInst.InCC(), syncInst.OutCC());
            mse[index] += (syncInst.ErrRate()[0] * syncInst.ErrRate()[0] +  syncInst.ErrRate()[1] * syncInst.ErrRate()[1]) / 2;
            outCC[index] += syncInst.OutCC();
            inCC[index] += syncInst.InCC();
            // syncInst.CCPrint();
            // syncInst.ProbPrint();
            // syncInst.SeqPrint();
        }
        mse[index] = sqrt(mse[index]/totalIter);
        outCC[index] = outCC[index]/totalIter;
        inCC[index] = inCC[index]/totalIter;
        printf("(%.3f, %.3f) %.3f\n", inCC[index], outCC[index], mse[index]);
    }
    printf("\n");
    
}
