#include "gsqrt.hpp"
#include "seqprob.hpp"
#include "autocorrelation.hpp"
#include "crosscorrelation.hpp"

GSQRT::GSQRT(){}
GSQRT::~GSQRT(){}
void GSQRT::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling GSQRT Help. Following are instructions to GSQRT Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the GSQRT inst.\n");
    printf("Initial Parameters: Input Vector, Random Number Seqsence, Trace Register Depth, Instance Name.\n");
    printf("Recommended Trace Register Depth: 4\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the quotient of two input sequences.\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the calculated result.\n");

    printf("4. inst.OutPrint() method:\n");
    printf("Print the information of result.\n");

    printf("5. inst.InAC() method:\n");
    printf("Return the input auto-correlation.\n");

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

void GSQRT::Init(vector<char> param1, vector<unsigned int> param2, unsigned int param3, string param4)
{
    inSeq = param1;
    SeqProb probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randNum = param2;
    depth = param3;
    m_name = param4;

    seqLength = (unsigned int)inSeq.size();
    theoProb = sqrt(inProb);
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

    for (int i = 0; i < seqLength; ++i)
    {
        if (randNum[i] > depth-1)
        {
            printf("Error: Range of random number is larger than the depth of trace register.\n");
        }
    }
}

void GSQRT::Report()
{
    printf("Current GSQRT:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f\n", inProb);
    printf("Theoretical Probability:%f\n", theoProb);
}

// void GSQRT::CalcQuot()
void GSQRT::Calc()
{
    AutoCorrelation autocorrelationInst;
    autocorrelationInst.Init(inSeq, 1, inProb, "autocorrelationInst");
    autocorrelationInst.Calc();
    inAC = autocorrelationInst.OutAC();
    unsigned int accuracyLength = seqLength/2;

    // *****************************************************************************
    // counter based no correlation
    // *****************************************************************************
    unsigned int upperBound = (unsigned int)pow(2,depth)-1;
    unsigned int halfBound = (unsigned int)pow(2,depth-1);
    unsigned int traceReg = halfBound;
    unsigned int oneCount = 0;

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
        unsigned int andGate = outSeq[i] & outSeq[(i+seqLength-1)%seqLength];
        unsigned int inc = inSeq[i];
        unsigned int dec = andGate;
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
    }

    for (int i = 0; i < seqLength; ++i)
    {
        if (errRate[seqLength-1-i] > 0.05 || errRate[seqLength-1-i] < -0.05)
        {
            lowErrLen = seqLength-i;
            break;
        }
    }
}

vector<char> GSQRT::OutSeq()
{
    return outSeq;
}

unsigned int GSQRT::PPStage()
{
    return ppStage;
}

void GSQRT::OutPrint()
{
    printf("Calling OutPrint for GSQRT instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: sqrt(%.3f) = %.3f with input autocorrelation %.3f\n", inProb, theoProb, inAC);
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

float GSQRT::InAC()
{
    return inAC;
}

float GSQRT::InProb()
{
    return inProb;
}

float GSQRT::TheoProb()
{
    return theoProb;
}

vector<float> GSQRT::RealProb()
{
    return realProb;
}

float GSQRT::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> GSQRT::ErrRate()
{
    return errRate;
}

float GSQRT::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int GSQRT::SeqLen()
{
    return seqLength;
}

unsigned int GSQRT::LowErrLen()
{
    return lowErrLen;
}
