#include "gsqrt.hpp"
#include "perfsim.hpp"

void GSQRT::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling GSQRT Help. Following are instructions to GSQRT Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Depth of Synchronizer, Depth of Cordiv Kernel, Window Size, Threshold for Window Bias, Instance Name.\n");
    printf("Input 0 is the valid input.\n");
    printf("Depth of Cordiv Kernel is not used.");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits, Vectorized Random Number.\n");
    printf("Input 0 is the valid input.\n");
    printf("The 0th value of Vectorized Random Number is for the Synchronizer.\n");
    printf("The 1th value of Vectorized Random Number is for the Cordiv Kernel and not used.\n");

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

void GSQRT::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, float param5, string param6)
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

    upperBound = (unsigned int)pow(2,depthSync)-1;
    halfBound = (unsigned int)pow(2,depthSync-1);
    cnt = halfBound;
    oBitOld = 0;

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
        speed.resize(oDim);

        for (int i = 0; i < oDim; ++i)
        {
            wProb[i] = 0;
            theoProb[i] = sqrt(iProb[0]);
            speed[i] = 0;
        }
    #endif
}

void GSQRT::Calc(vector<char> param1, vector<unsigned int> param2)
{
    iBit = param1;
    randNum = param2;

    // *****************************************************************************
    // counter based no correlation
    // *****************************************************************************

    if (cnt >= randNum[0])
    {
        oBit[0] = 1;
    }
    else
    {
        oBit[0] = 0;
    }

    andGate = oBit[0] & oBitOld;
    inc = iBit[0];
    dec = andGate;

    oBitOld = oBit[0];

    if (inc == 1 && dec == 0)
    {
        if (cnt < upperBound)
        {
            cnt = cnt + 1;
        }
    }
    else if (inc == 0 && dec == 1)
    {
        if (cnt > 0)
        {
            cnt = cnt - 1;
        }
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

vector<char> GSQRT::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<float> GSQRT::WProb()
    {
        return wProb;
    }

    vector<float> GSQRT::TheoProb()
    {
        return theoProb;
    }

    vector<float> GSQRT::WBias()
    {
        return wBias;
    }

    vector<unsigned int> GSQRT::Speed()
    {
        return speed;
    }
#endif
