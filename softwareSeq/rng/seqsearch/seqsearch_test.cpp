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
            seqsearchInst.Calc();
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
            seqsearchInst.Calc();
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
            seqsearchInst.Calc();
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
            seqsearchInst.Calc();
            // seqsearchInst.CombPrint();
            // seqsearchInst.MSCCPrint();
            printf("%-2.3f, ", seqsearchInst.MSCC());
        }
        printf("\n");
    }
    */