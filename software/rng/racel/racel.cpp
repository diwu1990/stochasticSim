#include "racel.hpp"

void RACEL::Init(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4, string param5)
{
    racelLen = param1;
    m_name = param5;
    outputLen = (unsigned int)pow(2,racelLen);
    outSeq.resize(outputLen);
    for (int i = 0; i < outputLen; ++i)
    {
        outSeq[i] = 0;
    }
}

RACEL::RACEL(){}

RACEL::~RACEL(){}

void RACEL::Report()
{
    printf("Current RACEL:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("RACEL Length:            %-5u\n", racelLen);
    printf("Output Sequence Length: %-5u\n", outputLen);
}

void RACEL::Help()
{
    // init params: racelLen, initState, polyIdx
    // the legal range of racelLen is 4~16
    // initState/polyIdx are not useful
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling RACEL Help. Following are instructions to RACEL Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the RACEL inst.\n");
    printf("Initial Parameters: RACEL Bit Length, Initial State, Polynomial Index, Output Delay, Instance Name.\n");
    printf("The legal range of RACEL Bit Length is 4~16.\n");
    printf("Initial State, Polynomial Index and Output Delay are not used, but kept for compatibility.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing random number sequence from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current RACEL instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current RACEL instance.\n");

    printf("6. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current RACEL instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

unsigned int RACEL::SeqLen()
{
    return outputLen;
}

vector<unsigned int> RACEL::OutSeq()
{
    return outSeq;
}

void RACEL::SeqPrint()
{
    printf("Calling SeqPrint for RACEL instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < outputLen; ++i)
    {
        printf("%u,", outSeq[i]);
    }
    printf("\n");
}

void RACEL::SeqGen()
{
    // racelLen is the current racel length, and should be from 4 to 16

    for (int i = 0; i < outputLen; ++i)
    {
        outSeq[i] = i;
    }
}