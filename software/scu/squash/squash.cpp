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
    printf("Parameters: Input Probability, Scale of 1, Depth of Synchronizer, Depth of CORDIV Kernel, Window Size, Threshold for Window Bias, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits, Vectorized Random Number for [sum, sqrt, div, add1], Vectorized Random Bit for [add1, mulScale].\n");
    printf("Bit Width of Vectorized Random Number for sum: log2(Input Dimension)-bit.\n");
    printf("Bit Width of Vectorized Random Number for sqrt: 1-bit.\n");
    printf("Bit Width of Vectorized Random Number for div: Depth of CORDIV Kernel.\n");
    printf("Bit Width of Vectorized Random Number for add1: 1-bit.\n");
    printf("Probability of Vectorized Random Bit for add1: 1/Scale.\n");
    printf("Probability of Vectorized Random Bit for mulScale: pow(2,-1-log2(Input Dimension)/2).\n");

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
        // printf("square out: %f\n", squareInstPtr[i].TheoProb()[0]);
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

    sumRandNum.resize(1);
    sqrtRandNum.resize(1);
    divRandNum.resize(1);
    add1RandNum.resize(1);

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

void SQUASH::Calc(vector<char> param1, vector<unsigned int> param2, vector<char> param3)
{
    iBit = param1;
    sumRandNum[0] = param2[0];
    sqrtRandNum[0] = param2[1];
    divRandNum[0] = param2[2];
    add1RandNum[0] = param2[3];
    add1IBit[0] = param3[0];
    mulScaleIBit[1] = param3[1];

    // square
    for (int i = 0; i < iDim; ++i)
    {
        sqreIBit[i][0] = iBit[i];
        squareInstPtr[i].Calc(sqreIBit[i]);
    }
    printf("square done!\n\n");

    // sum all square
    for (int i = 0; i < iDim; ++i)
    {
        sumIBit[i] = squareInstPtr[i].OutBit()[0];
        printf("%u\n", sumIBit[i]);
    }
    addSqreInst.Calc(sumIBit,sumRandNum);
    printf("sum done!\n\n");

    // get the square root of sum
    printf("%u\n", addSqreInst.OutBit()[0]);
    printf("%u\n", sqrtRandNum[0]);
    sqrtInst.Calc(addSqreInst.OutBit(), sqrtRandNum);
    printf("square root done!\n\n");

    // scale sqrt
    mulScaleIBit[0] = sqrtInst.OutBit()[0];
    mulScaleInst.Calc(mulScaleIBit);

    // add the sum with 1
    add1IBit[1] = addSqreInst.OutBit()[0];
    add1Inst.Calc(add1IBit, add1RandNum);
    // printf("add 1 done!\n\n");

    // division
    divIBit[0] = mulScaleInst.OutBit()[0];
    divIBit[1] = add1Inst.OutBit()[0];
    divInst.Calc(divIBit, divRandNum);
    // printf("division done!\n\n");

    // output: mul div result with input
    for (int i = 0; i < oDim; ++i)
    {
        mulOutIBit[i][0] = iBit[i];
        mulOutIBit[i][1] = divInst.OutBit()[0];
        mulOutInstPtr[i].Calc(mulOutIBit[i]);
    }

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
