#include "add.hpp"
#include "apcadd.hpp"
#include "exp.hpp"
#include "div.hpp"
#include "softmax.hpp"
#include "seqprob.hpp"
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

    // printf("%d\n", inSeq.size());
    // printf("%d\n", inProb.size());
    if ((unsigned int)inSeq.size() == (unsigned int)inProb.size() && (unsigned int)inSeq.size() == 16)
    {
        inSeqDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Sequence Dimension is not 16.\n");
    }

    // printf("%d\n", randNum.size());
    if ((unsigned int)randNum.size() == 5)
    {
        inRandDim = (unsigned int)randNum.size();
    }
    else
    {
        printf("Error: Input Random Number Dimension is not 5.\n");
    }

    seqLength = (unsigned int)inSeq[0].size();
    // printf("%d\n", seqLength);
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
        sum += exp(inProb[i]*16);
    }
    // printf("%f\n", sum/16/4);

    // printf("bbbb\n");
    theoProb.resize(inSeqDim);
    for (int i = 0; i < inSeqDim; ++i)
    {
        theoProb[i] = exp(inProb[i]*16)/sum;
        printf("softmax(%d): %f\n", i, theoProb[i]);
    }

    // printf("cccc\n");
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

    // printf("dddd\n");
    mse.resize(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        mse[i] = 0;
    }
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
    // scale exp by 4
    vector<vector<unsigned int>> expOut(inSeqDim);
    vector<vector<unsigned int>> expSel(3);
    for (int i = 0; i < 3; ++i)
    {
        expSel[i] = randNum[i];
    }

    for (int i = 0; i < inSeqDim; ++i)
    {
        EXP expInst;
        expInst.Init(inSeq[i], expSel, bitLength, "expInst");
        expInst.Calc();
        expOut[i] = expInst.OutSeq();
        printf("exp(%d): %.3f,%.3f,%.3f\n", i, expInst.InProb(), expInst.TheoProb(), expInst.FinalRealProb());
    }

    // sum for all exp output
    // scale sum by 16
    ADD sumInst;
    sumInst.Init(expOut, randNum[3] , bitLength, "addInst");
    sumInst.Calc();

    // SeqProb sumProbInst;
    // sumProbInst.Init(sumInst.OutSeq() , "sumProbInst");
    // sumProbInst.Calc();
    // printf("%f\n", sumProbInst.OutProb());

    // division for output
    vector<vector<unsigned int>> divInSeq(2);
    for (int i = 0; i < 2; ++i)
    {
        divInSeq[i].resize(seqLength);
    }
    divInSeq[1] = sumInst.OutSeq();
    unsigned int depth = 2;
    for (int i = 0; i < inSeqDim; ++i)
    {
        for (int j = 0; j < seqLength; ++j)
        {
            divInSeq[0][j] = ((randNum[4][j] >> (bitLength - (unsigned int)log2(inSeqDim))) == 0) ? expOut[i][j] : 0;
        }
        DIV divInst;
        divInst.Init(divInSeq, randNum[4], bitLength, depth, "divInst");
        divInst.Calc();
        outSeq[i] = divInst.OutSeq();
        printf("div(%d): %.3f,%.3f,%.3f,%.3f\n", i, divInst.InProb()[0], divInst.InProb()[1], divInst.TheoProb(), divInst.FinalRealProb());
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
    // for (int i = 0; i < seqLength; ++i)
    // {
    //     if (errRate[seqLength-1-i] > 0.05 || errRate[seqLength-1-i] < -0.05)
    //     {
    //         lowErrLen = seqLength-i;
    //         break;
    //     }
    // }

    for (int i = 0; i < seqLength; ++i)
    {
        for (int j = 0; j < inSeqDim; ++j)
        {
            mse[i] += errRate[j][i]*errRate[j][i];
        }
        mse[i] /= sqrt(mse[i]/(float)inSeqDim);
        // printf("%.3f,", mse[i]);
    }
    // printf("\n");
    // for (int i = 0; i < inSeqDim; ++i)
    // {
    //     for (int j = 0; j < seqLength; ++j)
    //     {
    //         // printf("%f,", errRate[i][j]);
    //         printf("%f,", realProb[i][j]);
    //     }
    //     printf("\n");
    // }
}

vector<vector<unsigned int>> SOFTMAX::OutSeq()
{
    return outSeq;
}
