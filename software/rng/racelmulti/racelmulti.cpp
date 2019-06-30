#include "racelmulti.hpp"
#include "racel.hpp"

void RACELMulti::Init(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4, string param5, string param6)
{
    dimNum = param1;
    racelLen = param4;
    outputLen = (unsigned int)(pow(2,racelLen))-1;
    m_name = param6;
    if (racelLen < 4 || racelLen > 16)
    {
        printf("Error: Input racelLen exceeds the legal range (4~16) in RACELMulti class.\n");
    }
    outSeq.resize(dimNum);
    for (int i = 0; i < dimNum; ++i)
    {
        outSeq[i].resize(outputLen);
    }
}

RACELMulti::RACELMulti(){}

RACELMulti::~RACELMulti(){}

void RACELMulti::Report()
{
    printf("Current RACELMulti:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("RACELMulti Length:       %-5u\n", racelLen);
    printf("Dimension Number:       %-5u\n", dimNum);
    printf("Output Sequence Length: %-5u\n", outputLen);
}

void RACELMulti::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling RACELMulti Help. Following are instructions to RACELMulti Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the RACELMulti inst.\n");
    printf("Initial Parameters: Number of Dimension, Index of Initial Polynomial, Delay Index, RACEL Bit Length, RACEL Mode, Instance Name.\n");
    printf("1) Number of Dimension: Number of Output Sequences.\n");
    printf("2) (not used) Index of Initial Polynomial: The starting index of polynomial.\n");
    printf("3) Index of RACEL delay: The interval index of output delay.\n");
    printf("4) (not used) RACEL Bit Length: Bit Length of RACEL.\n");
    printf("5) (not used) RACEL Mode: Define the relationship between each racel sequence, delayed means the each sequence is delayed one cycle from another, incremental means racel sequence of different dimension with same delay.\n");
    printf("6) Instance Name: Name of current RACELMulti Instance.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing random number sequence from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current RACELMulti instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current RACELMulti instance.\n");

    printf("6. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current RACELMulti instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

unsigned int RACELMulti::SeqLen()
{
    return outputLen;
}

vector<vector<unsigned int>> RACELMulti::OutSeq()
{
    return outSeq;
}

void RACELMulti::SeqPrint()
{
    printf("Calling SeqPrint for RACELMulti instance: ");
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

void RACELMulti::SeqGen()
{
    RACEL racelInst;
    racelInst.Init(racelLen,1,1,1,"racelInst");
    racelInst.SeqGen();
    for (int i = 0; i < dimNum; ++i)
    {
        outSeq[i] = racelInst.OutSeq();
    }
}

