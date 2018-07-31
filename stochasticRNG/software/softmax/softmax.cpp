#include "add.hpp"
#include "apcadd.hpp"
#include "exp.hpp"
#include "div.hpp"
#include "softmax.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

SOFTMAX::SOFTMAX(){}

SOFTMAX::~SOFTMAX(){}

void SOFTMAX::Init(vector<vector<unsigned int>> param1, vector<unsigned int> param2, unsigned int param3, string param4)
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randNum = param2;
    bitLength = param3;
    m_name = param4;
    if ((unsigned int)inSeq.size() == (unsigned int)inProb.size() && (unsigned int)inSeq.size() == 16)
    {
        inDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Dimension is not 16.\n");
    }

    seqLength = (unsigned int)inSeq[0].size();
    for (int i = 0; i < inDim; ++i)
    {
        if ((unsigned int)inSeq[i].size() != seqLength)
        {
            printf("Error: Input Length is not the same.\n");
            break;
        }
    }

    theoProb = 0;
    for (int i = 0; i < inDim; ++i)
    {
        theoProb += inProb[i];
    }
    theoProb /= 16;
    if (theoProb == 0)
    {
        theoProb == 0.0001;
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
    lowErrLen = seqLength;
    ppStage = 0;
}

void SOFTMAX::Calc()
{
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.Calc();
    inCC = inputCC.OutCC();
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

vector<unsigned int> SOFTMAX::OutSeq()
{
    return outSeq;
}

unsigned int SOFTMAX::PPStage()
{
    return ppStage;
}

vector<float> SOFTMAX::InCC()
{
    return inCC;
}

vector<float> SOFTMAX::InProb()
{
    return inProb;
}

float SOFTMAX::TheoProb()
{
    return theoProb;
}

vector<float> SOFTMAX::RealProb()
{
    return realProb;
}

float SOFTMAX::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> SOFTMAX::ErrRate()
{
    return errRate;
}

float SOFTMAX::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int SOFTMAX::LowErrLen()
{
    return lowErrLen;
}


