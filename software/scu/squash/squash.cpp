#include <squash.hpp>
#include <perfsim.hpp>

SQUASH::SQUASH()
{
}

SQUASH::~SQUASH()
{
    free(divInstPtr);
}

void SQUASH::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SQUASH Help. Following are instructions to SQUASH Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Depth of Synchronizer, Depth of CORDIV Kernel, Window Size, Threshold for Window Bias, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits, Vectorized Random Number.\n");

    printf("3. inst.OutBit() method:\n");
    printf("Return output bit from inst.Calc().\n");

    printf(">>>>>>>>The following methods require macro definition of PERFSIM.<<<<<<<<\n");
    printf("4. inst.OutBS() method:\n");
    printf("Return output bit stream from inst.Calc().\n");

    printf("5. inst.RealProb() method:\n");
    printf("Return the real probability.\n");

    printf("6. inst.TheoProb() method:\n");
    printf("Return the theoretical probability.\n");

    printf("7. inst.WBias() method:\n");
    printf("Return the window bias.\n");

    printf("8. inst.Speed() method:\n");
    printf("Return the converge speed.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void SQUASH::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, float param5, string param6)
{
    iProb = param1;
    depthSync = param2;
    depth = param3;
    wSize = param4;
    thdBias = param5;
    m_name = param6;

    iDim = iProb.size();
    if (iDim < 2)
    {
        printf("Error: Input Dimension is small than 2.\n");
    }
    #ifdef PERFSIM
        iLen = 0;
    #endif
    sqreBit.resize(iDim);
    sumBit.resize(1);
    sqrtBit.resize(1);
    add1Bit.resize(1);

    divInstPtr = (ISCBDIV *)malloc(iDim*sizeof(ISCBDIV));
    // divInstPtr = (CORDIV *)malloc(iDim*sizeof(CORDIV));
    // divInstPtr = (GDIV *)malloc(iDim*sizeof(GDIV));

    sqreProb.resize(iDim);
    sumProb.resize(1);
    sqrtProb.resize(1);
    add1Prob.resize(1);

    probVec.resize(iDim);
    for (int i = 0; i < iDim; ++i)
    {
        probVec[i].resize(2);
    }
    sumProb[0] = 0;
    for (int i = 0; i < iDim; ++i)
    {
        probVec[];
        sqreProb[i] = iProb[i] * iProb[i];
        sumProb[0] += sqreProb[i];
        divInstPtr[i].Init(probVec, depthSync, depth, wSize, thdBias, "divInst");
    }
    sumProb[0] /= iDim;
    sqrtProb[0] = sqrt(sqrt);

    oDim = iDim;
    oBit.resize(oDim);

    #ifdef PERFSIM
        oBS.resize(oDim);
        wProb.resize(oDim);
        theoProb.resize(oDim);
        wBias.resize(oDim);
        speed.resize(oDim);

        for (int i = 0; i < oDim; ++i)
        {
            wProb[i] = 0;
            theoProb[i] = iProb[i];
            speed[i] = 0;
        }
    #endif
}

void SQUASH::Calc(vector<char> param1, vector<unsigned int> param2)
{
    iBit = param1;
    randNum = param2;

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


    #ifdef PERFSIM
        iLen++;
        vector<unsigned int> totalSum(oDim);
        for (int i = 0; i < oDim; ++i)
        {
            totalSum[i] = 0;
            oBS[i].push_back(oBit[i]);
            if (oBS[i].size() < wSize)
            {
                for (int j = 0; j < oBS[i].size(); ++j)
                {
                    totalSum[i] += oBS[i][j];
                }
                wProb[i] = (float)totalSum[i]/iLen;
            }
            else
            {
                for (int j = oBS[i].size() - wSize; j < oBS[i].size(); ++j)
                {
                    totalSum[i] += oBS[i][j];
                }
                wProb[i] = (float)totalSum[i]/wSize;
            }
            wBias[i] = wProb[i] - theoProb[i];
            if ((wBias[i] > thdBias) || (wBias[i] < (0-thdBias)))
            {
                speed[i] = iLen;
            }
        }
    #endif
}

vector<char> SQUASH::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<float> SQUASH::WProb()
    {
        return wProb;
    }

    vector<float> SQUASH::TheoProb()
    {
        return theoProb;
    }

    vector<float> SQUASH::WBias()
    {
        return wBias;
    }

    vector<unsigned int> SQUASH::Speed()
    {
        return speed;
    }
#endif
