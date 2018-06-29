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
    if (mode != "delayed" && mode != "incremental")
    {
        printf("Error: Input mode is invalid. (delayed or incremental)\n");
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
}

