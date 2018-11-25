#include "relu.hpp"
#include "perfsim.hpp"

void RELU::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling RELU Help. Following are instructions to RELU Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Depth of Saturation Counter, Window Size, Threshold for Window Bias, Instance Name.\n");
    printf("Input [0] is used.\n");
    printf("Depth of Saturation Counter is suggested to be 3.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits, Vectorized Random Number.\n");
    printf("Input [0] is used.\n");

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

void RELU::Init(vector<float> param1, unsigned int param2, unsigned int param3, float param4, string param5)
{
    iProb = param1;
    depth = param2;
    wSize = param3;
    thdBias = param4;
    m_name = param5;

    iDim = (unsigned int)iProb.size();
    if (iDim != 1)
    {
        printf("Error: Input dimension is not 1.\n");
    }
    #ifdef PERFSIM
        iLen = 0;
    #endif
    upperBound = (unsigned int)pow(2,depth)-1;
    halfBound = (unsigned int)pow(2,depth-1);
    satCnt = halfBound;

    oDim = 1;
    oBit.resize(oDim);

    #ifdef PERFSIM
        oBS.resize(oDim);
        wProb.resize(oDim);
        theoProb.resize(oDim);
        wBias.resize(oDim);
        cTime.resize(oDim);

        for (int i = 0; i < oDim; ++i)
        {
            wProb[i] = 0;
            theoProb[i] = iProb[0] > 0.5 ? iProb[0] : 0.5;
            cTime[i] = 0;
        }
    #endif
}

void RELU::Calc(vector<char> param1, vector<unsigned int> param2)
{
    iBit = param1;
    randNum = param2;

    if (satCnt >= halfBound)
    {
        oBit[0] = iBit[0];
    }
    else
    {
        oBit[0] = randNum[0];
    }
    if (iBit[0] == 1)
    {
        if (satCnt < upperBound)
        {
            satCnt += 1;
        }
    }
    else
    {
        if (satCnt > 0)
        {
            satCnt -= 1;
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
                cTime[i] = iLen;
            }
        }
    #endif
}

vector<char> RELU::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<float> RELU::WProb()
    {
        return wProb;
    }

    vector<float> RELU::TheoProb()
    {
        return theoProb;
    }

    vector<float> RELU::WBias()
    {
        return wBias;
    }

    vector<unsigned int> RELU::CTime()
    {
        return cTime;
    }
#endif
