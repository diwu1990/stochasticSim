#include "seqprobmulti.hpp"

SeqProbMulti::SeqProbMulti(){}
SeqProbMulti::~SeqProbMulti(){}
void SeqProbMulti::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SeqProbMulti Help. Following are instructions to SeqProbMulti Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SeqProbMulti inst.\n");
    printf("Initial Parameters: Bit Vector, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the Cross-Correlation of input bit vector from inst.Init().\n");

    printf("3. inst.SeqLen() method:\n");
    printf("Return the length of input sequence.\n");

    printf("4. inst.OutProb() method:\n");
    printf("Return the probability of input sequence.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void SeqProbMulti::Init(vector<vector<char>> param1, string param2)
{
    inSeq = param1;
    inDim = (unsigned int)inSeq.size();
    m_name = param2;
    seqLength.resize(inDim);
    outProb.resize(inDim);
    for (int i = 0; i < inDim; ++i)
    {
        seqLength[i] = (unsigned int)inSeq[i].size();
    }
    // for (int i = 0; i < inDim; ++i)
    // {
    //     for (int j = 0; j < seqLength[i]; ++j)
    //     {
    //         printf("%u,", inSeq[i][j]);
    //     }
    //     printf("\n");
    // }
}

void SeqProbMulti::Report()
{
    printf("Current SeqProbMulti:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Sequence Dimension:     %-5u\n", inDim);
}

void SeqProbMulti::ProbPrint()
{
    printf("Calling ProbPrint for SeqProbMulti instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < inDim; ++i)
    {
        printf("%f,", outProb[i]);
    }
    printf("\n");
}

void SeqProbMulti::Calc()
{
    for (int i = 0; i < inDim; ++i)
    {
        int oneNum = 0;
        for (int j = 0; j < seqLength[i]; ++j)
        {
            oneNum += inSeq[i][j];
        }
        // printf("%d\n", oneNum);
        outProb[i] = (float)oneNum/(float)seqLength[i];
    }
}

vector<unsigned int> SeqProbMulti::SeqLen()
{
    return seqLength;
}

vector<float> SeqProbMulti::OutProb()
{
    return outProb;
}
