#include "jkff.hpp"
#include "seqprobmulti.hpp"
#include "autocorrelation.hpp"
#include "crosscorrelation.hpp"

JKFF::JKFF(){}
JKFF::~JKFF(){}
void JKFF::Help()
{
    // void Init();
    // void Report();
    // void CalcQuot();
    // void OutPrint();
    // vector<unsigned int> OutSeq();
    // float OutProb();
    // float TheoProb();
    // float ErrRate();
    // unsigned int PPStage();
    // unsigned int LowErrLen();
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling JKFF Help. Following are instructions to JKFF Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the JKFF inst.\n");
    printf("Initial Parameters: Two Input Vectors, Instance Name.\n");
    printf("Vector 0 is for J port; Vector 1 is for K port.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the quotient of two input sequences.\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the calculated result.\n");

    printf("4. inst.OutPrint() method:\n");
    printf("Print the information of result.\n");

    printf("5. inst.InCC() method:\n");
    printf("Return the input crosscorrelation.\n");

    printf("6. inst.InAC() method:\n");
    printf("Return the input autocorrelation.\n");

    printf("7. inst.outAC() method:\n");
    printf("Return the output autocorrelation.\n");

    printf("8. inst.outCC() method:\n");
    printf("Return the output correlation with input.\n");

    printf("9. inst.InProb() method:\n");
    printf("Return the input probability.\n");

    printf("10. inst.TheoProb() method:\n");
    printf("Return the theoretical output probability.\n");

    printf("11. inst.RealProb() method:\n");
    printf("Return the array of progressive output probability.\n");

    printf("12. inst.FinalRealProb() method:\n");
    printf("Return the final output probability.\n");

    printf("13. inst.ErrRate() method:\n");
    printf("Return the array of progressive output error rate.\n");

    printf("14. inst.FinalErrRate() method:\n");
    printf("Return the final output error rate.\n");

    printf("15. inst.SeqLen() method:\n");
    printf("Return the sequence length.\n");

    printf("16. inst.LowErrLen() method:\n");
    printf("Return the sequence length required to converge with less than 5 percent error rate.\n");

    printf("17. inst.PPStage() method:\n");
    printf("Return the pipline stages required by hardware.\n");

    printf("18. inst.Report() method:\n");
    printf("Report the current instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void JKFF::Init(vector<vector<char>> param1, string param2)
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
    if (inProb[0] + inProb[1] == 0)
    {
        theoProb = 0;
    }
    else
    {
        theoProb = inProb[0] / (inProb[0]+inProb[1]);
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
    ppStage = 0;
}

void JKFF::Report()
{
    printf("Current JKFF:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability J:    %f\n", inProb[0]);
    printf("Input Probability K:    %f\n", inProb[1]);
    printf("Theoretical Probability:%f\n", theoProb);
}

// void JKFF::CalcQuot()
void JKFF::Calc()
{
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.Calc();
    inCC = inputCC.OutCC()[0];

    unsigned int oneCount = 0;
    unsigned int accuracyLength = seqLength/2;

    for (int i = 0; i < seqLength; ++i)
    {
        if (inSeq[0][i] == 0 && inSeq[1][i] == 0)
        {
            outSeq[i] = outSeq[(i+seqLength-1)%seqLength];
        }
        else if (inSeq[0][i] == 0 && inSeq[1][i] == 1)
        {
            outSeq[i] = 0;
        }
        else if (inSeq[0][i] == 1 && inSeq[1][i] == 0)
        {
            outSeq[i] = 1;
        }
        else if (inSeq[0][i] == 1 && inSeq[1][i] == 1)
        {
            outSeq[i] = 1-outSeq[(i+seqLength-1)%seqLength];
        }
        else
        {
            printf("Input sequences contain non-logical bits.\n");
        }

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

    // calc inAC
    inAC.resize(2);
    AutoCorrelation inACInst0;
    inACInst0.Init(inSeq[0],1,inProb[0],"inACInst0");
    inACInst0.Calc();
    inAC[0] = inACInst0.OutAC();

    AutoCorrelation inACInst1;
    inACInst1.Init(inSeq[1],1,inProb[1],"inACInst1");
    inACInst1.Calc();
    inAC[1] = inACInst1.OutAC();

    // calc outAC
    AutoCorrelation outACInst;
    outACInst.Init(outSeq,1,theoProb,"outACInst");
    outACInst.Calc();
    outAC = outACInst.OutAC();

    // calc outCC
    outCC.resize(2);
    vector<vector<char>> outCCSeq0(2);
    outCCSeq0[0].resize(seqLength);
    outCCSeq0[1].resize(seqLength);
    outCCSeq0[0] = inSeq[0];
    outCCSeq0[1] = outSeq;
    CrossCorrelation outCCInst0;
    outCCInst0.Init(outCCSeq0,1,"outCCInst0");
    outCCInst0.Calc();
    outCC[0] = outCCInst0.OutCC()[0];

    vector<vector<char>> outCCSeq1(2);
    outCCSeq1[0].resize(seqLength);
    outCCSeq1[1].resize(seqLength);
    outCCSeq1[0] = inSeq[1];
    outCCSeq1[1] = outSeq;
    CrossCorrelation outCCInst1;
    outCCInst1.Init(outCCSeq1,1,"outCCInst1");
    outCCInst1.Calc();
    outCC[1] = outCCInst1.OutCC()[0];
}

vector<char> JKFF::OutSeq()
{
    return outSeq;
}

unsigned int JKFF::PPStage()
{
    return ppStage;
}

void JKFF::OutPrint()
{
    printf("Calling OutPrint for JKFF instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: %.3f / %.3f = %.3f with input crosscorrelation %.3f\n", inProb[0],inProb[1], theoProb, inCC);
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

float JKFF::InCC()
{
    return inCC;
}

vector<float> JKFF::InAC()
{
    return inAC;
}

float JKFF::OutAC()
{
    return outAC;
}

vector<float> JKFF::OutCC()
{
    return outCC;
}

vector<float> JKFF::InProb()
{
    return inProb;
}

float JKFF::TheoProb()
{
    return theoProb;
}

vector<float> JKFF::RealProb()
{
    return realProb;
}

float JKFF::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> JKFF::ErrRate()
{
    return errRate;
}

float JKFF::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int JKFF::SeqLen()
{
    return seqLength;
}

unsigned int JKFF::LowErrLen()
{
    return lowErrLen;
}
