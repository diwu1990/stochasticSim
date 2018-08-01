#include "add.hpp"
#include "apcadd.hpp"
#include "exp.hpp"
#include "div.hpp"
#include "softmax.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

SOFTMAX::SOFTMAX(){}

SOFTMAX::~SOFTMAX(){}

void SOFTMAX::Init(vector<vector<unsigned int>> param1, vector<vector<unsigned int>> param2, unsigned int param3, string param4)
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
        inSeqDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Sequence Dimension is not 16.\n");
    }

    if ((unsigned int)param2.size() = 4)
    {
        inRandDim = (unsigned int)randNum.size();
    }
    else
    {
        printf("Error: Input Random Number Dimension is not 4.\n");
    }

    seqLength = (unsigned int)inSeq[0].size();
    for (int i = 0; i < inSeqDim; ++i)
    {
        if ((unsigned int)inSeq[i].size() != seqLength)
        {
            printf("Error: Input Sequence Length is not the same.\n");
            break;
        }
    }

    for (int i = 0; i < inRandDim; ++i)
    {
        if ((unsigned int)randNum[i].size() != seqLength)
        {
            printf("Error: Input Random Number Length is not the same.\n");
            break;
        }
    }

    float sum = 0;
    for (int i = 0; i < inSeqDim; ++i)
    {
        sum += exp(inProb[i]);
    }
    for (int i = 0; i < inSeqDim; ++i)
    {
        theoProb[i] = exp(inProb[i])/sum;
    }

    outSeq.resize(inSeqDim);
    realProb.resize(inSeqDim);
    errRate.resize(inSeqDim);
    for (int i = 0; i < inSeqDim; ++i)
    {
        outSeq[i].resize(seqLength);
        realProb[i].resize(seqLength);
        errRate[i].resize(seqLength);
        for (int j = 0; j < seqLength; ++j) 
        {
            outSeq[i][j] = 0;
            realProb[i][j] = 0;
            errRate[i][j] = 0;
        }
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
    vector<float> oneCount(inSeqDim);
    for (int i = 0; i < inSeqDim; ++i)
    {
        oneCount[i] = 0;
    }

    // exp for each input
    vector<unsigned int> expOut(inSeqDim);
    for (int i = 0; i < inSeqDim; ++i)
    {
        EXP expInst;
        expInst.Init(, "expInst");
    }


    unsigned int accuracyLength = 128;
    for (int i = 0; i < inSeqDim; ++i)
    {
        for (int z = 0; z < seqLength; ++z)
        {
            oneCount[i] += outSeq[i][z];
            if (z < accuracyLength)
            {
                realProb[i][z] = (float)oneCount[i]/(float)(z+1);
            }
            else
            {
                realProb[i][z] = (realProb[i][z-1]*(float)accuracyLength+outSeq[i][z]-outSeq[i][z-accuracyLength])/(float)accuracyLength;
            }
            errRate[i][z] = (theoProb[i] - realProb[i][z])/theoProb[i];
        }
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


