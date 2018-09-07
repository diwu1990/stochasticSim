#include "synchronizer.hpp"
#include "crosscorrelation.hpp"
#include "seqprobmulti.hpp"

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
    printf("Mainly to match logic ones with logic ones.\n");
    printf("Buffer Depth is supposed to be 1.\n");
    printf("Increasing Buffer Depth will increase the output CC, but decrease the output accuracy.\n");

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
    SeqProbMulti inProbInst;
    inProbInst.Init(inSeq,"inProbInst");
    inProbInst.Calc();
    inLen = (unsigned int)inSeq[0].size();
    if (inLen != (unsigned int)inSeq[1].size())
    {
        printf("Error: Input length of two input sequences are not the same.\n");
    }
    inCC = 0;
    outCC = 0;
    outSeq.resize(inDim);
    inProb.resize(inDim);
    inProb[0] = inProbInst.OutProb()[0];
    inProb[1] = inProbInst.OutProb()[1];
    outProb.resize(inDim);
    for (int i = 0; i < inDim; ++i)
    {
        outSeq[i].resize(inLen);
        for (int j = 0; j < inLen; ++j)
        {
            if (inSeq[i][j] != 0 && inSeq[i][j] != 1)
            {
                printf("Error: Input sequences are not in bit format.\n");
                break;
            }
        }
    }
    errRate.resize(inDim);
    ppStage = 1;
}

void Synchronizer::Report()
{
    printf("Current Synchronizer:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Input Dimension:        %-5u\n", inDim);
    printf("Input Length:           %-5u\n", inLen);
    printf("Buffer Depth:           %-5u\n", depth);
    printf("Pipeline Stage:         %-5u\n", ppStage);
}

void Synchronizer::SeqGen()
{
    unsigned int upperbound = (unsigned int)pow(2,depth)-1;

    unsigned int saturateCnt = 0;
    unsigned int saturateCnt2 = 0;
    for (int i = 0; i < inLen; ++i)
    {
        if (inSeq[0][i] == 0 && inSeq[1][i] == 0)
        {
            outSeq[0][i] = 0;
            outSeq[1][i] = 0;
        }
        else if (inSeq[0][i] == 1 && inSeq[1][i] == 1)
        {
            outSeq[0][i] = 1;
            outSeq[1][i] = 1;
        }
        else if (inSeq[0][i] == 0 && inSeq[1][i] == 1)
        {
            if (saturateCnt > 0)
            {
                outSeq[0][i] = 1;
                outSeq[1][i] = 1;
                saturateCnt--;
            }
            else
            {
                outSeq[0][i] = 0;
                if (saturateCnt2 == upperbound)
                {
                    outSeq[1][i] = 1;
                }
                else
                {
                    outSeq[1][i] = 0;
                    saturateCnt2++;
                }
            }
        }
        else if (inSeq[0][i] == 1 && inSeq[1][i] == 0)
        {
            if (saturateCnt2 > 0)
            {
                outSeq[0][i] = 1;
                outSeq[1][i] = 1;
                saturateCnt2--;
            }
            else
            {
                outSeq[1][i] = 0;
                if (saturateCnt == upperbound)
                {
                    outSeq[0][i] = 1;
                }
                else
                {
                    outSeq[0][i] = 0;
                    saturateCnt++;
                }
            }
        }
    }

    CrossCorrelation inCCInst;
    inCCInst.Init(inSeq, 1, "inCCInst");
    inCCInst.Calc();
    inCC = inCCInst.OutCC()[0];

    SeqProbMulti outProbInst;
    outProbInst.Init(outSeq,"outProbInst");
    outProbInst.Calc();
    outProb[0] = outProbInst.OutProb()[0];
    outProb[1] = outProbInst.OutProb()[1];

    CrossCorrelation outCCInst;
    outCCInst.Init(outSeq, 1, "outCCInst");
    outCCInst.Calc();
    outCC = outCCInst.OutCC()[0];

    errRate[0] = (outProb[0] - inProb[0]);
    errRate[1] = (outProb[1] - inProb[1]);
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

vector<float> Synchronizer::InProb()
{
    return inProb;
}

vector<float> Synchronizer::OutProb()
{
    return outProb;
}

unsigned int Synchronizer::PPStage()
{
    return ppStage;
}

unsigned int Synchronizer::SeqLen()
{
    return inLen;
}

vector<float> Synchronizer::ErrRate()
{
    return errRate;
}

void Synchronizer::CCPrint()
{
    printf("Calling CCPrint for Synchronizer instance: ");
    std::cout << m_name << std::endl;
    printf("Input CC(%-.3f) => Output CC(%-.3f)\n", inCC, outCC);
}

void Synchronizer::ErrPrint()
{
    printf("Calling ErrPrint for Synchronizer instance: ");
    std::cout << m_name << std::endl;
    printf("Error rate of input sequence 0: %-.3f\n", errRate);
}

void Synchronizer::ProbPrint()
{
    printf("Calling ProbPrint for Synchronizer instance: ");
    std::cout << m_name << std::endl;
    printf("Input Prob[0](%-.3f) => Output Prob[0](%-.3f)\n", inProb[0], outProb[0]);
    printf("Input Prob[1](%-.3f) => Output Prob[1](%-.3f)\n", inProb[1], outProb[1]);
}

void Synchronizer::SeqPrint()
{
    printf("Calling SeqPrint for Synchronizer instance: ");
    std::cout << m_name << std::endl;
    printf("Input Sequence 0:\n");
    for (int i = 0; i < inLen; ++i)
    {
        printf("%u, ", inSeq[0][i]);
    }
    printf("\n");
    printf("Output Sequence 0:\n");
    for (int i = 0; i < inLen; ++i)
    {
        printf("%u, ", outSeq[0][i]);
    }
    printf("\n");

    printf("Input Sequence 1:\n");
    for (int i = 0; i < inLen; ++i)
    {
        printf("%u, ", inSeq[1][i]);
    }
    printf("\n");
    printf("Output Sequence 1:\n");
    for (int i = 0; i < inLen; ++i)
    {
        printf("%u, ", outSeq[1][i]);
    }
    printf("\n");
}
