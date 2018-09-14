#include "skewedsynchronizer.hpp"
#include "crosscorrelation.hpp"
#include "seqprobmulti.hpp"

SkewedSynchronizer::SkewedSynchronizer()
{}

SkewedSynchronizer::~SkewedSynchronizer()
{}

void SkewedSynchronizer::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SkewedSynchronizer Help. Following are instructions to SkewedSynchronizer Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SkewedSynchronizer inst.\n");
    printf("Initial Parameters: Input Stochastic Bit Sequence, Buffer Depth, Instance Name.\n");
    printf("The probability of input sequence 1 should be larger than sequence 0\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequences with max positive correlation based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return new sequences from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current SkewedSynchronizer instance.\n");

    printf("5. inst.CCPrint() method:\n");
    printf("Print the content in the output sequence of current SkewedSynchronizer instance.\n");

    printf("6. inst.InCC() method:\n");
    printf("Return new sequences from inst.SeqGen().\n");

    printf("7. inst.OutCC() method:\n");
    printf("Return new sequences from inst.SeqGen().\n");

    printf("8. inst.PPStage() method:\n");
    printf("Return new sequences from inst.SeqGen().\n");

    printf("9. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current SkewedSynchronizer instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void SkewedSynchronizer::Init(vector<vector<unsigned int>> param1, unsigned int param2, string param3)
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
    // if (inProbInst.OutProb()[0] > inProbInst.OutProb()[1])
    // {
    //     // printf("Error: Probability of input sequence 0 (%-.3f) is larger than that of input sequence 1 (%-.3f).\n", inProbInst.OutProb()[0], inProbInst.OutProb()[1]);
    // }
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
    ppStage = 1;
    errRate.resize(inDim);
}

void SkewedSynchronizer::Report()
{
    printf("Current SkewedSynchronizer:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Input Dimension:        %-5u\n", inDim);
    printf("Input Length:           %-5u\n", inLen);
    printf("Buffer Depth:           %-5u\n", depth);
    printf("Pipeline Stage:         %-5u\n", ppStage);
}

