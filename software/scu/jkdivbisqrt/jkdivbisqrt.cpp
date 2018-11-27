#include "jkdivbisqrt.hpp"
#include "perfsim.hpp"

void JKDIVBISQRT::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling JKDIVBISQRT Help. Following are instructions to JKDIVBISQRT Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Depth of Synchronizer, Depth of Cordiv Kernel, Window Size, Threshold for Window Bias, Instance Name.\n");
    printf("Input 0 is the valid input.\n");
    printf("Both Depth of Synchronizer and Depth of Cordiv Kernel are not used.");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits, Vectorized Random Number.\n");
    printf("Input 0 is the valid input.\n");
    printf("Both the 0th and the 1th value of Vectorized Random Number are not used.\n");

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

void JKDIVBISQRT::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, float param5, string param6)
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
    traceProb[0] = 1;
    traceProb[1] = sqrt(iProb[0]);
    traceInst.Init(traceProb, wSize, thdBias, "traceInst");
    muxInNum.resize(1);
    
    oBit.resize(oDim);
    for (int i = 0; i < oDim; ++i)
    {
        oBit[i] = 0;
    }
    
    #ifdef PERFSIM
        // *************************************
        // add for debugging
        traceBS.resize(2);
        // *************************************
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

void JKDIVBISQRT::Calc(vector<char> param1, vector<unsigned int> param2)
{
    iBit = param1;
    randNum = param2;

    // *****************************************************************************
    // counter based no correlation
    // *****************************************************************************
    muxInBit[0] = 1;
    muxInBit[1] = iBit[0];
    muxInNum[0] = (unsigned int)traceInst.OutBit()[0];
    muxInst.Calc(muxInBit, muxInNum);
    
    oBit[0] = muxInst.OutBit()[0];
    
    traceInBit[0] = 1;
    traceInBit[1] = muxInst.OutBit()[0];
    traceInst.Calc(traceInBit);

    #ifdef PERFSIM
        // *************************************
        // add for debugging
        traceBS[0].push_back(traceInst.OutBit()[0]);
        traceBS[1].push_back(iBit[0]);
        // *************************************
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

vector<char> JKDIVBISQRT::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    // *************************************
    // add for debugging
    vector<vector<char>> JKDIVBISQRT::TraceBS()
    {
        return traceBS;
    }
    // *************************************
    
    vector<vector<char>> JKDIVBISQRT::OutBS()
    {
        return oBS;
    }
    
    vector<float> JKDIVBISQRT::WProb()
    {
        return wProb;
    }

    vector<float> JKDIVBISQRT::TheoProb()
    {
        return theoProb;
    }

    vector<float> JKDIVBISQRT::WBias()
    {
        return wBias;
    }

    vector<unsigned int> JKDIVBISQRT::CTime()
    {
        return cTime;
    }
#endif
