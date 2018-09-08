#include "add.hpp"
#include "apcadd.hpp"
#include "exp.hpp"
#include "expscaled.hpp"
#include "div.hpp"
#include "softmax.hpp"
#include "seqprob.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

UNITVECTOR::UNITVECTOR(){}

UNITVECTOR::~UNITVECTOR(){}

void UNITVECTOR::Init(vector<vector<unsigned int>> param1, vector<unsigned int> param2, vector<unsigned int> param3, vector<vector<unsigned int>> param4, unsigned int param5, string param6);
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randAdd = param2;
    randSqrt = param3;
    randDiv = param4;
    bitLength = param5;
    m_name = param6;

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
        printf("unitvector(%d): %f, %f\n", i, inProb[i], theoProb[i]);
    }

    // printf("cccc\n");
    outSeq.resize(seqDim);
    realProb.resize(seqDim);
    errRate.resize(seqDim);
    finalRealProb.resize(seqDim);
    finalErrRate.resize(seqDim);

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

    // printf("dddd\n");
    mse.resize(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        mse[i] = 0;
    }
    lowErrLen = 0;
}

void UNITVECTOR::Calc()
{
    vector<float> oneCount(seqDim);
    for (int i = 0; i < seqDim; ++i)
    {
        oneCount[i] = 0;
    }
    unsigned int  accuracyLength = seqLength/2;

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
        MUL squareInst(inSquare,"squareInst");
        squareInst.Calc();
        outSquare[i] = squareInst.OutSeq();
    }

    // sum all square
    vector<unsigned int> outMuxAdd(seqLength);
    MUXADD muxaddInst(outSquare, randAdd, "muxaddInst");
    muxaddInst.Calc();
    outMuxAdd = muxaddInst.OutSeq();

    // get the square root of sum
    vector<unsigned int> outSqrt(seqLength);
    GSQRT sqrtInst(outMuxAdd, randSqrt, "sqrtInst");
    // JKDIVBISQRT sqrtInst(outMuxAdd, randSqrt, "sqrtInst");
    // ISCBDIVBISQRT sqrtInst(outMuxAdd, randSqrt, "sqrtInst");
    sqrtInst.Calc();
    outSqrt = sqrtInst.OutSeq();

    // division for each



    // // sum for all exp output
    // // scale sum by 16
    // ADD sumInst;
    // sumInst.Init(expOut, randNum[3] , bitLength, "addInst");
    // sumInst.Calc();

    // Or all the sums
    vector<unsigned int> sumArray(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        sumArray[i] = 0;
        for (int j = 0; j < seqDim; ++j)
        {
            sumArray[i] |= expOut[j][i];
        }
    }

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
    // divInSeq[1] = sumInst.OutSeq();
    divInSeq[1] = sumArray;
    unsigned int depth = 4;
    for (int i = 0; i < seqDim; ++i)
    {
        for (int j = 0; j < seqLength; ++j)
        {
            // divInSeq[0][j] = ((randNum[4][j] >> (bitLength - (unsigned int)log2(seqDim))) == 0) ? expOut[i][j] : 0;
            divInSeq[0][j] = expOut[i][j];
        }
        DIV divInst;
        divInst.Init(divInSeq, randNum[4], bitLength, depth, "divInst");
        divInst.Calc();
        outSeq[i] = divInst.OutSeq();
        printf("div(%d): %.3f,%.3f,%.3f,%.3f\n", i, divInst.InProb()[0], divInst.InProb()[1], divInst.TheoProb(), divInst.FinalRealProb());
    }

    unsigned int accuracyLength = 128;
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
        for (int j = 0; j < seqDim; ++j)
        {
            mse[i] += errRate[j][i]*errRate[j][i];
        }
        mse[i] /= sqrt(mse[i]/(float)seqDim);
        // printf("%.3f,", mse[i]);
    }
    // printf("\n");
    // for (int i = 0; i < seqDim; ++i)
    // {
    //     for (int j = 0; j < seqLength; ++j)
    //     {
    //         // printf("%f,", errRate[i][j]);
    //         printf("%f,", realProb[i][j]);
    //     }
    //     printf("\n");
    // }
}

vector<vector<unsigned int>> UNITVECTOR::OutSeq()
{
    return outSeq;
}
