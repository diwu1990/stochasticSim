#include "randNum2Bit.hpp"

void RandNum2Bit::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling RandNum2Bit Help. Following are instructions to RandNum2Bit Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the RandNum2Bit inst.\n");
    printf("The initial parameters are: Expectation, Input Random Number Bit Length, Random Number Sequence, Unipolar Enable, Instance Name.\n");
    printf("If Unipolar Enable is 0, use bipolar format; otherwise use unipolar format.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate one random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing stochastic bits from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current RandNum2Bit instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current RandNum2Bit instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

RandNum2Bit::RandNum2Bit()
{
    // printf("RandNum2Bit inst is created.\n");
}

RandNum2Bit::~RandNum2Bit()
{
    // printf("RandNum2Bit inst is deleted.\n");
}

void RandNum2Bit::SeqPrint()
{
    printf("Calling SeqPrint for RandNum2Bit instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < NumVec.size(); ++i)
    {
        printf("%u,", bitVec[i]);
    }
    printf("\n");
}

void RandNum2Bit::Report()
{
    printf("Current RandNum2Bit:\n");
    std::cout << "Instance name: " << m_name << std::endl;
    printf("Expectation:   %-f\n", expectation);
    printf("Bit Length:    %-u\n", bitLength);
}

void RandNum2Bit::Init(float param1, unsigned int param2, vector<unsigned int> param3, unsigned int param4, string param5)
{
    expectation = param1;
    bitLength = param2;
    NumVec = param3;
    unipolar = param4;
    m_name = param5;
    bitVec.resize((unsigned int)NumVec.size());
    for (int i = 0; i < (unsigned int)NumVec.size(); ++i)
    {
        bitVec[i] = 0;
    }
}

vector<char> RandNum2Bit::OutSeq()
{
    return bitVec;
}

void RandNum2Bit::SeqGen()
{
    unsigned int maxVal = (unsigned int)pow(2,bitLength);
    unsigned int expectationInt;
    if (unipolar == 0)
    {
        expectationInt = (unsigned int)((expectation+1)/2 * maxVal);
    }
    else
    {
        if (expectation < 0)
        {
            printf("Error: Input probability can't be negative when unipolar data is applied.\n");
        }
        expectationInt = (unsigned int)(expectation * maxVal);
    }
    for (int i = 0; i < NumVec.size(); ++i)
    {
        if (NumVec[i] < expectationInt)
        {
            bitVec[i] = 1;
        }
        else
        {
            bitVec[i] = 0;
        }
    }
}
