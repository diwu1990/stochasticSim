#include "synchronizer.hpp"
#include "crosscorrelation.hpp"

Synchronizer::Synchronizer()
{}

Synchronizer::~Synchronizer()
{}

void Synchronizer::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling Synchronizer Help. Following are instructions to Synchronizer Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the Synchronizer inst.\n");
    printf("Initial Parameters: Input Stochastic Bit Sequence, Buffer Depth, Instance Name.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequences with max positive correlation based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return new sequences from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current Synchronizer instance.\n");

    printf("5. inst.CCPrint() method:\n");
    printf("Print the content in the output sequence of current Synchronizer instance.\n");

    printf("6. inst.InCC() method:\n");
    printf("Return new sequences from inst.SeqGen().\n");

    printf("7. inst.OutCC() method:\n");
    printf("Return new sequences from inst.SeqGen().\n");

    printf("8. inst.PPStage() method:\n");
    printf("Return new sequences from inst.SeqGen().\n");

    printf("9. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current Synchronizer instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void Synchronizer::Init(vector<vector<unsigned int>> param1, unsigned int param2, string param3)
{
    inSeq = param1;
    depth = param2;
    m_name = param3;
    inDim = (unsigned int)inSeq.size();
    if (inDim != 2)
    {
        printf("Error: Input dimension is incorrect (Supposed to be 2).\n");
    }
    inLen = (unsigned int)inSeq[0].size();
    if (inLen != (unsigned int)inSeq[1].size())
    {
        printf("Error: Input length of two input sequences are not the same.\n");
    }
    inCC = 0;
    outCC = 0;
    ppStage = 1;
}

void Synchronizer::Report()
{
    printf("Current Synchronizer:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Input Dimension:        %-5u\n", inDim);
    printf("Input Length:           %-5u\n", inLen);
    printf("BUffer Depth:           %-5u\n", depth);
    printf("Pipeline Stage:         %-5u\n", ppStage);
}

void Synchronizer::SeqGen()
{
    crosscorrelation inCCInst;
    inCCInst.Init();
    inCCInst.CalcCC();
    inCC = inCCInst.OutCC();

    
    crosscorrelation outCCInst;
    outCCInst.Init();
    outCCInst.CalcCC();
    outCC = outCCInst.OutCC();
}

vector<vector<unsigned int>> Synchronizer::OutSeq()
{
    return outSeq;
}

float Synchronizer::InCC()
{
    return inCC;
}

float Synchronizer::OutCC()
{
    return outCC;
}

unsigned int Synchronizer::PPStage()
{
    return ppStage;
}

unsigned int Synchronizer::SeqLen()
{
    return inLen;
}

void Synchronizer::CCPrint()
{
    printf("Calling CCPrint for Synchronizer instance: ");
    std::cout << m_name << std::endl;
    printf("Input CC(%-.3f) => Output CC(%-.3f)\n", inCC, outCC);
}