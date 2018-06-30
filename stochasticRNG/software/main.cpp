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

int main()
{
    srand(time(NULL));
    /*test mixed sobol*/
    unsigned int sobolNum = 15;
    string sobolmode = "delayed";
    // string sobolmode = "incremental";

    printf("LFSR delayed:\n");
    for (int i = 0; i < sobolNum; ++i)
    {
        printf("%-2d, ", i+1);
        for (int j = 0; j < sobolNum; ++j)
        {
            unsigned int initialIndex = i+1;
            unsigned int soboldelay = j;

            // SOBOLMulti sobolmulti8to8;
            LFSRMulti sobolmulti8to8;
            // string sobolmode = "incremental";
            sobolmulti8to8.Init(sobolNum,initialIndex,soboldelay,8,sobolmode,"sobolmulti8to8");
            sobolmulti8to8.SeqGen();
            // sobolmulti8to8.SeqPrint();
            // sobolmulti8to8.VecPrint();
            // sobolmulti8to8.MemPrint();

            SeqSearch seqsearchInst;
            seqsearchInst.Init(sobolmulti8to8.OutSeq(),8,0.15,"seqsearchInst");
            // seqsearchInst.Report();
            seqsearchInst.CombGen();
            // seqsearchInst.CombPrint();
            // seqsearchInst.MSCCPrint();
            printf("%-2.3f, ", seqsearchInst.MSCC());
        }
        printf("\n");
    }

    printf("SOBOL delayed:\n");
    for (int i = 0; i < sobolNum; ++i)
    {
        printf("%-2d, ", i+1);
        for (int j = 0; j < sobolNum; ++j)
        {
            unsigned int initialIndex = i+1;
            unsigned int soboldelay = j;

            SOBOLMulti sobolmulti8to8;
            // LFSRMulti sobolmulti8to8;
            // string sobolmode = "incremental";
            sobolmulti8to8.Init(sobolNum,initialIndex,soboldelay,8,sobolmode,"sobolmulti8to8");
            sobolmulti8to8.SeqGen();
            // sobolmulti8to8.SeqPrint();
            // sobolmulti8to8.VecPrint();
            // sobolmulti8to8.MemPrint();

            SeqSearch seqsearchInst;
            seqsearchInst.Init(sobolmulti8to8.OutSeq(),8,0.15,"seqsearchInst");
            // seqsearchInst.Report();
            seqsearchInst.CombGen();
            // seqsearchInst.CombPrint();
            // seqsearchInst.MSCCPrint();
            printf("%-2.3f, ", seqsearchInst.MSCC());
        }
        printf("\n");
    }


    sobolmode = "incremental";
    printf("LFSR incremental:\n");

    for (int i = 0; i < sobolNum; ++i)
    {
        printf("%-2d, ", i+1);
        for (int j = 0; j < sobolNum; ++j)
        {
            unsigned int initialIndex = i+1;
            unsigned int soboldelay = j;

            // SOBOLMulti sobolmulti8to8;
            LFSRMulti sobolmulti8to8;
            // string sobolmode = "incremental";
            sobolmulti8to8.Init(sobolNum,initialIndex,soboldelay,8,sobolmode,"sobolmulti8to8");
            sobolmulti8to8.SeqGen();
            // sobolmulti8to8.SeqPrint();
            // sobolmulti8to8.VecPrint();
            // sobolmulti8to8.MemPrint();

            SeqSearch seqsearchInst;
            seqsearchInst.Init(sobolmulti8to8.OutSeq(),8,0.15,"seqsearchInst");
            // seqsearchInst.Report();
            seqsearchInst.CombGen();
            // seqsearchInst.CombPrint();
            // seqsearchInst.MSCCPrint();
            printf("%-2.3f, ", seqsearchInst.MSCC());
        }
        printf("\n");
    }

    printf("SOBOL incremental:\n");
    for (int i = 0; i < sobolNum; ++i)
    {
        printf("%-2d, ", i+1);
        for (int j = 0; j < sobolNum; ++j)
        {
            unsigned int initialIndex = i+1;
            unsigned int soboldelay = j;

            SOBOLMulti sobolmulti8to8;
            // LFSRMulti sobolmulti8to8;
            // string sobolmode = "incremental";
            sobolmulti8to8.Init(sobolNum,initialIndex,soboldelay,8,sobolmode,"sobolmulti8to8");
            sobolmulti8to8.SeqGen();
            // sobolmulti8to8.SeqPrint();
            // sobolmulti8to8.VecPrint();
            // sobolmulti8to8.MemPrint();

            SeqSearch seqsearchInst;
            seqsearchInst.Init(sobolmulti8to8.OutSeq(),8,0.15,"seqsearchInst");
            // seqsearchInst.Report();
            seqsearchInst.CombGen();
            // seqsearchInst.CombPrint();
            // seqsearchInst.MSCCPrint();
            printf("%-2.3f, ", seqsearchInst.MSCC());
        }
        printf("\n");
    }
    

    // unsigned int seqNum = sobolNum*(sobolNum-1)/(sobolNum-1);

    // vector<unsigned int> tenFoldCnt(10);
    // vector<float> tenFoldErr(10);
    // vector<float> tenFoldCC(10);
    // vector<float> tenFoldLowErrLen(10);
    // vector<unsigned int> bitLengthVecmix(seqNum);
    // vector<float> probVecmix(seqNum);
    // for (int iter = 0; iter < 10000; ++iter)
    // {
    //     for (int i = 0; i < seqNum; ++i)
    //     {
    //         bitLengthVecmix[i] = 8;
    //         // probVecmix[i] = (float)i/(float)seqNum;
    //         probVecmix[i] = (float)((float)(rand()%256)/256);
    //         // probVecmix[i] = 0.3;
    //     }
        
    //     RandNum2BitMulti num2bitSobolmix;
    //     num2bitSobolmix.Init(probVecmix,bitLengthVecmix,sobolmulti8to8.OutSeq(),"num2bitSobolmix");
    //     num2bitSobolmix.SeqGen();

    //     // float threshold = 0.15;
    //     float errorthreshold = 0.05;
    //     // CrossCorrelation CCsobolmix;
    //     // CCsobolmix.Init(num2bitSobolmix.OutSeq(),threshold,"CCsobolmix");
    //     // CCsobolmix.CalcCC();

    //     unsigned int totalOK = 0;
    //     vector<float> mulResult(sobolNum*(sobolNum-1)/2);
    //     for (int i = 0; i < sobolNum*(sobolNum-1)/2; ++i)
    //     {
    //         mulResult[i] = 0;
    //     }
    //     for (int i = 0; i < sobolNum; ++i)
    //     {
    //         for (int j = 0; j < sobolNum; ++j)
    //         {
    //             if (i < j)
    //             {
    //                 vector<vector<unsigned int>> mulInSeq(2);
    //                 mulInSeq[0] = num2bitSobolmix.OutSeq()[i];
    //                 mulInSeq[1] = num2bitSobolmix.OutSeq()[j];
    //                 MUL multiplier;
    //                 multiplier.Init(mulInSeq,"multiplier");
    //                 // multiplier.Report();
    //                 multiplier.CalcProd();
    //                 // if (multiplier.InCC() < threshold && multiplier.InCC() > -threshold)
    //                 if (multiplier.FinalErrRate() < errorthreshold && multiplier.FinalErrRate() > -errorthreshold)
    //                 {
    //                     totalOK++;
    //                     tenFoldCnt[(int)(multiplier.FinalRealProb()*10)]++;
    //                     tenFoldErr[(int)(multiplier.FinalRealProb()*10)] += multiplier.FinalErrRate()*multiplier.FinalErrRate();
    //                     tenFoldCC[(int)(multiplier.FinalRealProb()*10)] += multiplier.InCC()*multiplier.InCC();
    //                     tenFoldLowErrLen[(int)(multiplier.FinalRealProb()*10)] += multiplier.LowErrLen();
    //                     // printf("(%d,%d) Input Probability: %f, %f => Input CC: %.3f; Final Error Rate: %.3f; Low Error Length: %u\n", i,j,,,multiplier.InCC(),multiplier.FinalErrRate(), multiplier.LowErrLen());
    //                     // printf("(%d,%d)%u\n",i,j,totalOK);
    //                     // multiplier.OutPrint();
    //                     // if ((int)multiplier.FinalRealProb() >= 0 and multiplier.FinalRealProb() < 0.1)
    //                     // {
    //                     //     /* code */
    //                     // }
    //                     // else if (multiplier.FinalRealProb() >= 0.1 and multiplier.FinalRealProb() < 0.2)
    //                     // {
    //                     //     /* code */
    //                     // }
    //                 }
    //             }
    //         }
    //     }
    // }
    // std::cout << "Sobol Mode:          " << sobolmode << std::endl;
    // printf("Sobol Index:         %u\n", initialIndex);
    // printf("Sobol delay:         %u\n", soboldelay);
    // for (int i = 0; i < 10; ++i)
    // {
    //     printf("%d: %-7d, %-5.5f, %-5.5f, %-5.5f\n", i, tenFoldCnt[i], tenFoldErr[i]/tenFoldCnt[i], tenFoldCC[i]/tenFoldCnt[i], (float)tenFoldLowErrLen[i]/(float)tenFoldCnt[i]);
    // }
}
