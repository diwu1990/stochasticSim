#include "systemrandmulti.hpp"

void SystemRandMulti::Init(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4, string param5, string param6)
{
    dimNum = param1;
    dimIndex = param2;
    if (dimIndex < 1)
    {
        printf("Error: Input Dimensional Index is invalid. (should be larger than 1)\n");
    }
    delayIndex = param3;
    bitLength = param4;
    outputLen = (unsigned int)(pow(2,bitLength));
    mode = param5;
    if (mode != "delayed" && mode != "incremental" && mode != "random")
    {
        printf("Error: Input mode is invalid. (delayed or incremental or random)\n");
    }

    m_name = param6;
    
    if (bitLength < 4 || bitLength > 16)
    {
        printf("Error: Input bitLength exceeds the legal range (4~16) in SystemRandMulti class.\n");
    }
    outSeq.resize(dimNum);
    for (int i = 0; i < dimNum; ++i)
    {
        outSeq[i].resize(outputLen);
    }
}

SystemRandMulti::SystemRandMulti(){}
SystemRandMulti::~SystemRandMulti(){}
void SystemRandMulti::Report()
{
    printf("Current SystemRandMulti:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("SystemRandMulti Length:      %-5u\n", bitLength);
    printf("Dimension Number:       %-5u\n", dimNum);
    printf("Delay Index:            %-5u\n", delayIndex);
    printf("Output Sequence Length: %-5u\n", outputLen);
}

void SystemRandMulti::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SystemRandMulti Help. Following are instructions to SystemRandMulti Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SystemRandMulti inst.\n");
    printf("Initial Parameters: Number of Dimension, Index of Initial Dimension, Delay Index, Bit Length, Mode, Instance Name.\n");
    printf("1) Number of Dimension: Number of Output Sequences.\n");
    printf("4) Bit Length: Length of SystemRand Vectors.\n");
    printf("6) Instance Name: Name of current SystemRandMulti Instance.\n");
    printf("All other parameters are useless for SystemRandMulti.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing random number sequence from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current SystemRandMulti instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current SystemRandMulti instance.\n");

    printf("6. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current SystemRandMulti instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

unsigned int SystemRandMulti::SeqLen()
{
    return outputLen;
}

vector<vector<unsigned int>> SystemRandMulti::OutSeq()
{
    return outSeq;
}

void SystemRandMulti::SeqPrint()
{
    printf("Calling SeqPrint for SystemRandMulti instance: ");
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

void SystemRandMulti::SeqGen()
{
    srand(time(NULL));
    for (int i = 0; i < dimNum; ++i)
    {
        for (int j = 0; j < outputLen; ++j)
        {
            outSeq[i][j] = (unsigned int)(rand()%(unsigned int)pow(2,bitLength));
        }
    }
}

