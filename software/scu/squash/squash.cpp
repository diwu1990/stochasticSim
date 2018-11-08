#include <squash.hpp>
#include <perfsim.hpp>

SQUASH::SQUASH()
{
}

SQUASH::~SQUASH()
{
    delete [] squareInstPtr;
    delete [] mulOutInstPtr;
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

void SQUASH::Init(vector<float> param1, float param2, unsigned int param3, unsigned int param4, unsigned int param5, float param6, string param7)
{
    iProb = param1;
    scale = param2;
    depthSync = param3;
    depth = param4;
    wSize = param5;
    thdBias = param6;
    m_name = param7;

    iDim = iProb.size();
    if (iDim < 2)
    {
        printf("Error: Input Dimension is smaller than 2.\n");
    }
    #ifdef PERFSIM
        iLen = 0;
    #endif

    sqreIBit.resize(iDim);
    sumIBit.resize(iDim);
    sqrtIBit.resize(1);
    add1IBit.resize(2);
    divIBit.resize(2);
    mulScaleIBit.resize(2);
    mulOutIBit.resize(iDim);
    for (int i = 0; i < iDim; ++i)
    {
        sqreIBit[i].resize(1);
        mulOutIBit[i].resize(2);
    }

    sqreIProb.resize(iDim);
    sumIProb.resize(iDim);
    sqrtIProb.resize(1);
    add1IProb.resize(2);
    divIProb.resize(2);
    mulScaleIProb.resize(2);
    mulOutIProb.resize(iDim);
    for (int i = 0; i < iDim; ++i)
    {
        sqreIProb[i].resize(1);
        mulOutIProb[i].resize(2);
    }

    squareInstPtr = new SQUARE[iDim];
    for (int i = 0; i < iDim; ++i)
    {
        // printf("%u\n", squareInstPtr[i]);
        sqreIProb[i][0] = iProb[i];
        // printf("square in: %f\n", sqreIProb[i][0]);
        squareInstPtr[i].Init(sqreIProb[i], wSize, thdBias, "squareInst");
    }
    printf("\n");
    for (int i = 0; i < iDim; ++i)
    {
        sumIProb[i] = squareInstPtr[i].TheoProb()[0];
        // printf("square out: %f\n", sumIProb[i]);
    }
    addSqreInst.Init(sumIProb, wSize, thdBias, "addSqreInst");
    // printf("\n");
    // printf("add square out: %f\n", addSqreInst.TheoProb()[0]);

    sqrtIProb[0] = addSqreInst.TheoProb()[0];
    sqrtInst.Init(sqrtIProb, depthSync, depth, wSize, thdBias, "sqrtInst");
    // printf("\n");
    // printf("square root out: %f\n", sqrtInst.TheoProb()[0]);

    mulScaleIProb[0] = sqrtInst.TheoProb()[0];
    mulScaleIProb[1] = pow(2,-1-log2(iDim)/2);
    // printf("mulScaleIProb[1]: %f\n", mulScaleIProb[1]);
    mulScaleInst.Init(mulScaleIProb, wSize, thdBias, "mulScaleInst");
    // printf("mul scale out: %f\n", mulScaleInst.TheoProb()[0]);
    // printf("\n");

    add1IProb[0] = scale;
    // printf("scale: %f\n", scale);
    add1IProb[1] = addSqreInst.TheoProb()[0];
    add1Inst.Init(add1IProb, wSize, thdBias, "addSqreInst");
    // printf("\n");
    // printf("add 1 out: %f\n", add1Inst.TheoProb()[0]);

    divIProb[0] = mulScaleInst.TheoProb()[0];
    divIProb[1] = add1Inst.TheoProb()[0];
    divInst.Init(divIProb, depthSync, depth, wSize, thdBias, "divInst");
    // printf("\n");
    // printf("division out: %f\n", divInst.TheoProb()[0]);

    // printf("\n");

    oDim = iDim;
    oBit.resize(oDim);

    mulOutInstPtr = new ANDMUL[oDim];
    for (int i = 0; i < oDim; ++i)
    {
        mulOutIProb[i][0] = iProb[i];
        mulOutIProb[i][1] = divInst.TheoProb()[0];
        mulOutInstPtr[i].Init(mulOutIProb[i], wSize, thdBias, "mulOutInst");
    }

    #ifdef PERFSIM
        oBS.resize(oDim);
        wProb.resize(oDim);
        theoProb.resize(oDim);
        wBias.resize(oDim);
        speed.resize(oDim);

        for (int i = 0; i < oDim; ++i)
        {
            wProb[i] = 0;
            theoProb[i] = mulOutInstPtr[i].TheoProb()[0];
            // printf("theo out: %f\n", theoProb[i]);
            speed[i] = 0;
        }
    #endif
}

void SQUASH::Calc(vector<char> param1, vector<unsigned int> param2, vector<unsigned int> param3, vector<char> param4)
{
    iBit = param1;
    sumRandNum = param3;
    add1RandNum = param3;
    mulScaleIBit[1] = param4;

    // square
    for (int i = 0; i < iDim; ++i)
    {
        sqreIBit[i][0] = iBit[i];
        squareInstPtr[i].Calc(sqreIBit[i]);
    }
    // printf("square done!\n\n");

    // sum all square
    for (int i = 0; i < iDim; ++i)
    {
        sumIBit[i] = squareInstPtr.OBit()[0];
    }
    addSqreInst.Calc(sumIBit,);
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
