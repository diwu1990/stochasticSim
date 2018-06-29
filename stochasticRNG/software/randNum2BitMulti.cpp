#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"

void RandNum2BitMulti::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling RandNum2BitMulti Help. Following are instructions to RandNum2BitMulti Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the RandNum2BitMulti inst.\n");
    printf("The initial parameters are: Expectation, Input Random Number Bit Length, Random Number Sequence, Instance Name.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate one random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing stochastic bits from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current RandNum2BitMulti instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current RandNum2BitMulti instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

RandNum2BitMulti::RandNum2BitMulti()
{
    // printf("RandNum2BitMulti inst is created\n");
}

RandNum2BitMulti::~RandNum2BitMulti()
{
    // printf("RandNum2BitMulti inst is deleted\n");

}

void RandNum2BitMulti::SeqPrint()
{
    printf("Calling SeqPrint for RandNum2BitMulti instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < seqDim; ++i)
    {
        for (int j = 0; j < (unsigned int)NumVec[i].size(); ++j)
        {
            printf("%u,", bitVec[i][j]);
        }
        printf("\n");
    }
}

void RandNum2BitMulti::Report()
{
    printf("Current RandNum2BitMulti:\n");
    std::cout << "Instance name: " << m_name << std::endl;
    printf("Sequence Dim:  %-u\n", seqDim);
}

void RandNum2BitMulti::Init(vector<float> param1, vector<unsigned int> param2, vector<vector<unsigned int>> param3, string param4)
{
    expectation = param1;
    bitLength = param2;
    NumVec = param3;
    m_name = param4;
    if ((unsigned int)expectation.size() == (unsigned int)bitLength.size() && (unsigned int)expectation.size() == (unsigned int)NumVec.size())
    {
        seqDim = (unsigned int)expectation.size();
    }
    else
    {
        printf("Error: Input Dimension values do not match!\n");
    }
    bitVec.resize((unsigned int)NumVec.size());
    for (int i = 0; i < seqDim; ++i)
    {
        bitVec[i].resize((unsigned int)(NumVec[i].size()));
        for (int j = 0; j < (unsigned int)(NumVec[i].size()); ++j)
        {
            bitVec[i][j] = 0;
        }
    }
}

vector<vector<unsigned int>> RandNum2BitMulti::OutSeq()
{
    return bitVec;
}

void RandNum2BitMulti::SeqGen()
{
    for (int i = 0; i < seqDim; ++i)
    {
        RandNum2Bit num2bit;
        num2bit.Init(expectation[i],bitLength[i],NumVec[i],"num2bit");
        // num2bit.Report();
        num2bit.SeqGen();
        // num2bit.SeqPrint();
        bitVec[i] = num2bit.OutSeq();
        num2bit.SeqPrint();
    }
}
