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
#include "desynchronizer.hpp"
#include "perfsim.hpp"

int main()
{

    srand(time(NULL));
    unsigned int sobolNum = 1;
    unsigned int sobolBitLen = 8;
    // string mode = "incremental";
    // string mode = "delayed";
    string mode = "random";
    unsigned int totalIter = 10000;
    vector<float> inCC(1);
    vector<float> outCC(1);
    vector<float> mse(1);
    clock_t begin = clock();
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
        sobolinst.SeqPrint();

        vector<unsigned int> bitLengthVec(sobolNum);
        vector<float> probVec(sobolNum);
        vector<float> val(sobolNum);
        unsigned int depth;
        float thdBias = 0.05;

        for (int i = 0; i < totalIter; ++i)
        {
            depth = 1;
            /* code */
            for (int l = 0; l < sobolNum; ++l)
            {
                bitLengthVec[l] = sobolBitLen;
                probVec[l] = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
                // printf("%f,", probVec[l]);
            }
            // printf("\n");

            RandNum2BitMulti num2bitMultiInst;
            num2bitMultiInst.Init(probVec,bitLengthVec,sobolinst.OutSeq(),"num2bitMultiInst");
            num2bitMultiInst.SeqGen();

            // vector<char> iBit(2);
            // DeSynchronizer syncInst;
            // syncInst.Init(probVec,depth,128,thdBias,"syncInst");
            // for (int j = 0; j < (int)pow(2,sobolBitLen); ++j)
            // {
            //     iBit[0] = num2bitMultiInst.OutSeq()[0][j];
            //     iBit[1] = num2bitMultiInst.OutSeq()[1][j];
            //     syncInst.Calc(iBit);
            //     // printf("%d: (%u,%u)=>(%u, %u)\n", j, iBit[0], iBit[1], syncInst.OutBit()[0], syncInst.OutBit()[1]);
            // }
                // printf("(%f,%f)\n",syncInst.WProb()[0],syncInst.WProb()[1]);
                // printf("(%f,%f)\n",syncInst.TheoProb()[0],syncInst.TheoProb()[1]);
                // printf("(%f,%f)\n",syncInst.WBias()[0],syncInst.WBias()[1]);
                // printf("(%d,%d)\n",syncInst.Speed()[0],syncInst.Speed()[1]);
            // printf("\n");
            // #ifdef PERFSIM
            //     printf("defined\n");
            // #endif
            // printf("inProb0: %.5f, inProb1: %.5f\n", probVec[0], probVec[1]);
            // printf("inProb0: %.5f, inProb1: %.5f\n", syncInst.InProb()[0], syncInst.InProb()[1]);
            // printf("otProb0: %.5f, otProb1: %.5f\n", syncInst.OutProb()[0], syncInst.OutProb()[1]);
            // printf("Error 0: %.5f, Error 1: %.5f\n", syncInst.ErrRate()[0], syncInst.ErrRate()[1]);
            // printf("InCC:    %.5f, OutCC:   %.5f\n", syncInst.InCC(), syncInst.OutCC());
            // mse[index] += (syncInst.ErrRate()[0] * syncInst.ErrRate()[0] +  syncInst.ErrRate()[1] * syncInst.ErrRate()[1]) / 2;
            // outCC[index] += syncInst.OutCC();
            // inCC[index] += syncInst.InCC();
            // syncInst.CCPrint();
            // syncInst.ProbPrint();
            // syncInst.SeqPrint();
        }
        // mse[index] = sqrt(mse[index]/totalIter);
        // outCC[index] = outCC[index]/totalIter;
        // inCC[index] = inCC[index]/totalIter;
        // printf("(%.3f, %.3f) %.3f\n", inCC[index], outCC[index], mse[index]);
    }
    printf("\n");
    clock_t end = clock();
    double elasped_secs = double(end - begin) / CLOCKS_PER_SEC;

    printf("%f\n", elasped_secs);
    
}