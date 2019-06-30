#include "cfmul.hpp"
#include "perfsim.hpp"

void CFMUL::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling CFMUL Help. Following are instructions to CFMUL Instance Usage:\n");

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

void CFMUL::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, float param5, string param6)
{
    iProb = param1;
    depthSync = param2;
    unipolar = param3;
    wSize = param4;
    thdBias = param5;
    m_name = param6;

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
    
    upperBound = (unsigned int)pow(2,depthSync)-1;
    halfBound = (unsigned int)pow(2,depthSync-1);
    cnt = halfBound;

    rngInst.Init(1,1,0,depthSync,"random","rngInst");
    rngInst.SeqGen();
    rngIdx = 0;

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

void CFMUL::Calc(vector<char> param1)
{
    iBit = param1;

    if (iBit[1] == 1)
    {
        if (cnt < upperBound)
        {
            cnt = cnt + 1;
        }
    }
    else
    {
        if (cnt > 0)
        {
            cnt = cnt - 1;
        }
    }

    rngIdx += iBit[0];
    

    if (cnt >= rngInst.OutSeq()[0][rngIdx])
    {
        regenBit = 1;
    }
    else
    {
        regenBit = 0;
    }

    if (unipolar == 0)
    {
        oBit[0] = 1 - (iBit[0] ^ regenBit);
    }
    else
    {
        oBit[0] = iBit[0] & regenBit;
    }
    // printf("inBit0(%d), inBit0(%d), cnt(%d), rngIdx(%d), random(%d): oBit(%d)\n", iBit[0], iBit[1], cnt, rngIdx, rngInst.OutSeq()[0][rngIdx], oBit[0]);

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

vector<char> CFMUL::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<vector<char>> CFMUL::OutBS()
    {
        return oBS;
    }
    
    vector<float> CFMUL::WProb()
    {
        return wProb;
    }

    vector<float> CFMUL::TheoProb()
    {
        return theoProb;
    }

    vector<float> CFMUL::WBias()
    {
        return wBias;
    }

    vector<unsigned int> CFMUL::CTime()
    {
        return cTime;
    }
#endif
