#include "cfmul.hpp"
#include "perfsim.hpp"

void CFMUL::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling CFMUL Help. Following are instructions to CFMUL Instance Usage:\n");

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

void CFMUL::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, float param5, unsigned int param6, string param7)
{
    iProb = param1;
    depthSync = param2;
    shift = param3;
    wSize = param4;
    thdBias = param5;
    unipolar = param6;
    m_name = param7;

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
    bound1 = halfBound - (unsigned int)pow(2,depthSync-shift);
    bound2 = halfBound + (unsigned int)pow(2,depthSync-shift);
    cnt = halfBound;

    rngInst.Init(1,9,0,depthSync,"incremental","rngInst");
    // rngInst.Init(1,5,0,depthSync,"random","rngInst");
    rngInst.SeqGen();
    rngIdx = 0;

    regenBit = 0;
    lastBit = 1;

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

    if (cnt > bound1 && cnt < bound2)
    {
        if (iBit[1] == 1 & lastBit == 1)
        {
            cnt += 2;
        }
        else if(iBit[1] == 0 & lastBit == 0)
        {
            cnt -= 2;
        }
        // else if (iBit[1] == 1 & lastBit == 0)
        // {
        //     cnt += 1;
        // }
        // else if(iBit[1] == 0 & lastBit == 1)
        // {
        //     cnt -= 1;
        // }
    }
    else
    {
        if (iBit[1] == 1 && cnt < upperBound)
        {
            cnt += 1;
        }
        else if(iBit[1] == 0 && cnt > 0)
        {
            cnt -= 1;
        }
    }

    lastBit = iBit[1];

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
    // printf("inBit0(%d), inBit1(%d), cnt(%d), rngIdx(%d), random(%d): oBit(%d), wProb(%f)\n", iBit[0], iBit[1], cnt, rngIdx, rngInst.OutSeq()[0][rngIdx], oBit[0], wProb[0]);
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
