#include "seqprob.hpp"

SeqProb::SeqProb(){}
SeqProb::~SeqProb(){}
void SeqProb::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SeqProb Help. Following are instructions to SeqProb Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SeqProb inst.\n");
    printf("Initial Parameters: Bit Vector, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the Cross-Correlation of input bit vector from inst.Init().\n");

    printf("3. inst.SeqLen() method:\n");
    printf("Return the length of input sequence.\n");

    printf("4. inst.OutProb() method:\n");
    printf("Return the probability of input sequence.\n");

    printf("5. inst.ProbPrint() method:\n");
    printf("Print the probability of input sequence.\n");

    printf("6. inst.Report() method:\n");
    printf("Report the status of current instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void SeqProb::Init(vector<char> param1, string param2)
{
    inSeq = param1;
    m_name = param2;
    seqLength = (unsigned int)inSeq.size();
}

void SeqProb::Report()
{
    printf("Current SeqProb:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Sequence Length:        %-5u\n", seqLength);
}

void SeqProb::ProbPrint()
{
    printf("Calling ProbPrint for SeqProb instance: ");
    std::cout << m_name << std::endl;
    printf("%f\n", outProb);
}

void SeqProb::Calc()
{
    int oneNum = 0;
    for (int i = 0; i < seqLength; ++i)
    {
        oneNum += inSeq[i];
    }
    outProb = (float)oneNum/(float)seqLength;
}

unsigned int SeqProb::SeqLen()
{
    return seqLength;
}

float SeqProb::OutProb()
{
    return outProb;
}

