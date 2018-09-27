#include "systemrand.hpp"

void SystemRand::Init(unsigned int param1, unsigned int param2, unsigned int param3, string param4)
{
    bitLength = param1;
    dimNum = param2;
    delay = param3;
    m_name = param4;
    if (bitLength < 4 || bitLength > 16)
    {
        printf("Error: Input bitLength exceeds the legal range (4~16) in SystemRand class.\n");
    }
    outputLen = (unsigned int)(pow(2,bitLength));
    outSeq.resize(outputLen);
    for (int i = 0; i < outputLen; ++i)
    {
        outSeq[i] = 0;
    }
}
 
SystemRand::SystemRand(){}
SystemRand::~SystemRand(){}
void SystemRand::Report()
{
    printf("Current SystemRand:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("SystemRand Length:      %-5u\n", bitLength);
    printf("Output Sequence Length: %-5u\n", outputLen);
}

void SystemRand::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SystemRand Help. Following are instructions to SystemRand Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SystemRand inst.\n");
    printf("Initial Parameters: SystemRand Bit Length, Dimension Index, Delay, Instance Name.\n");
    printf("In SystemRand Class, Dimension and Delay are useless. They are remained to align with SOBOL and LFSR.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing random number sequence from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current SystemRand instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current SystemRand instance.\n");

    printf("8. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current SystemRand instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

unsigned int SystemRand::SeqLen()
{
    return outputLen;
}

vector<unsigned int> SystemRand::OutSeq()
{
    return outSeq;
}

void SystemRand::SeqPrint()
{
    printf("Calling SeqPrint for SystemRand instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < outputLen; ++i)
    {
        printf("%u,", outSeq[i]);
    }
    printf("\n");
}

void SystemRand::SeqGen()
{
    srand(time(NULL));
    for (int i = 0; i < outputLen; ++i)
    {
        outSeq[i] = (unsigned int)(rand()%(unsigned int)pow(2,bitLength));
    }
}
