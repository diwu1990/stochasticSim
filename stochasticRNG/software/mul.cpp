#include "mul.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

MUL::MUL(){}

MUL::~MUL(){}

void MUL::Help()
{
    // void Report();
    // void OutPrint();
    // vector<unsigned int> OutSeq();
    // float InCC();
    // vector<float> InProb();
    // float TheoProb();
    // vector<float> RealProb();
    // float FinalRealProb();
    // vector<float> ErrRate();
    // float FinalErrRate();
    // unsigned int SeqLen();
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling MUL Help. Following are instructions to MUL Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the MUL inst.\n");
    printf("Initial Parameters: Two Input Vectors.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the product of two input sequences.\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the calculated result.\n");

    printf("4. inst.OutPrint() method:\n");
    printf("Print the information of result.\n");

    printf("5. inst.InCC() method:\n");
    printf("Return the input crosscorrelation.\n");

    printf("6. inst.InProb() method:\n");
    printf("Return the input probability.\n");

    printf("7. inst.TheoProb() method:\n");
    printf("Return the theoretical output probability.\n");

    printf("8. inst.RealProb() method:\n");
    printf("Return the array of progressive output probability.\n");

    printf("9. inst.FinalRealProb() method:\n");
    printf("Return the final output probability.\n");

    printf("10. inst.ErrRate() method:\n");
    printf("Return the array of progressive output error rate.\n");

    printf("11. inst.FinalErrRate() method:\n");
    printf("Return the final output error rate.\n");

    printf("12. inst.SeqLen() method:\n");
    printf("Return the sequence length.\n");

    printf("13. inst.LowErrLen() method:\n");
    printf("Return the sequence length required to converge with less than 5 percent error rate.\n");

    printf("14. inst.PPStage() method:\n");
    printf("Return the pipline stages required by hardware.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void MUL::Init(vector<vector<unsigned int>> param1, string param2)
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    m_name = param2;
    if ((unsigned int)inSeq.size() == (unsigned int)inProb.size() && (unsigned int)inSeq.size() == 2)
    {
        inDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Dimension is not 2.\n");
    }

    if ((unsigned int)inSeq[0].size() == (unsigned int)inSeq[1].size())
    {
        seqLength = (unsigned int)inSeq[0].size();
    }
    else
    {
        printf("Error: Input Length is not the same.\n");
    }
    theoProb = inProb[0] * inProb[1];
    outSeq.resize(seqLength);
    realProb.resize(seqLength);
    errRate.resize(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        outSeq[i] = 0;
        realProb[i] = 0;
        errRate[i] = 0;
    }
    lowErrLen = 0;
    // for (int i = 0; i < inDim; ++i)
    // {
    //     for (int j = 0; j < seqLength; ++j)
    //     {
    //         printf("%u,", inSeq[i][j]);
    //     }
    //     printf("\n");
    // }
    ppStage = 0;
}

void MUL::Report()
{
    printf("Current MUL:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f,%f\n", inProb[0], inProb[1]);
    printf("Theoretical Probability:%f\n", theoProb);
}

void MUL::Calc()
{
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.Calc();
    inCC = inputCC.OutCC()[0];

    unsigned int oneCount = 0;
    for (int i = 0; i < seqLength; ++i)
    {
        if (inSeq[0][i] == 1 && inSeq[1][i] == 1)
        {
            outSeq[i] = 1;
            oneCount += 1;
            if (i < 32)
            {
                realProb[i] = (float)oneCount/(float)(i+1);
            }
            else
            {
                realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
            }
            errRate[i] = (theoProb - realProb[i])/theoProb;
        }
        else
        {
            outSeq[i] = 0;
            if (i < 32)
            {
                realProb[i] = (float)oneCount/(float)(i+1);
            }
            else
            {
                realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
            }
            errRate[i] = (theoProb - realProb[i])/theoProb;
        }
    }
    for (int i = 0; i < seqLength; ++i)
    {
        // printf("%f\n", errRate[seqLength-1-i]);
        // if (errRate[seqLength-1-i] > 0.05)
        // {
        //     printf("larger than 0.05\n");
        // }
        // if (errRate[seqLength-1-i] < -0.05)
        // {
        //     printf("smaller than -0.05\n");
        // }
        if (errRate[seqLength-1-i] > 0.05 || errRate[seqLength-1-i] < -0.05)
        {
            lowErrLen = seqLength-i;
            break;
        }
    }
}

vector<unsigned int> MUL::OutSeq()
{
    return outSeq;
}

unsigned int MUL::PPStage()
{
    return ppStage;
}

void MUL::OutPrint()
{
    printf("Calling OutPrint for MUL instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: %.3f x %.3f = %.3f with input crosscorrelation %.3f\n", inProb[0],inProb[1], theoProb, inCC);
    printf("Final Probability: %.3f with Error Rate: %.3f\n", realProb[seqLength-1], errRate[seqLength-1]);
    printf("Low Error Length (5 percent approximation): %u\n", lowErrLen);
    // for (int i = 0; i < seqLength; ++i)
    // {
    //     printf("%.3f,", errRate[i]);
    // }
    // printf("\n");
    // printf("Output Probability:\n");
    // for (int i = 0; i < seqLength; ++i)
    // {
    //     printf("%.3f,", realProb[i]);
    // }
    // printf("\n");
}

float MUL::InCC()
{
    return inCC;
}

vector<float> MUL::InProb()
{
    return inProb;
}

float MUL::TheoProb()
{
    return theoProb;
}

vector<float> MUL::RealProb()
{
    return realProb;
}

float MUL::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> MUL::ErrRate()
{
    return errRate;
}

float MUL::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int MUL::SeqLen()
{
    return seqLength;
}

unsigned int MUL::LowErrLen()
{
    return lowErrLen;
}


