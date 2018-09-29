#include "cordiv.hpp"
#include "systemrand.hpp"
#include "sobol.hpp"
#include "randNum2BitMulti.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"
#include "skewedsynchronizer.hpp"

void CORDIV::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling CORDIV Help. Following are instructions to CORDIV Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the CORDIV inst.\n");
    printf("Initial Parameters: Two Input Vectors, Random Number Seqsence, Depth of Trace Register, Depth of Synchronizer, Instance Name.\n");
    printf("Recommended Depth of Trace Register Length: 2\n");
    printf("Recommended Depth of Synchronizer: 2~4\n");

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

void CORDIV::Init(vector<vector<char>> param1, vector<unsigned int> param2, unsigned int param3, unsigned int param4, string param5)
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

void CORDIV::Report()
{
    printf("Current CORDIV:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Bit Length of Tracer:   %u\n", depth);
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f,%f\n", inProb[0], inProb[1]);
    printf("Theoretical Probability:%f\n", theoProb);
}

// void CORDIV::CalcQuot()
void CORDIV::Calc()
{

    // *****************************************************************************
    // shift reg based for correlation
    // *****************************************************************************
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.Calc();
    inCC = inputCC.OutCC()[0];

    // printf("%f\n", inProb[0]);
    // printf("%f\n", inProb[1]);
    // printf("%f\n", theoProb);
    // data regeneration via rng
    SOBOL rngInst;
    rngInst.Init(depthSync,5,0,"rngInst");
    rngInst.SeqGen();

    unsigned int upperBound = (unsigned int)pow(2,depthSync)-1;
    unsigned int halfBound = (unsigned int)pow(2,depthSync-1);
    vector<vector<unsigned int>> cnt(2);
    for (int i = 0; i < 2; ++i)
    {
        cnt[i].resize(seqLength);
        for (int j = 0; j < seqLength; ++j)
        {
            cnt[i][j] = halfBound;
        }
    }

    vector<char> traceReg(depth);
    for (int i = 0; i < depth; ++i)
    {
        traceReg[i] = i%2;
        // printf("%u, ", traceReg[i]);
    }
    // printf("\n");
    unsigned int oneCount = 0;
    unsigned int accuracyLength = seqLength/2;

    vector<char> divisorBit(seqLength);
    vector<char> dividendBit(seqLength);

    for (int i = 0; i < seqLength; ++i)
    {
        if (cnt[0][i] >= upperBound)
        {
            cnt[0][i] = inSeq[0][i] ? cnt[0][(i-1+seqLength)%seqLength] : (cnt[0][(i-1+seqLength)%seqLength]-1);
        }
        else if (cnt[0][i] <= 0)
        {
            cnt[0][i] = inSeq[0][i] ? (cnt[0][(i-1+seqLength)%seqLength]+1) : (cnt[0][(i-1+seqLength)%seqLength]);
        }
        else
        {
            cnt[0][i] = inSeq[0][i] ? (cnt[0][(i-1+seqLength)%seqLength]+1) : (cnt[0][(i-1+seqLength)%seqLength]-1);
        }

        if (cnt[1][i] >= upperBound)
        {
            cnt[1][i] = inSeq[1][i] ? cnt[1][(i-1+seqLength)%seqLength] : (cnt[1][(i-1+seqLength)%seqLength]-1);
        }
        else if (cnt[1][i] <= 0)
        {
            cnt[1][i] = inSeq[1][i] ? (cnt[1][(i-1+seqLength)%seqLength]+1) : (cnt[1][(i-1+seqLength)%seqLength]);
        }
        else
        {
            cnt[1][i] = inSeq[1][i] ? (cnt[1][(i-1+seqLength)%seqLength]+1) : (cnt[1][(i-1+seqLength)%seqLength]-1);
        }

        // printf("%u,%u\n", cnt[0][i],cnt[1][i]);
        divisorBit[i] = cnt[1][i] >= rngInst.OutSeq()[i];
        dividendBit[i] = cnt[0][i] >= rngInst.OutSeq()[i];
        // printf("%d iter\n", i);
        if (divisorBit[i] == 1)
        {
            // printf("%d: effective\n", i);
            // effectiveBit++;
            outSeq[i] = dividendBit[i];
            for (int index = 0; index < depth-1; ++index)
            {
                traceReg[index] = traceReg[index+1];
                // printf("%u, ", traceReg[index]);
            }
            traceReg[depth-1] = outSeq[i];
            // printf("%u\n", traceReg[depth-1]);
            // effectiveOne += outSeq[i];
            // printf("%u\n", outSeq[i]);
            
        }
        else if (divisorBit[i] == 0)
        {
            // printf("%d: reserved, %5u\n", i, (randNum[i] >> (bitLength-logDepth)));
            // printf("reserved, %u\n", );
            // reservedBit++;
            outSeq[i] = traceReg[randNum[i]];
            // reservedOne += outSeq[i];
            // printf("reserved, %5u, %5u, %5u, %5u, %5u\n", randNum[i],traceReg,outSeq[i],oneCount,reservedOne);
        }
        else
        {
            printf("Error: Seqsence from Class Synchronizer is not bit stream.\n");
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
        // printf("%d iter: %u => %u\n\n", i, divSyncInst.OutSeq()[1][i], outSeq[i]);
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

vector<char> CORDIV::OutSeq()
{
    return outSeq;
}

unsigned int CORDIV::PPStage()
{
    return ppStage;
}

void CORDIV::OutPrint()
{
    printf("Calling OutPrint for CORDIV instance: ");
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

float CORDIV::InCC()
{
    return inCC;
}

vector<float> CORDIV::InProb()
{
    return inProb;
}

float CORDIV::TheoProb()
{
    return theoProb;
}

vector<float> CORDIV::RealProb()
{
    return realProb;
}

float CORDIV::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> CORDIV::ErrRate()
{
    return errRate;
}

float CORDIV::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int CORDIV::SeqLen()
{
    return seqLength;
}

unsigned int CORDIV::LowErrLen()
{
    return lowErrLen;
}
