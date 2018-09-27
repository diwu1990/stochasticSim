#include "mul.hpp"
#include "muxadd.hpp"
#include "gdiv.hpp"
#include "cordiv.hpp"
#include "iscbdiv.hpp"
#include "gsqrt.hpp"
#include "jkdivbisqrt.hpp"
#include "iscbdivbisqrt.hpp"
#include "seqprob.hpp"
#include "seqprobmulti.hpp"
#include "unitvector.hpp"

UNITVECTOR::UNITVECTOR(){}

UNITVECTOR::~UNITVECTOR(){}

void UNITVECTOR::Init(vector<vector<char>> param1, vector<unsigned int> param2, vector<unsigned int> param3, vector<vector<unsigned int>> param4, unsigned int param5, unsigned int param6, unsigned int param7, string param8)
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randAdd = param2;
    randSqrt = param3;
    randDiv = param4;
    depthSqrt = param5;
    depthDiv = param6;
    depthDivSync = param7;
    m_name = param8;

    if ((unsigned int)inSeq.size() == (unsigned int)randDiv.size())
    {
        seqDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Sequence Dimension is not the same as that for Division.\n");
    }

    seqLength = (unsigned int)inSeq[0].size();
    // printf("%d\n", seqLength);
    for (int i = 0; i < seqDim; ++i)
    {
        if ((unsigned int)inSeq[i].size() != seqLength)
        {
            printf("Error: Input Sequence Length is not the same.\n");
            break;
        }
    }

    float rms = 0;
    for (int i = 0; i < seqDim; ++i)
    {
        rms += inProb[i]*inProb[i];
    }
    // printf("%f\n", sum/16/4);

    theoProb.resize(seqDim);
    for (int i = 0; i < seqDim; ++i)
    {
        theoProb[i] = inProb[i]/rms;
        // printf("unitvector(%d): %f, %f\n", i, inProb[i], theoProb[i]);
    }

    // printf("cccc\n");
    outSeq.resize(seqDim);
    realProb.resize(seqDim);
    errRate.resize(seqDim);
    finalRealProb.resize(seqDim);
    finalErrRate.resize(seqDim);

    // printf("flag1\n");
    for (int i = 0; i < seqDim; ++i)
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

    // printf("flag2\n");
    // printf("dddd\n");
    mse.resize(seqLength);
    // printf("flag3\n");
    for (int i = 0; i < seqLength; ++i)
    {
        mse[i] = 0;
    }
    // printf("flag4\n");
    lowErrLen.resize(seqDim);
    for (int i = 0; i < seqDim; ++i)
    {
        // printf("start)%d\n", i);
        lowErrLen[i] = 0;
        // printf("end)%d\n", i);
    }
    // printf("flag5\n");
    finalMSE = 0;
    avgLowErrLen = 0;
    theoSqrtProb = 0;
    for (int i = 0; i < seqDim; ++i)
    {
        theoSqrtProb += inProb[i]*inProb[0];
    }
    theoSqrtProb /= seqDim;
    theoSqrtProb = sqrt(theoSqrtProb);
}