void SkewedSynchronizer::SeqGen()
{
    /*keep no reg for sequence 1*/
    /*it has high accuracy, but low correlation ()*/
    /*correlation is can be less than 95%*/

    unsigned int upperbound = (unsigned int)pow(2,depth)-1;

    unsigned int saturateCnt = 0;
    // here the bit order of sequence 1 is not changed.
    for (int i = 0; i < inLen; ++i)
    {
        if (inSeq[0][i] == 1 && inSeq[1][i] == 1)
        {
            outSeq[0][i] = inSeq[0][i];
            outSeq[1][i] = inSeq[1][i];
        }
        else if (inSeq[0][i] == 0 && inSeq[1][i] == 1)
        {
            if (saturateCnt > 0)
            {
                outSeq[0][i] = 1;
                saturateCnt--;
            }
            else
            {
                outSeq[0][i] = 0;
            }
            outSeq[1][i] = 1;
        }
        else if (inSeq[0][i] == 0 && inSeq[1][i] == 0)
        {
            outSeq[0][i] = 0;
            outSeq[1][i] = 0;
        }
        else if (inSeq[0][i] == 1 && inSeq[1][i] == 0)
        {
            if (saturateCnt == upperbound)
            {
                outSeq[0][i] = 1;
                outSeq[1][i] = 0;
            }
            else
            {
                outSeq[0][i] = 0;
                outSeq[1][i] = 0;
                saturateCnt = saturateCnt + 1;
                if (saturateCnt > upperbound)
                {
                    saturateCnt = upperbound;
                }
            }
        }
    }


    /*keep one reg for sequence 1*/
    /*it has low accuracy, but high correlation*/
    /*error rate is always larger than 0.1% at length 2^8*/
    /*correlation is always larger than 99%*/
    
    // unsigned int upperbound = (unsigned int)pow(2,depth)-1;

    // unsigned int saturateCnt = 0;
    // unsigned int saturateCnt2 = 0;
    // for (int i = 0; i < inLen; ++i)
    // {
    //     if (inSeq[0][i] == 0 && inSeq[1][i] == 0)
    //     {
    //         outSeq[0][i] = 0;
    //         outSeq[1][i] = 0;
    //     }
    //     else if (inSeq[0][i] == 1 && inSeq[1][i] == 1)
    //     {
    //         outSeq[0][i] = 1;
    //         outSeq[1][i] = 1;
    //     }
    //     else if (inSeq[0][i] == 0 && inSeq[1][i] == 1)
    //     {
    //         if (saturateCnt > 0)
    //         {
    //             outSeq[0][i] = 1;
    //             outSeq[1][i] = 1;
    //             saturateCnt--;
    //         }
    //         else
    //         {
    //             outSeq[0][i] = 0;
    //             if (saturateCnt2 == 1)
    //             {
    //                 outSeq[1][i] = 1;
    //             }
    //             else
    //             {
    //                 outSeq[1][i] = 0;
    //                 saturateCnt2++;
    //             }
    //         }
    //     }
    //     else if (inSeq[0][i] == 1 && inSeq[1][i] == 0)
    //     {
    //         if (saturateCnt2 > 0)
    //         {
    //             outSeq[0][i] = 1;
    //             outSeq[1][i] = 1;
    //             saturateCnt2--;
    //         }
    //         else
    //         {
    //             outSeq[1][i] = 0;
    //             if (saturateCnt == upperbound)
    //             {
    //                 outSeq[0][i] = 1;
    //             }
    //             else
    //             {
    //                 outSeq[0][i] = 0;
    //                 saturateCnt++;
    //             }
    //         }
    //     }
    // }

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

vector<vector<unsigned int>> SkewedSynchronizer::OutSeq()
{
    return outSeq;
}

float SkewedSynchronizer::InCC()
{
    return inCC;
}

float SkewedSynchronizer::OutCC()
{
    return outCC;
}

vector<float> SkewedSynchronizer::InProb()
{
    return inProb;
}

vector<float> SkewedSynchronizer::OutProb()
{
    return outProb;
}

unsigned int SkewedSynchronizer::PPStage()
{
    return ppStage;
}

unsigned int SkewedSynchronizer::SeqLen()
{
    return inLen;
}

vector<float> SkewedSynchronizer::ErrRate()
{
    return errRate;
}

void SkewedSynchronizer::CCPrint()
{
    printf("Calling CCPrint for SkewedSynchronizer instance: ");
    std::cout << m_name << std::endl;
    printf("Input CC(%-.3f) => Output CC(%-.3f)\n", inCC, outCC);
}

void SkewedSynchronizer::ErrPrint()
{
    printf("Calling ErrPrint for SkewedSynchronizer instance: ");
    std::cout << m_name << std::endl;
    printf("Error rate of input sequence 0: %-.3f\n", errRate[0]);
    printf("Error rate of input sequence 1: %-.3f\n", errRate[1]);
}

void SkewedSynchronizer::ProbPrint()
{
    printf("Calling ProbPrint for SkewedSynchronizer instance: ");
    std::cout << m_name << std::endl;
    printf("Input Prob[0](%-.3f) => Output Prob[0](%-.3f)\n", inProb[0], outProb[0]);
    printf("Input Prob[1](%-.3f) => Output Prob[1](%-.3f)\n", inProb[1], outProb[1]);
}

void SkewedSynchronizer::SeqPrint()
{
    printf("Calling SeqPrint for SkewedSynchronizer instance: ");
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
    int same = 1;
    for (int i = 0; i < inLen; ++i)
    {
        if (inSeq[1][i] != outSeq[1][i])
        {
            same = 0;
            break;
        }
    }
    if (same == 1)
    {
        printf("Input Sequence 1 is the same as output sequence 1.\n");
        for (int i = 0; i < inLen; ++i)
        {
            printf("%u, ", inSeq[1][i]);
        }
        printf("\n");
    }
    else if (same == 0)
    {
        printf("Error: Input Sequence 1 is not the same as output sequence 1.\n");
    }
}
