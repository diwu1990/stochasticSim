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
    unsigned int sobolBitLen = 6;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 1;
    unsigned int totalDepth = 8;
    vector<float> inCC(totalDepth);
    vector<float> outCC(totalDepth);
    vector<float> mse0(totalDepth);
    vector<float> mse1(totalDepth);
    for (int index = 0; index < totalDepth; ++index)
    {
    	inCC[index] = 0;
    	outCC[index] = 0;
        mse0[index] = 0;
    	mse1[index] = 0;
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
            depth = index+1;
            /* code */
            float prob0 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
            float prob1 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
            vector<float> val(2);
            val[0] = min(prob0,prob1);
            val[1] = max(prob0,prob1);

            // val[0] = 0.7;
            // val[1] = 0.9;

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
            mse0[index] += syncInst.ErrRate()[0] * syncInst.ErrRate()[0];
            mse1[index] += syncInst.ErrRate()[1] * syncInst.ErrRate()[1];
            outCC[index] += syncInst.OutCC();
            inCC[index] += syncInst.InCC();
            // syncInst.CCPrint();
            // syncInst.ProbPrint();
            // syncInst.SeqPrint();
        }
        mse0[index] = sqrt(mse0[index]/totalIter);
        mse1[index] = sqrt(mse1[index]/totalIter);
        outCC[index] = outCC[index]/totalIter;
        inCC[index] = inCC[index]/totalIter;
        printf("(%.5f, %.5f) %.5f %.5f\n", inCC[index], outCC[index], mse0[index], mse1[index]);
    }
    printf("\n");
    
}
