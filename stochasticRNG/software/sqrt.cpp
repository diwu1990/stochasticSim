#include "sqrt.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"
#include "synchronizer.hpp"

SQRT::SQRT(){}
SQRT::~SQRT(){}
void SQRT::Help()
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
    printf("Calling SQRT Help. Following are instructions to SQRT Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SQRT inst.\n");
    printf("Initial Parameters: Input Vector, Random Number Seqsence, Bit Length of Random Number, Tracing Memory Bit Length, Instance Name.\n");
    printf("Recommended Tracing Memory Bit Length: 2\n");

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
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void SQRT::Init(vector<unsigned int> param1, vector<unsigned int> param2, unsigned int param3, unsigned int param4, string param5)
{
    inSeq = param1;
    SeqProb probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randNum = param2;
    bitLength = param3;
    depth = param4;
    if (ceil(log2(depth)) != floor(log2(depth)))
    {
        printf("Error: Input tracer bit length is not pow of 2.\n");
    }
    logDepth = (unsigned int)log2(depth);
    m_name = param5;

    seqLength = (unsigned int)inSeq[0].size();
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
    lowErrLen = seqLength;
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

void SQRT::Report()
{
    printf("Current SQRT:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Bit Length of RandNum:  %u\n", bitLength);
    printf("Bit Length of Tracer:   %u\n", depth);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f\n", inProb);
    printf("Theoretical Probability:%f\n", theoProb);
}

// void SQRT::CalcQuot()
void SQRT::Calc()
{
    

    // *****************************************************************************
    // counter based no correlation
    // *****************************************************************************
    unsigned int upperBound = (unsigned int)pow(2,depth)-1;
    unsigned int halfBound = (unsigned int)pow(2,depth-1);
    unsigned int traceReg = halfBound;
    unsigned int oneCount = 0;

    unsigned int effectiveBit = 0;
    unsigned int effectiveOne = 0;
    unsigned int reservedBit = 0;
    unsigned int reservedOne = 0;

    for (int i = 0; i < seqLength; ++i)
    {
        // printf("%d iter\n", i);
        if (traceReg >= (randNum[i] >> (bitLength-depth)))
        {
            // printf("%d: effective\n", i);
            outSeq[i] = 1;
            oneCount += outSeq[i];
            if (i < 32)
            {
                realProb[i] = (float)oneCount/(float)(i+1);
            }
            else
            {
                realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
            }
            errRate[i] = (theoProb - realProb[i])/theoProb;
            // printf("%f, %u, %f, %f\n", errRate[i], outSeq[i], realProb[i], theoProb);
        }
        else
        {
            outSeq[i] = 0;
            oneCount += outSeq[i];
            if (i < 32)
            {
                realProb[i] = (float)oneCount/(float)(i+1);
            }
            else
            {
                realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
            }
            errRate[i] = (theoProb - realProb[i])/theoProb;
            // printf("%f, %u, %f, %f\n", errRate[i], outSeq[i], realProb[i], theoProb);
        }
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

    // printf("theoretical prob: %-.3f\n", theoProb);
    // printf("effective prob:   %-.3f, One: %5u, Total Bit: %5u\n", (float)effectiveOne/(float)effectiveBit, effectiveOne, effectiveBit);
    // printf("reserved prob :   %-.3f, One: %5u, Total Bit: %5u\n", (float)reservedOne/(float)reservedBit, reservedOne, reservedBit);
    
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

vector<unsigned int> SQRT::OutSeq()
{
    return outSeq;
}

unsigned int SQRT::PPStage()
{
    return ppStage;
}

void SQRT::OutPrint()
{
    printf("Calling OutPrint for SQRT instance: ");
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

float SQRT::InAC()
{
    return inAC;
}

vector<float> SQRT::InProb()
{
    return inProb;
}

float SQRT::TheoProb()
{
    return theoProb;
}

vector<float> SQRT::RealProb()
{
    return realProb;
}

float SQRT::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> SQRT::ErrRate()
{
    return errRate;
}

float SQRT::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int SQRT::SeqLen()
{
    return seqLength;
}

unsigned int SQRT::LowErrLen()
{
    return lowErrLen;
}
