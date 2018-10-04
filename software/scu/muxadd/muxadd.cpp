#include "muxadd.hpp"
#include "perfsim.hpp"

void MUXADD::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling MUXADD Help. Following are instructions to MUXADD Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Window Size, Threshold for Window Bias, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits, Vectorized Random Number.\n");
    printf("The 0th value of vectorized Random Number is used.\n");

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

void MUXADD::Init(vector<float> param1, unsigned int param2, float param3, string param4)
{
    iProb = param1;
    wSize = param2;
    thdBias = param3;
    m_name = param4;

    iDim = (unsigned int)iProb.size();
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
        speed.resize(oDim);

        for (int i = 0; i < oDim; ++i)
        {
            wProb[i] = 0;
            theoProb[i] = 0;
            for (int j = 0; j < iDim; ++j)
            {
                theoProb[i] += iProb[j];
            }
            theoProb[i] /= iDim;
            speed[i] = 0;
        }
    #endif
}

void MUXADD::Calc(vector<char> param1, vector<unsigned int> param2)
{
    iBit = param1;
    randNum = param2;

    oBit[0] = iBit[randNum[0]];
        
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

vector<char> MUXADD::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<float> MUXADD::WProb()
    {
        return wProb;
    }

    vector<float> MUXADD::TheoProb()
    {
        return theoProb;
    }

    vector<float> MUXADD::WBias()
    {
        return wBias;
    }

    vector<unsigned int> MUXADD::Speed()
    {
        return speed;
    }
#endif