void UNITVECTOR::Calc()
{
    vector<float> oneCount(seqDim);
    for (int i = 0; i < seqDim; ++i)
    {
        oneCount[i] = 0;
    }
    unsigned int  accuracyLength = seqLength/2;

    // printf("start=>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    // square of input
    vector<vector<char>> outSquare(seqDim);
    for (int i = 0; i < seqDim; ++i)
    {
        vector<vector<char>> inSquare(2);
        inSquare[0].resize(seqLength);
        inSquare[1].resize(seqLength);
        for (int j = 0; j < seqLength; ++j)
        {
            inSquare[0][j] = inSeq[i][j];
            inSquare[1][j] = inSeq[i][(j-1+seqLength)%seqLength];
        }
        MUL squareInst;
        squareInst.Init(inSquare,"squareInst");
        squareInst.Calc();
        outSquare[i] = squareInst.OutSeq();
        // printf("%f->%f(%f)\n", squareInst.TheoProb(), squareInst.FinalRealProb(), squareInst.FinalErrRate());
    }
    // printf("square done!\n\n");

    // sum all square
    vector<char> outMuxAdd(seqLength);
    MUXADD muxaddInst;
    muxaddInst.Init(outSquare, randAdd, "muxaddInst");
    muxaddInst.Calc();
    outMuxAdd = muxaddInst.OutSeq();
    // printf("%f->%f(%f)\n", muxaddInst.TheoProb(), muxaddInst.FinalRealProb(), muxaddInst.FinalErrRate());
    // printf("sum done!\n\n");

    // get the square root of sum
    vector<char> outSqrt(seqLength);
    // GSQRT sqrtInst;
    // JKDIVBISQRT sqrtInst;
    ISCBDIVBISQRT sqrtInst;
    sqrtInst.Init(outMuxAdd, randSqrt, depthSqrt, "sqrtInst");
    sqrtInst.Calc();
    outSqrt = sqrtInst.OutSeq();
    sqrtMse = sqrt((sqrtInst.FinalRealProb()-theoSqrtProb)*(sqrtInst.FinalRealProb()-theoSqrtProb));
    // printf("%f->%f(%f)\n", sqrtInst.TheoProb(), sqrtInst.FinalRealProb(), sqrtInst.FinalErrRate());
    // printf("square root done!\n\n");

    // get the inProb[i]/seqDim
    vector<vector<char>> inSeqSmall(seqDim);
    vector<vector<char>> outSeqSmall(seqDim);
    for (int i = 0; i < seqDim; ++i)
    {
        inSeqSmall[i].resize(seqLength);
        outSeqSmall[i].resize(seqLength);
    }
    for (int i = 0; i < seqDim; ++i)
    {
        for (int j = 0; j < seqDim; ++j)
        {
            if (i == j)
            {
                inSeqSmall[j] = inSeq[j];
            }
            else
            {
                for (int k = 0; k < seqLength; ++k)
                {
                    inSeqSmall[j][k] = 0;
                }
            }
        }

        MUXADD inSeqSmallInst;
        inSeqSmallInst.Init(inSeqSmall, randAdd, "inSeqSmallInst");
        inSeqSmallInst.Calc();
        outSeqSmall[i] = inSeqSmallInst.OutSeq();
        // printf("%f->%f(%f)\n", inSeqSmallInst.TheoProb(), inSeqSmallInst.FinalRealProb(), inSeqSmallInst.FinalErrRate());
    }
    // printf("input scaling done!\n\n");

    // division for each
    vector<vector<char>> divInSeq(2);
    for (int i = 0; i < 2; ++i)
    {
        divInSeq[i].resize(seqLength);
    }
    divInSeq[1] = outMuxAdd;
    for (int i = 0; i < seqDim; ++i)
    {
        divInSeq[0] = outSeqSmall[i];
        // GDIV divInst;
        // CORDIV divInst;
        ISCBDIV divInst;
        divInst.Init(divInSeq, randDiv[i], depthDiv, depthDivSync, "divInst");
        divInst.Calc();
        outSeq[i] = divInst.OutSeq();
        // printf("(%f,%f)%f->%f(%f)\n", divInst.InProb()[0], divInst.InProb()[1], divInst.TheoProb(), divInst.FinalRealProb(), divInst.FinalErrRate());
    }
    // printf("division done!\n\n");

    for (int i = 0; i < seqDim; ++i)
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
            errRate[i][z] = (theoProb[i] - realProb[i][z]);
        }
        finalRealProb[i] = realProb[i][seqLength-1];
        finalErrRate[i] = errRate[i][seqLength-1];
    }
    // printf("errRate and realProb done!\n\n");

    for (int i = 0; i < seqDim; ++i)
    {
        for (int j = 0; j < seqLength; ++j)
        {
            if (errRate[i][seqLength-1-j] > 0.05 || errRate[i][seqLength-1-j] < -0.05)
            {
                lowErrLen[i] = seqLength-j;
                break;
            }
        }
    }

    for (int i = 0; i < seqLength; ++i)
    {
        for (int j = 0; j < seqDim; ++j)
        {
            mse[i] += errRate[j][i]*errRate[j][i];
        }
        mse[i] = sqrt(mse[i]/(float)seqDim);
        // printf("%.3f,", mse[i]);
    }
    // printf("\nmse done!\n\n");
    finalMSE = mse[seqLength-1];
    for (int i = 0; i < seqDim; ++i)
    {
        avgLowErrLen += (float)lowErrLen[i];
    }
    avgLowErrLen = avgLowErrLen/(float)seqDim;
}

vector<vector<char>> UNITVECTOR::OutSeq()
{
    return outSeq;
}

vector<float> UNITVECTOR::InProb()
{
    // printf("In Prob:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%f\n", inProb[i]);
    // }
    // printf("\n");
    return inProb;
}

vector<float> UNITVECTOR::TheoProb()
{
    // printf("Theo Prob:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%f\n", theoProb[i]);
    // }
    // printf("\n");
    return theoProb;
}

vector<vector<float>> UNITVECTOR::RealProb()
{
    return realProb;
}

vector<vector<float>> UNITVECTOR::ErrRate()
{
    return errRate;
}

vector<float> UNITVECTOR::FinalRealProb()
{
    // printf("Final Real Prob:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%f\n", finalRealProb[i]);
    // }
    // printf("\n");
    return finalRealProb;
}

vector<float> UNITVECTOR::FinalErrRate()
{
    // printf("Final Error Rate:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%f\n", finalErrRate[i]);
    // }
    // printf("\n");
    return finalErrRate;
}

float UNITVECTOR::FinalMSE()
{
    // printf("Final MSE:\n");
    // printf("%f\n", finalMSE);
    // printf("\n");
    return finalMSE;
}

vector<float> UNITVECTOR::MSE()
{
    return mse;
}

vector<unsigned int> UNITVECTOR::LowErrLen()
{
    // printf("Low Err Len:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%u\n", lowErrLen[i]);
    // }
    // printf("\n");
    return lowErrLen;
}

float UNITVECTOR::AvgLowErrLen()
{
    // printf("Avg Low Err Len:\n");
    // printf("%u\n", avgLowErrLen);
    // printf("\n");
    return avgLowErrLen;
}

float UNITVECTOR::SqrtMSE()
{
    return sqrtMse;
}