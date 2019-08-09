#include "cfadd.hpp"
#include "perfsim.hpp"

void CFADD::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling CFADD Help. Following are instructions to CFADD Instance Usage:\n");

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

void CFADD::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, float param5, unsigned int param6, string param7)
{
    iProb = param1;
    scaled = param2;
    depthSync = param3;
    wSize = param4;
    thdBias = param5;
    unipolar = param6;
    m_name = param7;

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
    signed_accumulator = 0;
    offset_accumulator = 0;
    theoOutOne = 0;
    upper = iDim;
    if (unipolar == 1)
    {
        offset = 0;
    }
    else
    {
        offset = (int)(iDim-1);
    }
    outOneCnt = 0;

    // printf("%d, %d, %d\n", signed_acc_upper, signed_acc_lower, offset);

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
            if (scaled == 1)
            {
                theoProb[i] /= iDim;
            }
            else
            {
                if (theoProb[i] > 1)
                {
                    theoProb[i] = 1;
                }
                else if (theoProb[i] < -1)
                {
                    theoProb[i] = -1;
                }
            }
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

    if (scaled == 1)
    {
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
    }
    else
    {
        signed_accumulator += parallel_cnt*2;
        offset_accumulator += offset;
        theoOutOne = signed_accumulator - offset_accumulator;
        if (theoOutOne > outOneCnt)
        {
            oBit[0] = 1;
        }
        else
        {
            oBit[0] = 0;
        }
        outOneCnt += (oBit[0]*2);
        // printf("===>%d, %d, %d, %d, %d, %d\n", parallel_cnt, signed_accumulator, offset_accumulator, theoOutOne, outOneCnt, oBit[0]);
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
