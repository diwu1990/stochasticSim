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
#include "synchronizer.hpp"

int main()
{
    
    srand(time(NULL));
    unsigned int sobolNum = 2;
    unsigned int sobolBitLen = 8;
    string mode = "incremental";
    // string mode = "delayed";
    unsigned int totalIter = 1;
    vector<float> inCC(10);
    vector<float> outCC(10);
    for (int index = 0; index < 10; ++index)
    {
    	inCC[index] = 0;
    	outCC[index] = 0;
        unsigned int sobolInitIdx = 1+index;
        unsigned int delay = 1;
        // SOBOLMulti sobolinst;
        // LFSRMulti sobolinst;
        SystemRandMulti sobolinst;
        sobolinst.Init(sobolNum,sobolInitIdx,delay,sobolBitLen,mode,"sobolinst1");
        sobolinst.SeqGen();
        // sobolinst.SeqPrint();

        vector<unsigned int> bitLengthVec(2);
        vector<float> probVec(2);
        vector<float> val(2);
        unsigned int depth;

        for (int i = 0; i < totalIter; ++i)
        {
            depth = 1;
            /* code */
            for (int l = 0; l < 2; ++l)
            {
                bitLengthVec[l] = sobolBitLen;
                probVec[l] = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
            }

            RandNum2BitMulti num2bitMultiInst;
            num2bitMultiInst.Init(probVec,bitLengthVec,sobolinst.OutSeq(),"num2bitMultiInst");
            num2bitMultiInst.SeqGen();

            Synchronizer syncInst;
            syncInst.Init(num2bitMultiInst.OutSeq(),depth,"syncInst");
            syncInst.SeqGen();
            printf("\n");
            printf("inProb0: %.5f, inProb1: %.5f\n", probVec[0], probVec[1]);
            printf("otProb0: %.5f, otProb1: %.5f\n", syncInst.OutProb()[0], syncInst.OutProb()[1]);
            printf("Error 0: %.5f, Error 1: %.5f\n", syncInst.ErrRate()[0], syncInst.ErrRate()[1]);
            printf("InCC:    %.5f, OutCC:   %.5f\n", syncInst.InCC(), syncInst.OutCC());
            // mse[i] += syncInst.ErrRate() * syncInst.ErrRate();
            outCC[index] += syncInst.OutCC() * syncInst.OutCC();
            inCC[index] += syncInst.InCC() * syncInst.InCC();
            // syncInst.CCPrint();
            // syncInst.ProbPrint();
            syncInst.SeqPrint();
        }
        outCC[index] = sqrt(outCC[index]/totalIter);
        inCC[index] = sqrt(inCC[index]/totalIter);
        printf("(%.3f, %.3f)\n", inCC[index], outCC[index]);
    }
    printf("\n");
    
}
