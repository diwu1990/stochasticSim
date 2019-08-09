#include "oradd.hpp"
#include "perfsim.hpp"

void ORADD::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling ORADD Help. Following are instructions to ORADD Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Scaled ADD Endable, Window Size, Threshold for Window Bias, Unipolar Enable, Instance Name.\n");

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

void ORADD::Init(vector<float> param1, unsigned int param2, float param3, string param4)
{
    iProb = param1;
    wSize = param2;
    thdBias = param3;
    m_name = param4;

    unipolar = 1;

    iDim = (unsigned int)iProb.size();
    // iDim check, have to be power of 2
    if(ceil(log2(iDim)) != floor(log2(iDim)))
    {
        printf("Info: Input dimension of ORADD instantance is not power of 2.\n");
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
    parallel_cnt = 0;

    #ifdef PERFSIM
        oBS.resize(oDim);
        wProb.resize(oDim);
        theoProb.resize(oDim);
        wBias.resize(oDim);
        cTime.resize(oDim);

        for (int i = 0; i < oDim; ++i)
        {
            wProb[i] = 0;
            theoProb[i] = 0;
            for (int j = 0; j < iDim; ++j)
            {
                theoProb[i] += iProb[j];
            }
            if (theoProb[i] > 1)
            {
                theoProb[i] = 1;
            }
            else if (theoProb[i] < -1)
            {
                theoProb[i] = -1;
            }
            cTime[i] = 0;
        }
    #endif
}

void ORADD::Calc(vector<char> param1)
{
    iBit = param1;

    parallel_cnt = 0;
    for (int i = 0; i < iDim; ++i)
    {
        parallel_cnt += iBit[i];
        // printf("%d,", iBit[i]);
    }
    if (parallel_cnt > 0)
    {
        oBit[0] = 1;
    }
    else
    {
        oBit[0] = 0;
    }
    // printf("===> %d, %d\n", parallel_cnt, oBit[0]);

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
                if (unipolar == 0)
                {
                    wProb[i] = (float)totalSum[i]/iLen*2.0 - 1.0;
                }
                else
                {
                    wProb[i] = (float)totalSum[i]/iLen;
                }
            }
            else
            {
                for (int j = oBS[i].size() - wSize; j < oBS[i].size(); ++j)
                {
                    totalSum[i] += oBS[i][j];
                }
                if (unipolar == 0)
                {
                    wProb[i] = (float)totalSum[i]/iLen*2.0 - 1.0;
                }
                else
                {
                    wProb[i] = (float)totalSum[i]/iLen;
                }
            }
            wBias[i] = wProb[i] - theoProb[i];
            if ((wBias[i] > thdBias) || (wBias[i] < (0-thdBias)))
            {
                cTime[i] = iLen;
            }
        }
    #endif
}

vector<char> ORADD::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<vector<char>> ORADD::OutBS()
    {
        return oBS;
    }
    
    vector<float> ORADD::WProb()
    {
        return wProb;
    }

    vector<float> ORADD::TheoProb()
    {
        return theoProb;
    }

    vector<float> ORADD::WBias()
    {
        return wBias;
    }

    vector<unsigned int> ORADD::CTime()
    {
        return cTime;
    }
#endif
