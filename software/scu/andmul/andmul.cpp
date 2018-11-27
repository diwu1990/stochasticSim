#include "andmul.hpp"
#include "perfsim.hpp"

void ANDMUL::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling ANDMUL Help. Following are instructions to ANDMUL Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Window Size, Threshold for Window Bias, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits.\n");

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

void ANDMUL::Init(vector<float> param1, unsigned int param2, float param3, string param4)
{
    iProb = param1;
    wSize = param2;
    thdBias = param3;
    m_name = param4;

    iDim = (unsigned int)iProb.size();
    if (iDim != 2)
    {
        printf("Error: Input dimension is not 2.\n");
    }
    oDim = 1;
    #ifdef PERFSIM
        iLen = 0;
    #endif

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
            theoProb[i] = iProb[0]*iProb[1];
            cTime[i] = 0;
        }
    #endif
}

void ANDMUL::Calc(vector<char> param1)
{
    iBit = param1;

    oBit[0] = iBit[0] & iBit[1];

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

vector<char> ANDMUL::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<vector<char>> ANDMUL::OutBS()
    {
        return oBS;
    }
    
    vector<float> ANDMUL::WProb()
    {
        return wProb;
    }

    vector<float> ANDMUL::TheoProb()
    {
        return theoProb;
    }

    vector<float> ANDMUL::WBias()
    {
        return wBias;
    }

    vector<unsigned int> ANDMUL::CTime()
    {
        return cTime;
    }
#endif
