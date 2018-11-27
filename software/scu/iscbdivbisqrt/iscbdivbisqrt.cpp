#include "iscbdivbisqrt.hpp"
#include "perfsim.hpp"

void ISCBDIVBISQRT::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling ISCBDIVBISQRT Help. Following are instructions to ISCBDIVBISQRT Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Depth of Synchronizer, Depth of Cordiv Kernel, Window Size, Threshold for Window Bias, Instance Name.\n");
    printf("Input 0 is the valid input.\n");
    printf("Depth of Synchronizer are not used.");
    printf("Depth of Cordiv Kernel is suggested to be 2.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits, Vectorized Random Number.\n");
    printf("Input 0 is the valid input.\n");
    printf("The 0th value of Vectorized Random Number is for Synchronizer and not used.\n");
    printf("The 1th value of Vectorized Random Number is for Cordiv Kernel and used.\n");

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

    printf("8. inst.CTime() method:\n");
    printf("Return the converge cTime.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void ISCBDIVBISQRT::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, float param5, string param6)
{
    iProb = param1;
    depthSync = param2;
    depth = param3;
    wSize = param4;
    thdBias = param5;
    m_name = param6;

    iDim = (unsigned int)iProb.size();
    if (iDim != 1)
    {
        printf("Error: Input dimension is not 1.\n");
    }
    oDim = 1;
    #ifdef PERFSIM
        iLen = 0;
    #endif

    muxProb.resize(2);
    muxInBit.resize(2);
    muxProb[0] = 1;
    muxProb[1] = iProb[0];
    muxInst.Init(muxProb, wSize, thdBias, "muxInst");
    traceProb.resize(2);
    traceInBit.resize(2);
    traceProb[0] = 0.5;
    traceProb[1] = 0.5+sqrt(iProb[0])/2;
    traceInst.Init(traceProb, depthSync, depth, wSize, thdBias, "traceInst");
    muxInNum.resize(1);
    traceInNum.resize(2);

    DFF.resize(1);
    DFF[0] = 0;
    
    oBit.resize(oDim);
    for (int i = 0; i < oDim; ++i)
    {
        oBit[i] = 0;
    }
    
    #ifdef PERFSIM
        oBS.resize(oDim);
        wProb.resize(oDim);
        theoProb.resize(oDim);
        wBias.resize(oDim);
        cTime.resize(oDim);

        for (int i = 0; i < oDim; ++i)
        {
            wProb[i] = 0;
            theoProb[i] = sqrt(iProb[0]);
            cTime[i] = 0;
        }
    #endif
}

void ISCBDIVBISQRT::Calc(vector<char> param1, vector<unsigned int> param2)
{
    iBit = param1;
    randNum = param2;

    #ifdef PERFSIM
        if (iBit.size() != 1 || randNum.size() != 2)
        {
            printf("Error: Input Number to ISCBDIVBISQRT is wrong.");
            printf("    Expect: (iBit(1), randNum(2))\n");
            printf("    Actual: (iBit(%u), randNum(%u))\n", iBit.size(), randNum.size());
        }
    #endif
    // *****************************************************************************
    // bit inserting with simplified iscvdiv for 1/(1+Po)
    // *****************************************************************************
    
    muxInBit[0] = 1;
    muxInBit[1] = iBit[0];
    muxInNum[0] = (unsigned int)traceInst.OutBit()[0];
    muxInst.Calc(muxInBit, muxInNum);

    oBit[0] = muxInst.OutBit()[0];

    traceInBit[0] = DFF[0];
    traceInBit[1] = (oBit[0] & (1-DFF[0])) | DFF[0];
    traceInNum[0] = 0;
    traceInNum[1] = randNum[1];
    traceInst.Calc(traceInBit, traceInNum);

    DFF[0] = 1 - DFF[0];

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
                cTime[i] = iLen;
            }
        }
    #endif
}

vector<char> ISCBDIVBISQRT::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<vector<char>> ISCBDIVBISQRT::OutBS()
    {
        return oBS;
    }
    
    vector<float> ISCBDIVBISQRT::WProb()
    {
        return wProb;
    }

    vector<float> ISCBDIVBISQRT::TheoProb()
    {
        return theoProb;
    }

    vector<float> ISCBDIVBISQRT::WBias()
    {
        return wBias;
    }

    vector<unsigned int> ISCBDIVBISQRT::CTime()
    {
        return cTime;
    }
#endif
