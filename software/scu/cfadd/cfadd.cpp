#include "cfadd.hpp"
#include "perfsim.hpp"

void CFADD::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling CFADD Help. Following are instructions to CFADD Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Window Size, Threshold for Window Bias, Unipolar Enable, Instance Name.\n");

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

void CFADD::Init(vector<float> param1, unsigned int param2, float param3, unsigned int param4, string param5)
{
    iProb = param1;
    wSize = param2;
    thdBias = param3;
    unipolar = param4;
    m_name = param5;

    iDim = (unsigned int)iProb.size();
    // iDim check, have to be power of 2
    if(ceil(log2(iDim)) != floor(log2(iDim)))
    {
        printf("Info: Input dimension of CFADD instantance is not power of 2.\n");
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
    accumulator = 0;
    upper = iDim;

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
            theoProb[i] /= iDim;
            cTime[i] = 0;
        }
    #endif
}

void CFADD::Calc(vector<char> param1)
{
    iBit = param1;

    parallel_cnt = 0;
    for (int i = 0; i < iDim; ++i)
    {
        parallel_cnt += iBit[i];
        // printf("%d,", iBit[i]);
    }
    // printf("===>%d, %d\n", parallel_cnt, accumulator);
    accumulator += (parallel_cnt%iDim);
    if (parallel_cnt >= upper)
    {
        oBit[0] = 1;
    }
    else
    {
        if (accumulator >= upper)
        {
            oBit[0] = 1;
            accumulator = (accumulator%iDim);
        }
        else
        {
            oBit[0] = 0;
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

vector<char> CFADD::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<vector<char>> CFADD::OutBS()
    {
        return oBS;
    }
    
    vector<float> CFADD::WProb()
    {
        return wProb;
    }

    vector<float> CFADD::TheoProb()
    {
        return theoProb;
    }

    vector<float> CFADD::WBias()
    {
        return wBias;
    }

    vector<unsigned int> CFADD::CTime()
    {
        return cTime;
    }
#endif
