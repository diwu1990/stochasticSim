#include "muxadd.hpp"
#include "seqprob.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

MUXADD::MUXADD(){}

MUXADD::~MUXADD(){}

void MUXADD::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling MUXADD Help. Following are instructions to MUXADD Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the MUXADD inst.\n");
    printf("Initial Parameters: Input Vectors, Random NUmbers, Bit Length of Input Random Number, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the sum of two input sequences.\n");

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
    printf("Return the sequence length.\n");

    printf("14. inst.Report() method:\n");
    printf("Report the configuration for current instance.\n");

    printf("14. inst.PPStage() method:\n");
    printf("Report the pipeline stage for current instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void MUXADD::Init(vector<vector<char>> param1, vector<unsigned int> param2, string param3)
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randNum = param2;
    m_name = param3;
    if ((unsigned int)inSeq.size() == (unsigned int)inProb.size() && (unsigned int)inSeq.size() >= 2)
    {
        inDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Dimensions of Sequences and Probabilities mismatch.\n");
    }


    seqLength = (unsigned int)inSeq[0].size();
    for (int i = 0; i < inDim; ++i)
    {
        if ((unsigned int)inSeq[i].size() != seqLength)
        {
            printf("Error: Input Length is not the same.\n");
            break;
        }
    }

    for (int i = 0; i < seqLength; ++i)
    {
        if (randNum[i] > inDim-1)
        {
            printf("Error: Iput Random Number is larger than the expected range(%d).\n",inDim);
            break;
        }
    }

    theoProb = inProb[0];
    for (int i = 1; i < inDim; ++i)
    {
        theoProb += inProb[i];
    }
    if (inDim == 2)
    {
        SeqProb selProbCalc;
        selProbCalc.Init(randNum,"selProbCalc");
        selProbCalc.Calc();
        theoProb = inProb[0]*(1 - selProbCalc.OutProb()) + inProb[1]*selProbCalc.OutProb();
    }
    else
    {
        theoProb = theoProb/(float)inDim; // '+' for summation, '*' for multiplication?
    }
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
    ppStage=0;
}

void MUXADD::Report()
{
    printf("Current MUXADD:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Theoretical Probability:%f\n", theoProb);
}

void MUXADD::Calc()
{
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.Calc();
    inCC.resize(inputCC.OutCC().size());
    inCC = inputCC.OutCC();

    unsigned int oneCount = 0;
    unsigned int accuracyLength = 128;

    for (int i = 0; i < seqLength; ++i)
    {
        outSeq[i] = inSeq[randNum[i]][i];
        oneCount += outSeq[i];
        if (i < accuracyLength)
        {
            realProb[i] = (float)oneCount/(float)(i+1);
        }
        else
        {
            realProb[i] = (realProb[i-1]*(float)accuracyLength+outSeq[i]-outSeq[i-accuracyLength])/(float)accuracyLength;
        }
        errRate[i] = (theoProb - realProb[i]);
    }

    // find the convergence point
    for (int i = 0; i < seqLength; ++i)
    {
        if (errRate[seqLength-1-i] > 0.05 || errRate[seqLength-1-i] < -0.05)
        {
            lowErrLen = seqLength-i;
            break;
        }
    }
}

vector<char> MUXADD::OutSeq()
{
    return outSeq;
}

void MUXADD::OutPrint()
{
    printf("Calling OutPrint for MUXADD instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: (%.3f + %.3f)/%d = %.3f with input crosscorrelation %.3f\n", inProb[0],inProb[1], inDim, theoProb, inCC);
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

vector<float>  MUXADD::InCC()
{
    return inCC;
}

vector<float> MUXADD::InProb()
{
    return inProb;
}

float MUXADD::TheoProb()
{
    return theoProb;
}

vector<float> MUXADD::RealProb()
{
    return realProb;
}

float MUXADD::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> MUXADD::ErrRate()
{
    return errRate;
}

float MUXADD::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int MUXADD::SeqLen()
{
    return seqLength;
}

unsigned int MUXADD::LowErrLen()
{
    return lowErrLen;
}

unsigned int MUXADD::PPStage()
{
    return ppStage;
}
