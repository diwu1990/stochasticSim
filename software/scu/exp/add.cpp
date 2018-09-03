#include "add.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

ADD::ADD(){}

ADD::~ADD(){}

void ADD::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling ADD Help. Following are instructions to ADD Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the ADD inst.\n");
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

void ADD::Init(vector<vector<unsigned int>> param1, vector<unsigned int> param2, unsigned int param3, string param4)
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randNum = param2;
    bitLength = param3;
    m_name = param4;
    if ((unsigned int)inSeq.size() == (unsigned int)inProb.size() && (unsigned int)inSeq.size() >= 2)
    {
        inDim = (unsigned int)inSeq.size();
        float logInDimfloat = log2(inDim);
        if (ceil(logInDimfloat) != floor(logInDimfloat))
        {
            printf("Error: Input Dimension is not power of 2.\n");
        }
        logInDim = (unsigned int)logInDimfloat;
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
    theoProb = inProb[0];
    for (int i = 1; i < inDim; ++i)
    {
        theoProb += inProb[i];
    }
    theoProb = theoProb/(float)inDim; // '+' for summation, '*' for multiplication?
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

void ADD::Report()
{
    printf("Current ADD:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Random Num Bit Length:  %u\n", bitLength);
    printf("Theoretical Probability:%f\n", theoProb);
}

void ADD::Calc()
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
        outSeq[i] = inSeq[randNum[i] >> (bitLength - logInDim)][i];
        oneCount += outSeq[i];
        if (i < accuracyLength)
        {
            realProb[i] = (float)oneCount/(float)(i+1);
        }
        else
        {
            realProb[i] = (realProb[i-1]*(float)accuracyLength+outSeq[i]-outSeq[i-accuracyLength])/(float)accuracyLength;
        }
        errRate[i] = (theoProb - realProb[i])/theoProb;
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

vector<unsigned int> ADD::OutSeq()
{
    return outSeq;
}

void ADD::OutPrint()
{
    printf("Calling OutPrint for ADD instance: ");
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

vector<float>  ADD::InCC()
{
    return inCC;
}

vector<float> ADD::InProb()
{
    return inProb;
}

float ADD::TheoProb()
{
    return theoProb;
}

vector<float> ADD::RealProb()
{
    return realProb;
}

float ADD::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> ADD::ErrRate()
{
    return errRate;
}

float ADD::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int ADD::SeqLen()
{
    return seqLength;
}

unsigned int ADD::LowErrLen()
{
    return lowErrLen;
}

unsigned int ADD::PPStage()
{
    return ppStage;
}
