#include "exp.hpp"
#include "seqprob.hpp"
#include "autocorrelation.hpp"
#include "add.hpp"
#include "mul.hpp"

EXP::EXP(){}

EXP::~EXP(){}

void EXP::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling EXP Help. Following are instructions to EXP Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the EXP inst.\n");
    printf("Initial Parameters: Input Vectors and Selection Signal.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the exponent of input.\n");

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

void EXP::Init(vector<unsigned int> param1, vector<vector<unsigned int>> param2, unsigned int param3, string param4)
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

    theoProb = exp(inProb)/4;
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

void EXP::Report()
{
    printf("Current EXP:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Random Num Bit Length:  %u\n", bitLength);
    printf("Input Probability:      %f\n", inProb);
    //printf("Input sel:      %f\n", inProb[2]); // input probability of selection signal??**
    printf("Theoretical Probability:%f\n", theoProb);
}

void EXP::Calc()
{
    AutoCorrelation inputAC;
    inputAC.Init(inSeq,1,inProb,"inputAC"); 
    inputAC.Calc();
    inAC = inputAC.OutAC();

    float oneCount = 0;

    vector<vector<unsigned int>> addConst(3);
    // add[0] is for (0 + x)/2
    // add[1] is for (1 + x/2)/2
    // add[2] is for (1 + x)/2
    // add[3] is for ((1+x)/2 + x^2/2(1+x/2)/2)/2
    addConst[0].resize(seqLength);
    addConst[1].resize(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        addConst[0][i] = 0;
        addConst[1][i] = 1;
    }

    // add 0
    vector<vector<unsigned int>> addIn0(2);
    addIn0[0].resize(seqLength);
    addIn0[0] = inSeq;
    addIn0[1].resize(seqLength);
    addIn0[1] = addConst[0];
    ADD addInst0;
    addInst0.Init(addIn0,inSel[0],bitLength,"addInst0");
    addInst0.Calc();

    // mul 0
    vector<vector<unsigned int>> mulIn0(2);
    vector<unsigned int> d0(seqLength); // dff 0
    for (int i = 0; i < seqLength; ++i)
    {
        d0[i] = inSeq[(i+seqLength-1)%seqLength];
    }
    mulIn0[0].resize(seqLength);
    mulIn0[0] = inSeq;
    mulIn0[1].resize(seqLength);
    mulIn0[1] = d0;
    MUL mulInst0;
    mulInst0.Init(mulIn0,"mulInst0");
    mulInst0.Calc();

    // add 1
    vector<vector<unsigned int>> addIn1(2);
    addIn1[0].resize(seqLength);
    addIn1[0] = addInst0.OutSeq();
    addIn1[1].resize(seqLength);
    addIn1[1] = addConst[1];
    ADD addInst1;
    addInst1.Init(addIn1,inSel[1],bitLength,"addInst1");
    addInst1.Calc();

    // mul 1
    vector<vector<unsigned int>> mulIn1(2);
    vector<unsigned int> d1(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        d1[i] = mulInst0.OutSeq()[(i+seqLength-1)%seqLength];
    }
    mulIn1[0].resize(seqLength);
    mulIn1[0] = addInst1.OutSeq();
    mulIn1[1].resize(seqLength);
    mulIn1[1] = d1;
    MUL mulInst1;
    mulInst1.Init(mulIn1,"mulInst1");
    mulInst1.Calc();

    // add 2
    vector<vector<unsigned int>> addIn2(2);
    addIn2[0].resize(seqLength);
    addIn2[0] = inSeq;
    addIn2[1].resize(seqLength);
    addIn2[1] = addConst[1];
    ADD addInst2;
    addInst2.Init(addIn2,inSel[0],bitLength,"addInst2");
    addInst2.Calc();

    // add 3
    vector<vector<unsigned int>> addIn3(2);
    vector<unsigned int> d2(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        d2[i] = mulInst1.OutSeq()[(i+seqLength-1)%seqLength];
    }
    addIn3[0].resize(seqLength);
    addIn3[0] = addInst2.OutSeq();
    addIn3[1].resize(seqLength);
    addIn3[1] = d2;
    ADD addInst3;
    addInst3.Init(addIn3,inSel[2],bitLength,"addInst3");
    addInst3.Calc();

    unsigned int accuracyLength = 128;
    outSeq = addInst3.OutSeq();
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

vector<unsigned int> EXP::OutSeq()
{
    return outSeq;
}

void EXP::OutPrint()
{
    printf("Calling OutPrint for EXP instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: exp(%.3f) = %.3f with input autocorrelation %.3f\n", inProb, theoProb, inAC);
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

float  EXP::InAC()
{
    return inAC;
}

float EXP::InProb()
{
    return inProb;
}

float EXP::TheoProb()
{
    return theoProb;
}

vector<float> EXP::RealProb()
{
    return realProb;
}

float EXP::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> EXP::ErrRate()
{
    return errRate;
}

float EXP::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int EXP::SeqLen()
{
    return seqLength;
}

unsigned int EXP::LowErrLen()
{
    return lowErrLen;
}

unsigned int EXP::PPStage()
{
    return ppStage;
}
