#include "relu.hpp"
#include "seqprob.hpp"
#include "autocorrelation.hpp"

RELU::RELU(){}

RELU::~RELU(){}

void RELU::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling RELU Help. Following are instructions to RELU Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the RELU inst.\n");
    printf("Initial Parameters: Input Vectors, Instance Name.\n");
    printf("Suggested Depth: 3.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the reluonent of input.\n");

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

// void RELU::Init(vector<char> param1, vector<unsigned int> param2, unsigned int param3, string param4)
// {
//     inSeq = param1;
//     randSeq = param2;
//     SeqProb probCalc;
//     probCalc.Init(inSeq,"probCalc");
//     probCalc.Calc();
//     inProb = probCalc.OutProb();
//     depth = param3;
//     m_name = param4;

void RELU::Init(vector<char> param1, unsigned int param2, string param3)
{
    inSeq = param1;
    SeqProb probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    depth = param2;
    m_name = param3;


    seqLength = (unsigned int)inSeq.size();
    theoProb = max(0.5,inProb);
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

void RELU::Report()
{
    printf("Current RELU:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f\n", inProb);
    printf("Theoretical Probability:%f\n", theoProb);
}

void RELU::Calc()
{
    AutoCorrelation inputAC;
    inputAC.Init(inSeq,1,inProb,"inputAC");
    inputAC.Calc();
    inAC = inputAC.OutAC();

    unsigned int upperBound = (unsigned int)pow(2,depth)-1;
    unsigned int halfBound = (unsigned int)pow(2,depth-1);
    unsigned int satCnt = halfBound;

    vector<char> dff(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        dff[i] = i%2;
    }
    for (int i = 0; i < seqLength; ++i)
    {
        if (satCnt >= halfBound)
        {
            outSeq[i] = inSeq[i];
        }
        else
        {
            outSeq[i] = dff[i];
        }
        if (inSeq[i] == 1)
        {
            if (satCnt < upperBound)
            {
                satCnt += 1;
            }
        }
        else
        {
            if (satCnt > 0)
            {
                satCnt -= 1;
            }
        }
    }

    float oneCount = 0;
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

vector<char> RELU::OutSeq()
{
    return outSeq;
}

void RELU::OutPrint()
{
    printf("Calling OutPrint for RELU instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: relu(%.3f) = %.3f with input autocorrelation %.3f\n", inProb, theoProb, inAC);
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

float  RELU::InAC()
{
    return inAC;
}

float RELU::InProb()
{
    return inProb;
}

float RELU::TheoProb()
{
    return theoProb;
}

vector<float> RELU::RealProb()
{
    return realProb;
}

float RELU::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> RELU::ErrRate()
{
    return errRate;
}

float RELU::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int RELU::SeqLen()
{
    return seqLength;
}

unsigned int RELU::LowErrLen()
{
    return lowErrLen;
}

unsigned int RELU::PPStage()
{
    return ppStage;
}
