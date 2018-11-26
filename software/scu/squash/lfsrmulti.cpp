#include "lfsrmulti.hpp"
#include "lfsr.hpp"

void LFSRMulti::Init(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4, string param5, string param6)
{
    dimNum = param1;
    polyIndex = param2;
    delayIndex = param3;
    lfsrLen = param4;
    outputLen = (unsigned int)(pow(2,lfsrLen))-1;
    mode = param5;
    if (mode != "delayed" && mode != "incremental" && mode != "random")
    {
        printf("Error: Input mode is invalid. (delayed or incremental or random)\n");
    }
    m_name = param6;
    if (lfsrLen < 4 || lfsrLen > 16)
    {
        printf("Error: Input lfsrLen exceeds the legal range (4~16) in LFSRMulti class.\n");
    }
    outSeq.resize(dimNum);
    polyVal.resize(dimNum);
    for (int i = 0; i < dimNum; ++i)
    {
        outSeq[i].resize(outputLen);
    }
    arrayLen.resize(13);
    realIndex = 0;
}

LFSRMulti::LFSRMulti(){}

LFSRMulti::~LFSRMulti(){}

void LFSRMulti::Report()
{
    printf("Current LFSRMulti:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("LFSRMulti Length:       %-5u\n", lfsrLen);
    printf("Dimension Number:       %-5u\n", dimNum);
    printf("Delay Index:            %-5u\n", delayIndex);
    printf("Output Sequence Length: %-5u\n", outputLen);
}

void LFSRMulti::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling LFSRMulti Help. Following are instructions to LFSRMulti Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the LFSRMulti inst.\n");
    printf("Initial Parameters: Number of Dimension, Index of Initial Polynomial, Delay Index, LFSR Bit Length, LFSR Mode, Instance Name.\n");
    printf("1) Number of Dimension: Number of Output Sequences.\n");
    printf("2) Index of Initial Polynomial: The starting index of polynomial.\n");
    printf("3) Index of LFSR delay: The interval index of output delay.\n");
    printf("4) LFSR Bit Length: Bit Length of LFSR.\n");
    printf("5) LFSR Mode: Define the relationship between each lfsr sequence, delayed means the each sequence is delayed one cycle from another, incremental means lfsr sequence of different dimension with same delay.\n");
    printf("6) Instance Name: Name of current LFSRMulti Instance.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing random number sequence from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current LFSRMulti instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current LFSRMulti instance.\n");

    printf("6. inst.PlyPrint() method:\n");
    printf("Print the initial directional vector sequence of current LFSRMulti instance.\n");

    printf("7. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current LFSRMulti instance.\n");

    printf("8. inst.OutPly() method:\n");
    printf("Return the polynomials of current LFSRMulti instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

unsigned int LFSRMulti::SeqLen()
{
    return outputLen;
}

vector<vector<unsigned int>> LFSRMulti::OutSeq()
{
    return outSeq;
}

vector<unsigned int> LFSRMulti::OutPly()
{
    return polyVal;
}

void LFSRMulti::SeqPrint()
{
    printf("Calling SeqPrint for LFSRMulti instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < dimNum; ++i)
    {
        printf("%dth Dimension: ", i);
        for (int j = 0; j < outputLen; ++j)
        {
            printf("%u,", outSeq[i][j]);
        }
        printf("\n");
    }
}

void LFSRMulti::PlyPrint()
{
    printf("Calling PlyPrint for LFSRMulti instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < dimNum; ++i)
    {
        printf("%dth Dimension: %u\n", i, polyVal[i]);
    }
}

void LFSRMulti::SeqGen()
{
    static const unsigned int array4[] = {TAP_4};
    static const unsigned int array5[] = {TAP_5};
    static const unsigned int array6[] = {TAP_6};
    static const unsigned int array7[] = {TAP_7};
    static const unsigned int array8[] = {TAP_8};
    static const unsigned int array9[] = {TAP_9};
    static const unsigned int array10[] = {TAP_10};
    static const unsigned int array11[] = {TAP_11};
    static const unsigned int array12[] = {TAP_12};
    static const unsigned int array13[] = {TAP_13};
    static const unsigned int array14[] = {TAP_14};
    static const unsigned int array15[] = {TAP_15};
    static const unsigned int array16[] = {TAP_16};
    arrayLen[0] = sizeof(array4) / sizeof(array4[0]);
    arrayLen[1] = sizeof(array5) / sizeof(array5[0]);
    arrayLen[2] = sizeof(array6) / sizeof(array6[0]);
    arrayLen[3] = sizeof(array7) / sizeof(array7[0]);
    arrayLen[4] = sizeof(array8) / sizeof(array8[0]);
    arrayLen[5] = sizeof(array9) / sizeof(array9[0]);
    arrayLen[6] = sizeof(array10) / sizeof(array10[0]);
    arrayLen[7] = sizeof(array11) / sizeof(array11[0]);
    arrayLen[8] = sizeof(array12) / sizeof(array12[0]);
    arrayLen[9] = sizeof(array13) / sizeof(array13[0]);
    arrayLen[10] = sizeof(array14) / sizeof(array14[0]);
    arrayLen[11] = sizeof(array15) / sizeof(array15[0]);
    arrayLen[12] = sizeof(array16) / sizeof(array16[0]);

    if (mode == "delayed")
    {
        for (int i = 0; i < dimNum; ++i)
        {
            LFSR lfsrInst;
            lfsrInst.Init(lfsrLen,1,polyIndex,i*delayIndex,"lfsrInst");
            lfsrInst.SeqGen();
            outSeq[i] = lfsrInst.OutSeq();
            polyVal[i] = lfsrInst.OutPly();
        }
    }
    else if (mode == "incremental")
    {
        for (int i = 0; i < dimNum; ++i)
        {
            LFSR lfsrInst;
            lfsrInst.Init(lfsrLen,1,polyIndex+i,0,"lfsrInst");
            lfsrInst.SeqGen();
            outSeq[i] = lfsrInst.OutSeq();
            polyVal[i] = lfsrInst.OutPly();
        }
    }
    else if (mode == "random")
    {
        srand(time(NULL));
        for (int i = 0; i < dimNum; ++i)
        {
            realIndex = (unsigned int)rand()%arrayLen[lfsrLen-4];
            LFSR lfsrInst;
            lfsrInst.Init(lfsrLen,1,realIndex,(unsigned int)rand()%(unsigned int)pow(2,lfsrLen),"lfsrInst");
            lfsrInst.SeqGen();
            outSeq[i] = lfsrInst.OutSeq();
            polyVal[i] = lfsrInst.OutPly();
        }
    }
}

