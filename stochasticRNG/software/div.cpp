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
    printf("Initial Parameters: Two Input Vectors, Random Number Seqsence, Bit Length of Random Number, Tracing Memory Bit Length, Instance Name.\n");
    printf("Recommended Tracing Memory Bit Length: 2\n");

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
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void DIV::Init(vector<vector<unsigned int>> param1, vector<unsigned int> param2, unsigned int param3, unsigned int param4, string param5)
{
    inSeq = param1;
    SeqProbMulti probCalc;
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
    printf("Bit Length of Tracer:   %u\n", depth);
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f,%f\n", inProb[0], inProb[1]);
    printf("Theoretical Probability:%f\n", theoProb);
}

// void DIV::CalcQuot()
void DIV::Calc()
{
    

    // *****************************************************************************
    // counter based for correlation
    // *****************************************************************************
    // CrossCorrelation inputCC;
    // inputCC.Init(inSeq,1,"inputCC");
    // inputCC.Calc();
    // inCC = inputCC.OutCC()[0];

    // Synchronizer divSyncInst;
    // divSyncInst.Init(inSeq, 2, "divSyncInst");
    // divSyncInst.SeqGen();
    // // divSyncInst.ProbPrint();
    // // divSyncInst.SeqPrint();
    // unsigned int upperBound = (unsigned int)pow(2,depth)-1;
    // unsigned int halfBound = (unsigned int)pow(2,depth-1);
    // unsigned int traceReg = halfBound;
    // unsigned int oneCount = 0;

    // unsigned int effectiveBit = 0;
    // unsigned int effectiveOne = 0;
    // unsigned int reservedBit = 0;
    // unsigned int reservedOne = 0;

    // for (int i = 0; i < seqLength; ++i)
    // {
    //     // printf("%d iter\n", i);
    //     if (divSyncInst.OutSeq()[1][i] == 1)
    //     {
    //         // printf("effective\n");
    //         effectiveBit++;
    //         outSeq[i] = divSyncInst.OutSeq()[0][i];
    //         if (outSeq[i] == 0)
    //         {
    //             // if (traceReg > 0 && effectiveBit < 16)
    //             if (traceReg > 0)
    //             {
    //                 traceReg -= 1;
    //                 // printf("%d => %5u, %-.3f\n", i, traceReg, (float)traceReg/pow(2,depth));
    //             }
    //         }
    //         else
    //         {
    //             effectiveOne++;
    //             // if (traceReg < upperBound && effectiveBit < 16)
    //             if (traceReg < upperBound)
    //             {
    //                 traceReg += 1;
    //                 // printf("%d => %5u, %-.3f\n", i, traceReg, (float)traceReg/pow(2,depth));
    //             }
    //         }
    //         // printf("%u\n", outSeq[i]);
    //         oneCount += outSeq[i];
    //         if (i < 32)
            // {
            //     realProb[i] = (float)oneCount/(float)(i+1);
            // }
            // else
            // {
            //     realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
            // }
    //         errRate[i] = (theoProb - realProb[i])/theoProb;
    //     }
    //     else
    //     {
    //         // printf("reserved, %u\n", randNum[i]);
    //         // printf("reserved, %u\n", (randNum[i] >> (bitLength-depth)));
    //         reservedBit++;
    //         if (traceReg <= (randNum[i] >> (bitLength-depth)))
    //         {
    //             outSeq[i] = 0;
    //             // printf("%d => %5u, %5u, %5u\n", i, traceReg, (randNum[i] >> (bitLength-depth)), randNum[i]);
    //         }
    //         else
    //         {
    //             outSeq[i] = 1;
    //             // printf("%d => %5u, %5u, %5u\n", i, traceReg, (randNum[i] >> (bitLength-depth)), randNum[i]);
    //             reservedOne++;
    //         }
    //         oneCount += outSeq[i];
    //         if (i < 32)
            // {
            //     realProb[i] = (float)oneCount/(float)(i+1);
            // }
            // else
            // {
            //     realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
            // }
    //         errRate[i] = (theoProb - realProb[i])/theoProb;
    //     }
    //     // printf("%d iter: %u => %u\n\n", i, divSyncInst.OutSeq()[1][i], outSeq[i]);
    // }

    // *****************************************************************************
    // shift reg based for correlation
    // *****************************************************************************
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.Calc();
    inCC = inputCC.OutCC()[0];

    Synchronizer divSyncInst;
    divSyncInst.Init(inSeq, 2, "divSyncInst");
    divSyncInst.SeqGen();
    // divSyncInst.ProbPrint();
    // divSyncInst.SeqPrint();

    // unsigned int upperBound = (unsigned int)pow(2,depth)-1;
    // unsigned int halfBound = (unsigned int)pow(2,depth-1);
    vector<unsigned int> traceReg(depth);
    for (int i = 0; i < depth; ++i)
    {
        traceReg[i] = i%2;
        // printf("%u, ", traceReg[i]);
    }
    // printf("\n");
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
            // printf("%d: effective\n", i);
            effectiveBit++;
            outSeq[i] = divSyncInst.OutSeq()[0][i];
            for (int index = 0; index < depth-1; ++index)
            {
                traceReg[index] = traceReg[index+1];
                // printf("%u, ", traceReg[index]);
            }
            traceReg[depth-1] = outSeq[i];
            // printf("%u\n", traceReg[depth-1]);
            effectiveOne += outSeq[i];
            // printf("%u\n", outSeq[i]);
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
        }
        else if (divSyncInst.OutSeq()[1][i] == 0)
        {
            // printf("%d: reserved, %5u\n", i, (randNum[i] >> (bitLength-logDepth)));
            // printf("reserved, %u\n", );
            reservedBit++;
            outSeq[i] = traceReg[(randNum[i] >> (bitLength - logDepth))];
            oneCount += outSeq[i];
            reservedOne += outSeq[i];
            if (i < 32)
            {
                realProb[i] = (float)oneCount/(float)(i+1);
            }
            else
            {
                realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
            }
            errRate[i] = (theoProb - realProb[i])/theoProb;
            // printf("reserved, %5u, %5u, %5u, %5u, %5u\n", randNum[i],traceReg,outSeq[i],oneCount,reservedOne);
        }
        else
        {
            printf("Error: Seqsence from Class Synchronizer is not bit stream.\n");
        }
        // printf("%d iter: %u => %u\n\n", i, divSyncInst.OutSeq()[1][i], outSeq[i]);
    }

    // // *****************************************************************************
    // // counter based no correlation
    // // *****************************************************************************
    // unsigned int upperBound = (unsigned int)pow(2,depth)-1;
    // unsigned int halfBound = (unsigned int)pow(2,depth-1);
    // unsigned int traceReg = halfBound;
    // unsigned int oneCount = 0;

    // unsigned int effectiveBit = 0;
    // unsigned int effectiveOne = 0;
    // unsigned int reservedBit = 0;
    // unsigned int reservedOne = 0;

    // for (int i = 0; i < seqLength; ++i)
    // {
    //     // printf("%d iter\n", i);
    //     if (traceReg >= (randNum[i] >> (bitLength-depth)))
    //     {
    //         // printf("%d: effective\n", i);
    //         outSeq[i] = 1;
    //         oneCount += outSeq[i];
    //         if (i < 32)
    //         {
    //             realProb[i] = (float)oneCount/(float)(i+1);
    //         }
    //         else
    //         {
    //             realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
    //         }
    //         errRate[i] = (theoProb - realProb[i])/theoProb;
    //         printf("%f, %u, %f, %f\n", errRate[i], outSeq[i], realProb[i], theoProb);
    //     }
    //     else
    //     {
    //         outSeq[i] = 0;
    //         oneCount += outSeq[i];
    //         if (i < 32)
    //         {
    //             realProb[i] = (float)oneCount/(float)(i+1);
    //         }
    //         else
    //         {
    //             realProb[i] = (realProb[i-1]*32+outSeq[i]-outSeq[i-32])/32;
    //         }
    //         errRate[i] = (theoProb - realProb[i])/theoProb;
    //         printf("%f, %u, %f, %f\n", errRate[i], outSeq[i], realProb[i], theoProb);
    //     }
    //     unsigned int andGate = outSeq[i] & inSeq[1][i];
    //     unsigned int inc = !andGate & inSeq[0][i];
    //     unsigned int dec = andGate & !inSeq[0][i];
    //     // printf("%u, %u, %u, %u, %u\n", andGate, inSeq[0][i], inSeq[1][i], inc, dec);
    //     if (inc == 1 && dec == 0)
    //     {
    //         if (traceReg < upperBound)
    //         {
    //             traceReg = traceReg + 1;
    //         }
    //     }
    //     else if (inc == 0 && dec == 1)
    //     {
    //         if (traceReg > 0)
    //         {
    //             traceReg = traceReg - 1;
    //         }
    //     }
    //     // printf("%u\n", traceReg);
    // }

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
    // printf("CalcQuot Done\n");
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
