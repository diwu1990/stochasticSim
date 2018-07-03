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

int main()
{
    /*
    srand(time(NULL));
    unsigned int sobolNum = 15;
    string sobolmode = "delayed";
    // string sobolmode = "incremental";

    printf("LFSR delayed:\n");
    for (int i = 0; i < sobolNum; ++i)
    {
        printf("%-2d, ", i+1);
        for (int j = 0; j < sobolNum; ++j)
        {
            unsigned int initialIndex = i;
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

    for (int i = 0; i < 1; ++i)
    {
        printf("%-2d, ", i+1);
        for (int j = 0; j < 1; ++j)
        {
            unsigned int initialIndex = i;
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
    for (int i = 0; i < 1; ++i)
    {
        printf("%-2d, ", i+1);
        for (int j = 0; j < 1; ++j)
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
    */
    unsigned int dim1 = 8;
    unsigned int dim2 = 1;
    SOBOL sobolinst1;
    sobolinst1.Init(8,dim1,0,"sobolinst1");
    sobolinst1.SeqGen();
    // sobolinst1.SeqPrint();
    // sobolinst1.VecPrint();
    // sobolinst1.MemPrint();

    SOBOL sobolinst2;
    sobolinst2.Init(8,dim2,0,"sobolinst2");
    sobolinst2.SeqGen();
    // sobolinst2.SeqPrint();
    // sobolinst2.VecPrint();
    // sobolinst2.MemPrint();

    vector<vector<unsigned int>> dirVec(2);
    dirVec[0] = sobolinst1.DirVec();
    dirVec[0] = sobolinst2.DirVec();
    dirVec[1] = {0,0,0,0,0,0,0,0};
    SOBOLMerge sobolmergeinst;
    sobolmergeinst.Init(dirVec,8,"sobolmergeinst");
    sobolmergeinst.Report();
    sobolmergeinst.MemGen();
    sobolmergeinst.VecPrint();
    sobolmergeinst.MemPrint();

}
