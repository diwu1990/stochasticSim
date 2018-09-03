#include "expscaled.hpp"
#include "seqprob.hpp"
#include "autocorrelation.hpp"
#include "add.hpp"
#include "mul.hpp"

EXPSCALED::EXPSCALED(){}

EXPSCALED::~EXPSCALED(){}

void EXPSCALED::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling EXPSCALED Help. Following are instructions to EXPSCALED Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the EXPSCALED inst.\n");
    printf("Initial Parameters: Input Vectors and Selection Signal.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the scaled exponent of input.\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the calculated result.\n");

    printf("4. inst.OutPrint() method:\n");
    printf("Print the information of result.\n");

    printf("5. inst.InAC() method:\n");
    printf("Return the input autocorrelation.\n");

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
    printf("Report the status of current instance.\n");

    printf("15. inst.PPStage() method:\n");
    printf("Report the required pipeline stage for hardware.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void EXPSCALED::Init(vector<unsigned int> param1, vector<vector<unsigned int>> param2, unsigned int param3, string param4)
{
    inSeq = param1;
    inSel = param2;
    SeqProb probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    bitLength = param3;
    m_name = param4;

    seqLength = (unsigned int)inSeq.size();
    if ((unsigned int)inSeq.size() != (unsigned int)inSel[0].size())
    {
        printf("Error: Input Seqsence Length is not the same with the Sel Length.\n");
    }

    // theoProb = exp(inProb);
    theoProb = exp(8*(inProb-0.5))/exp(4);
    if (theoProb == 0)
    {
        theoProb = 0.0001;
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

void EXPSCALED::Report()
{
    printf("Current EXPSCALED:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Random Num Bit Length:  %u\n", bitLength);
    printf("Input Probability:      %f\n", inProb);
    //printf("Input sel:      %f\n", inProb[2]); // input probability of selection signal??**
    printf("Theoretical Probability:%f\n", theoProb);
}

void EXPSCALED::Calc()
{
    AutoCorrelation inputAC;
    inputAC.Init(inSeq,1,inProb,"inputAC"); 
    inputAC.Calc();
    inAC = inputAC.OutAC();

    float oneCount = 0;

    // for scaled exp
    // just in^6
    vector<vector<unsigned int>> d(3);
    vector<vector<unsigned int>> mulOut(3);
    for (int i = 0; i < 3; ++i)
    {
        d[i].resize(seqLength);
        mulOut[i].resize(seqLength);
    }

    for (int i = 0; i < seqLength; ++i)
    {
        d[0][i] = inSeq[(i+seqLength-1)%seqLength];
        d[1][i] = inSeq[(i+seqLength-2)%seqLength];
    }

    // mul 0
    vector<vector<unsigned int>> mulIn0(2);
    for (int i = 0; i < 2; ++i)
    {
        mulIn0[i].resize(seqLength);
    }
    mulIn0[0] = inSeq;
    mulIn0[1] = d[0];
    MUL mulInst0;
    mulInst0.Init(mulIn0, "mulInst0");
    mulInst0.Calc();

    // mul 1
    vector<vector<unsigned int>> mulIn1(2);
    for (int i = 0; i < 2; ++i)
    {
        mulIn1[i].resize(seqLength);
    }
    mulIn1[0] = mulInst0.OutSeq();
    mulIn1[1] = d[1];
    MUL mulInst1;
    mulInst1.Init(mulIn1, "mulInst1");
    mulInst1.Calc();

    // mul 2
    for (int i = 0; i < seqLength; ++i)
    {
        d[2][i] = mulInst1.OutSeq()[(i+seqLength-1)%seqLength];
    }
    vector<vector<unsigned int>> mulIn2(2);
    for (int i = 0; i < 2; ++i)
    {
        mulIn2[i].resize(seqLength);
    }
    mulIn2[0] = mulInst1.OutSeq();
    mulIn2[1] = d[2];
    MUL mulInst2;
    mulInst2.Init(mulIn2, "mulInst2");
    mulInst2.Calc();

    outSeq = mulInst2.OutSeq();

    unsigned int accuracyLength = 128;
    for (int i = 0; i < seqLength; ++i)
    {
        oneCount += outSeq[i];
        // printf("%f\n", oneCount);
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

vector<unsigned int> EXPSCALED::OutSeq()
{
    return outSeq;
}

void EXPSCALED::OutPrint()
{
    printf("Calling OutPrint for EXPSCALED instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: expscaled(%.3f) = %.3f with input autocorrelation %.3f\n", inProb, theoProb, inAC);
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

float  EXPSCALED::InAC()
{
    return inAC;
}

float EXPSCALED::InProb()
{
    return inProb;
}

float EXPSCALED::TheoProb()
{
    return theoProb;
}

vector<float> EXPSCALED::RealProb()
{
    return realProb;
}

float EXPSCALED::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> EXPSCALED::ErrRate()
{
    return errRate;
}

float EXPSCALED::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int EXPSCALED::SeqLen()
{
    return seqLength;
}

unsigned int EXPSCALED::LowErrLen()
{
    return lowErrLen;
}

unsigned int EXPSCALED::PPStage()
{
    return ppStage;
}
