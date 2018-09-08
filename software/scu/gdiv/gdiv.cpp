#include "gdiv.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"
#include "skewedsynchronizer.hpp"

GDIV::GDIV(){}
GDIV::~GDIV(){}
void GDIV::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling GDIV Help. Following are instructions to GDIV Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the GDIV inst.\n");
    printf("Initial Parameters: Two Input Vectors, Random Number Seqsence, Depth of Trace Register, Depth of Synchronizer, Instance Name.\n");
    printf("Recommended Depth of Trace Register: 4\n");
    printf("Depth of Synchronizer is not used for GDIV.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the quotient of two input sequences.\n");

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

    printf("15. inst.Report() method:\n");
    printf("Report the current instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void GDIV::Init(vector<vector<unsigned int>> param1, vector<unsigned int> param2, unsigned int param3, unsigned int param4, string param5)
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randNum = param2;
    depth = param3;
    depthSync = param4;
    m_name = param5;

    if ((unsigned int)inSeq.size() == (unsigned int)inProb.size() && (unsigned int)inSeq.size() == 2)
    {
        inDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Dimension is not 2.\n");
    }

    if ((unsigned int)inSeq[0].size() == (unsigned int)inSeq[1].size() && (unsigned int)inSeq[0].size() == (unsigned int)randNum.size())
    {
        seqLength = (unsigned int)inSeq[0].size();
    }
    else
    {
        printf("Error: Input Length is not the same.\n");
    }
    if (inProb[1] == 0)
    {
        theoProb = 0;
    }
    else
    {
        theoProb = inProb[0] / inProb[1];
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
    // for (int i = 0; i < inDim; ++i)
    // {
    //     for (int j = 0; j < seqLength; ++j)
    //     {
    //         printf("%u,", inSeq[i][j]);
    //     }
    //     printf("\n");
    // }
    ppStage = 0;
    for (int i = 0; i < seqLength; ++i)
    {
        if (randNum[i] > depth-1)
        {
            printf("Error: Range of random number is larger than the depth of trace register.\n");
        }
    }
}

void GDIV::Report()
{
    printf("Current GDIV:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Bit Length of Tracer:   %u\n", depth);
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f,%f\n", inProb[0], inProb[1]);
    printf("Theoretical Probability:%f\n", theoProb);
}

// void GDIV::CalcQuot()
void GDIV::Calc()
{

    // *****************************************************************************
    // counter based no correlation
    // *****************************************************************************
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.Calc();
    inCC = inputCC.OutCC()[0];
    unsigned int upperBound = (unsigned int)pow(2,depth)-1;
    unsigned int halfBound = (unsigned int)pow(2,depth-1);
    unsigned int traceReg = halfBound;
    unsigned int oneCount = 0;
    unsigned int accuracyLength = seqLength/2;

    unsigned int effectiveBit = 0;
    unsigned int effectiveOne = 0;
    unsigned int reservedBit = 0;
    unsigned int reservedOne = 0;

    for (int i = 0; i < seqLength; ++i)
    {
        if (traceReg >= randNum[i])
        {
            outSeq[i] = 1;
        }
        else
        {
            outSeq[i] = 0;
        }
        oneCount += outSeq[i];
        if (i < accuracyLength)
        {
            realProb[i] = (float)oneCount/(float)(i+1);
        }
        else
        {
            realProb[i] = (realProb[i-1]*accuracyLength+outSeq[i]-outSeq[i-accuracyLength])/accuracyLength;
        }
        errRate[i] = (theoProb - realProb[i]);
        // unsigned int andGate = outSeq[i] & inSeq[1][(i+seqLength-1)%seqLength];
        unsigned int andGate = outSeq[i] & inSeq[1][i];
        unsigned int inc = !andGate & inSeq[0][i];
        unsigned int dec = andGate & !inSeq[0][i];
        // printf("%u, %u, %u, %u, %u\n", andGate, inSeq[0][i], inSeq[1][i], inc, dec);
        if (inc == 1 && dec == 0)
        {
            if (traceReg < upperBound)
            {
                traceReg = traceReg + 1;
            }
        }
        else if (inc == 0 && dec == 1)
        {
            if (traceReg > 0)
            {
                traceReg = traceReg - 1;
            }
        }
        // printf("%u\n", traceReg);
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
    // printf("%u\n", lowErrLen);
    // printf("CalcQuot Done\n");
}

vector<unsigned int> GDIV::OutSeq()
{
    return outSeq;
}

unsigned int GDIV::PPStage()
{
    return ppStage;
}

void GDIV::OutPrint()
{
    printf("Calling OutPrint for GDIV instance: ");
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

float GDIV::InCC()
{
    return inCC;
}

vector<float> GDIV::InProb()
{
    return inProb;
}

float GDIV::TheoProb()
{
    return theoProb;
}

vector<float> GDIV::RealProb()
{
    return realProb;
}

float GDIV::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> GDIV::ErrRate()
{
    return errRate;
}

float GDIV::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int GDIV::SeqLen()
{
    return seqLength;
}

unsigned int GDIV::LowErrLen()
{
    return lowErrLen;
}
