#include "mul.hpp"
#include "muxadd.hpp"
#include "seqprob.hpp"
#include "seqprobmulti.hpp"
#include "conv2d.hpp"

CONV2D::CONV2D(){}

CONV2D::~CONV2D(){}

void CONV2D::Init(vector<vector<vector<float>>> param1, vector<vector<vector<vector<float>>>> param2, vector<float> param3, unsigned int param4, unsigned int param5)
{
    iFmProb = param1;
    wgtProb = param2;
    biasProb = param3;
    issame = param4;
    stride = param5;

    unsigned int ifm_c;
    unsigned int ifm_h;
    unsigned int ifm_w;

    unsigned int wgt_h;
    unsigned int wgt_w;

    unsigned int ofm_c;
    unsigned int ofm_h;
    unsigned int ofm_w;
    
    if ((unsigned int)iFmProb.size() == (unsigned int)iWeight[0].size())
    {
        weightDim = (unsigned int)iWeight.size();
        seqLength = (unsigned int)iWeight[0][0].size();
    }
    else
    {
        printf("Error: Weight Dimensions are not the same.\n");
    }
    
    if ((unsigned int)iFeature.size() == (unsigned int)iFeature[0].size())
    {
        featureDim = (unsigned int)iFeature.size();
    }
    else
    {
        printf("Error: Feature Dimensions are not the same.\n");
    }

    for (int i = 0; i < weightDim; ++i)
    {
        
    }

    SeqProbMulti weightProbCalc;
    weightProbCalc.Init(iWeight,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();

    

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
}

void CONV2D::Calc()
{
    vector<float> oneCount(seqDim);
    for (int i = 0; i < seqDim; ++i)
    {
        oneCount[i] = 0;
    }
    unsigned int  accuracyLength = seqLength/2;

    // printf("start=>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    // square of input
    vector<vector<unsigned int>> outSquare(seqDim);
    for (int i = 0; i < seqDim; ++i)
    {
        vector<vector<unsigned int>> inSquare(2);
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
    vector<unsigned int> outMuxAdd(seqLength);
    MUXADD muxaddInst;
    muxaddInst.Init(outSquare, randAdd, "muxaddInst");
    muxaddInst.Calc();
    outMuxAdd = muxaddInst.OutSeq();
    // printf("%f->%f(%f)\n", muxaddInst.TheoProb(), muxaddInst.FinalRealProb(), muxaddInst.FinalErrRate());
    // printf("sum done!\n\n");

    // get the square root of sum
    vector<unsigned int> outSqrt(seqLength);
    // GSQRT sqrtInst;
    // JKDIVBISQRT sqrtInst;
    ISCBDIVBISQRT sqrtInst;
    sqrtInst.Init(outMuxAdd, randSqrt, depthSqrt, "sqrtInst");
    sqrtInst.Calc();
    outSqrt = sqrtInst.OutSeq();
    // printf("%f->%f(%f)\n", sqrtInst.TheoProb(), sqrtInst.FinalRealProb(), sqrtInst.FinalErrRate());
    // printf("square root done!\n\n");

    // get the inProb[i]/seqDim
    vector<vector<unsigned int>> inSeqSmall(seqDim);
    vector<vector<unsigned int>> outSeqSmall(seqDim);
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
    vector<vector<unsigned int>> divInSeq(2);
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

vector<vector<unsigned int>> CONV2D::OutSeq()
{
    return outSeq;
}

vector<float> CONV2D::InProb()
{
    // printf("In Prob:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%f\n", inProb[i]);
    // }
    // printf("\n");
    return inProb;
}

vector<float> CONV2D::TheoProb()
{
    // printf("Theo Prob:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%f\n", theoProb[i]);
    // }
    // printf("\n");
    return theoProb;
}

vector<vector<float>> CONV2D::RealProb()
{
    return realProb;
}

vector<vector<float>> CONV2D::ErrRate()
{
    return errRate;
}

vector<float> CONV2D::FinalRealProb()
{
    // printf("Final Real Prob:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%f\n", finalRealProb[i]);
    // }
    // printf("\n");
    return finalRealProb;
}

vector<float> CONV2D::FinalErrRate()
{
    // printf("Final Error Rate:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%f\n", finalErrRate[i]);
    // }
    // printf("\n");
    return finalErrRate;
}

float CONV2D::FinalMSE()
{
    // printf("Final MSE:\n");
    // printf("%f\n", finalMSE);
    // printf("\n");
    return finalMSE;
}

vector<float> CONV2D::MSE()
{
    return mse;
}

vector<unsigned int> CONV2D::CTime()
{
    // printf("Low Err Len:\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     printf("%u\n", lowErrLen[i]);
    // }
    // printf("\n");
    return lowErrLen;
}

float CONV2D::AvgCTime()
{
    // printf("Avg Low Err Len:\n");
    // printf("%u\n", avgLowErrLen);
    // printf("\n");
    return avgLowErrLen;
}