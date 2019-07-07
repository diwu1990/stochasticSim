#include "cfmul.hpp"
#include "perfsim.hpp"

void CFMUL::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling CFMUL Help. Following are instructions to CFMUL Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters:\n");
    printf("1) Input Probability:           length-2 vector.\n");
    printf("                                In Correlation Free mode, the weight should be at input[1], but not input[0].\n");
    printf("2) Correlation Free Enable:     Enable Correlation Free mode with internal RNG(s): 1 for enable, 0 for disable.\n");
    printf("3) RNG Depth:                   RNG Depth.\n");
    printf("4) In-Stream Enable:            Enable in-stream calculation: 1 for enable, 0 for disable.\n");
    printf("                                If enbaled, input[1] in inst.Calc() is used;\n");
    printf("                                Otherwise, input[1] in inst.Calc() is not used; required bit is generated statically with internal RNGs.\n");
    printf("5) Accelerate Window:           Window within which to accelerate convergence for in-stream design. Better to set to half RNG Depth.\n");
    printf("6) Convergence Window:          Window Size to calculate convergence.\n");
    printf("7) Threshold of Bias:           Threshold to calculate convergence.\n");
    printf("8) Unipolar Enable:             Enable Unipolar data format, 1 for enable, 0 for disable.\n");
    printf("9) Instance Name:               Name of this instance, needing to be a string.\n");

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

void CFMUL::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, unsigned int param5, unsigned int param6, float param7, unsigned int param8, string param9)
{
    iProb = param1;
    cfree = param2;
    rngDepth = param3;
    inStream = param4;
    inSWindow = param5;
    wSize = param6;
    thdBias = param7;
    unipolar = param8;
    m_name = param9;

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
    
    upperBound = (unsigned int)pow(2,rngDepth)-1;
    halfBound = (unsigned int)pow(2,rngDepth-1);
    bound1 = halfBound - (unsigned int)pow(2,rngDepth-inSWindow);
    bound2 = halfBound + (unsigned int)pow(2,rngDepth-inSWindow);

    cnt = halfBound;
    cnt_inv = halfBound;

    if (cfree != 0)
    {
        // correlation free is enabled, then the internal rng is required.
        rngInst.Init(1,9,0,rngDepth,"incremental","rngInst");
        rngInst.SeqGen();

        if (inStream == 0)
        {
            vector<float> probVec(1);
            probVec[0] = iProb[1];
            vector<unsigned int> bitLengthVec(1);
            bitLengthVec[0] = rngDepth;
            num2bitMultiInst.Init(probVec, bitLengthVec, rngInst.OutSeq(), unipolar, "num2bitMultiInst");
            num2bitMultiInst.SeqGen();
        }
        
        rngIdx = 0;
        rngIdx_inv = 0;

        regenBit = 0;
        regenBit_inv = 0;
        lastBit = 1;
        lastBit_inv = 0;
    }
    else
    {
        // correlation free is disabled, cfmul is normal AND for 
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

void CFMUL::Calc(vector<char> param1)
{
    iBit = param1;

    if (cfree != 0)
    {
        // correlation free is enabled, then the internal rng is required.
        // always regenerate input[1]
        if (inStream == 0)
        {
            // in stream mode is disabled, input[1] is static. All bits of input[1] are generated via static input value with internal RNG.
            if (unipolar == 0)
            {
                // bipolar mode, XNOR gate
                // a normal AND gate and an inv AND gate into an OR gate
                oBit[0] = (iBit[0] & num2bitMultiInst.OutSeq()[0][rngIdx]) | ((1-iBit[0]) & (1-num2bitMultiInst.OutSeq()[0][rngIdx_inv]));

                rngIdx += iBit[0];
                rngIdx %= (upperBound+1);
                rngIdx_inv += (1 - iBit[0]);
                rngIdx_inv %= (upperBound+1);
            }
            else
            {
                // unipolar mode, AND gate
                oBit[0] = iBit[0] & num2bitMultiInst.OutSeq()[0][rngIdx];

                rngIdx += iBit[0];
                rngIdx %= (upperBound+1);
            }
        }
        else
        {
            // in stream mode is enabled, input[1] is dynamically regenrated.
            if (unipolar == 0)
            {
                // bipolar mode, XNOR gate
                // normal AND gate
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
                rngIdx %= (upperBound+1);

                // inv AND gate
                if (cnt_inv > bound1 && cnt_inv < bound2)
                {
                    if (iBit[1] == 0 & lastBit == 0)
                    {
                        cnt_inv += 2;
                    }
                    else if(iBit[1] == 1 & lastBit == 1)
                    {
                        cnt_inv -= 2;
                    }
                }
                else
                {
                    if (iBit[1] == 0 && cnt_inv < upperBound)
                    {
                        cnt_inv += 1;
                    }
                    else if(iBit[1] == 1 && cnt_inv > 0)
                    {
                        cnt_inv -= 1;
                    }
                }
                lastBit_inv = (1 - iBit[1]);
                rngIdx_inv += (1 - iBit[0]);
                rngIdx_inv %= (upperBound+1);

                if (cnt > rngInst.OutSeq()[0][rngIdx])
                {
                    regenBit = 1;
                }
                else
                {
                    regenBit = 0;
                }

                if (cnt_inv > rngInst.OutSeq()[0][rngIdx_inv])
                {
                    regenBit_inv = 1;
                }
                else
                {
                    regenBit_inv = 0;
                }

                oBit[0] = (iBit[0] & regenBit) | ((1-iBit[0]) & regenBit_inv);
            }
            else
            {
                // unipolar mode, AND gate
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

                if (cnt > rngInst.OutSeq()[0][rngIdx])
                {
                    regenBit = 1;
                }
                else
                {
                    regenBit = 0;
                }
                oBit[0] = iBit[0] & regenBit;
            }
        }
    }
    else
    {
        // correlation free is disabled, cfmul is normal AND for unipolar, and XNOR for bipolar.
        if (unipolar == 0)
        {
            oBit[0] = 1 - (iBit[0] ^ iBit[1]);
        }
        else
        {
            oBit[0] = iBit[0] & iBit[1];
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
    // printf("inBit0(%d), inBit1(%d), cnt(%d), rngIdx(%d), random(%d): oBit(%d), wProb(%f)\n", iBit[0], iBit[1], cnt, rngIdx, rngInst.OutSeq()[0][rngIdx], oBit[0], wProb[0]);
    // printf("inBit0(%d), inBit1(%d), cnt_inv(%d), rngIdx_inv(%d), random(%d): oBit(%d), wProb(%f)\n", iBit[0], iBit[1], cnt_inv, rngIdx_inv, rngInst.OutSeq()[0][rngIdx_inv], oBit[0], wProb[0]);
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
