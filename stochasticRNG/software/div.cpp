#include "div.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"
#include "synchronizer.hpp"

DIV::DIV(){}
DIV::~DIV(){}
void DIV::Help()
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
    printf("Calling DIV Help. Following are instructions to DIV Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the DIV inst.\n");
    printf("Initial Parameters: Two Input Vectors.\n");

    printf("2. inst.CalcQuot() method:\n");
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

    printf("13. inst.PPStage() method:\n");
    printf("Return the pipline stages required by hardware.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void DIV::Init(vector<vector<unsigned int>> param1, vector<unsigned int> param2, unsigned int param3, unsigned int param4, string param5)
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.CalcProb();
    inProb = probCalc.OutProb();
    randNum = param2;
    bitLength = param3;
    depth = param4;
    m_name = param5;
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
    theoProb = inProb[0] / inProb[1];
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
    ppStage = 2;
}

void DIV::Report()
{
    printf("Current DIV:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Bit Length of RandNum:  %u\n", bitLength);
    printf("Bit Length of tracer:   %u\n", depth);
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f,%f\n", inProb[0], inProb[1]);
    printf("Theoretical Probability:%f\n", theoProb);
}

void DIV::CalcQuot()
{
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.CalcCC();
    inCC = inputCC.OutCC()[0];

    Synchronizer divSyncInst;
    divSyncInst.Init(inSeq, 2, "divSyncInst");
    divSyncInst.SeqGen();
    // divSyncInst.ErrPrint();
    // divSyncInst.SeqPrint();

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
        if (divSyncInst.OutSeq()[1][i] == 1)
        {
            // printf("effective\n");
            effectiveBit++;
            outSeq[i] = divSyncInst.OutSeq()[0][i];
            if (outSeq[i] == 0)
            {
                if (traceReg > 0 && effectiveBit < 16)
                {
                    traceReg -= 1;
                    printf("%d => %5u, %-.3f\n", i, traceReg, (float)traceReg/pow(2,depth));
                }
            }
            else
            {
                effectiveOne++;
                if (traceReg < upperBound && effectiveBit < 16)
                {
                    traceReg += 1;
                    printf("%d => %5u, %-.3f\n", i, traceReg, (float)traceReg/pow(2,depth));
                }
            }
            // printf("%u\n", outSeq[i]);
            oneCount += outSeq[i];
            realProb[i] = (float)oneCount/(float)(i+1);
            errRate[i] = (theoProb - realProb[i])/theoProb;
        }
        else
        {
            // printf("reserved, %u\n", randNum[i]);
            // printf("reserved, %u\n", (randNum[i] >> (bitLength-depth)));
            reservedBit++;
            if (traceReg <= (randNum[i] >> (bitLength-depth)))
            {
                outSeq[i] = 0;
                printf("%d => %5u, %5u\n", i, traceReg, (randNum[i] >> (bitLength-depth)));
            }
            else
            {
                outSeq[i] = 1;
                printf("%d => %5u, %5u\n", i, traceReg, (randNum[i] >> (bitLength-depth)));
                reservedOne++;
            }
            oneCount += outSeq[i];
            realProb[i] = (float)oneCount/(float)(i+1);
            errRate[i] = (theoProb - realProb[i])/theoProb;
        }
        // printf("%d iter: %u => %u\n\n", i, divSyncInst.OutSeq()[1][i], outSeq[i]);
    }
    printf("theoretical prob: %-.3f\n", theoProb);
    printf("effective prob:   %-.3f, One: %5u, Total Bit: %5u\n", (float)effectiveOne/(float)effectiveBit, effectiveOne, effectiveBit);
    printf("reserved prob :   %-.3f, One: %5u, Total Bit: %5u\n", (float)reservedOne/(float)reservedBit, reservedOne, reservedBit);
    
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

vector<unsigned int> DIV::OutSeq()
{
    return outSeq;
}

unsigned int DIV::PPStage()
{
    return ppStage;
}

void DIV::OutPrint()
{
    printf("Calling OutPrint for DIV instance: ");
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

float DIV::InCC()
{
    return inCC;
}

vector<float> DIV::InProb()
{
    return inProb;
}

float DIV::TheoProb()
{
    return theoProb;
}

vector<float> DIV::RealProb()
{
    return realProb;
}

float DIV::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> DIV::ErrRate()
{
    return errRate;
}

float DIV::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int DIV::SeqLen()
{
    return seqLength;
}

unsigned int DIV::LowErrLen()
{
    return lowErrLen;
}
