#include "desynchronizer.hpp"
#include "perfsim.hpp"

void DeSynchronizer::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling DeSynchronizer Help. Following are instructions to DeSynchronizer Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Buffer Depth, Window Size, Threshold for Window Bias, Instance Name.\n");
    printf("Buffer Depth is not used in this class, always default to 1.\n");

    printf("2. inst.Calc() method:\n");
    printf("Match logic ones with logic zeros for max negative correlation with configuration in inst.Init().\n");
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

    printf("8. inst.Speed() method:\n");
    printf("Return the converge speed.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void DeSynchronizer::Init(vector<float> param1, unsigned int param2, unsigned int param3, float param4, string param5)
{
    iProb = param1;
    depth = param2;
    wSize = param3;
    thdBias = param4;
    m_name = param5;

    iDim = (unsigned int)iProb.size();
    if (iDim != 2)
    {
        printf("Error: Input dimension is not 2.\n");
    }
    oDim = iDim;
    #ifdef PERFSIM
        iLen = 0;
    #endif
    satCnt0 = 0;
    satCnt1 = 0;

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

void DeSynchronizer::Calc(vector<char> param1)
{
    iBit = param1;

    if (iBit[0] == 0 && iBit[1] == 0)
    {
        if (satCnt0 == 0 && satCnt1 == 0)
        {
            oBit[0] = 0;
            oBit[1] = 0;
        }
        else if (satCnt0 > 0 && satCnt1 == 0)
        {
            oBit[0] = 1;
            oBit[1] = 0;
            satCnt0--;
        }
        else if (satCnt0 == 0 && satCnt1 > 0)
        {
            oBit[0] = 0;
            oBit[1] = 1;
            satCnt1--;
        }
        else
        {
            oBit[0] = 0;
            oBit[1] = 1;
            satCnt1--;
        }
    }
    else if (iBit[0] == 0 && iBit[1] == 1)
    {
        oBit[0] = iBit[0];
        oBit[1] = iBit[1];
    }
    else if (iBit[0] == 1 && iBit[1] == 0)
    {
        oBit[0] = iBit[0];
        oBit[1] = iBit[1];
    }
    else if (iBit[0] == 1 && iBit[1] == 1)
    {
        if (satCnt0 == 0 && satCnt1 == 0)
        {
            oBit[0] = 0;
            oBit[1] = 1;
            satCnt0++;
        }
        else if (satCnt0 > 0 && satCnt1 == 0)
        {
            oBit[0] = 1;
            oBit[1] = 0;
            satCnt1++;
        }
        else if (satCnt0 == 0 && satCnt1 > 0)
        {
            oBit[0] = 0;
            oBit[1] = 1;
            satCnt0++;
        }
        else
        {
            oBit[0] = 1;
            oBit[1] = 1;
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

vector<char> DeSynchronizer::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<float> DeSynchronizer::WProb()
    {
        return wProb;
    }

    vector<float> DeSynchronizer::TheoProb()
    {
        return theoProb;
    }

    vector<float> DeSynchronizer::WBias()
    {
        return wBias;
    }

    vector<unsigned int> DeSynchronizer::Speed()
    {
        return speed;
    }
#endif
