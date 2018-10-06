#include "cordivkernelsrout.hpp"
#include "perfsim.hpp"

void CORDIVKERNELSROUT::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling CORDIVKERNELSROUT Help. Following are instructions to CORDIVKERNELSROUT Instance Usage:\n");

    printf("1. inst.Init() method:\n");
    printf("Configure the current inst.\n");
    printf("Parameters: Input Probability, Depth of Synchronizer using Regeneration, Depth of CORDIV Kernel, Window Size, Threshold for Window Bias, Instance Name.\n");
    printf("Input 0 is dividend, and input 1 is divisor; divisor should always be larger than dividend.\n");
    printf("Depth of Synchronizer is not used.");
    printf("Depth of CORDIV Kernel is suggested to be 2.");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the result bit.\n");
    printf("Parameters: Vectorized Input Bits, Vectorized Random Number.\n");
    printf("Input bit 0 is dividend, and input bit 1 is divisor.\n");
    printf("The 0th value of Vectorized Random Number is for the Synchronizer and not used.\n");
    printf("The 1th value of Vectorized Random Number is for the CORDIV Kernel.\n");

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

void CORDIVKERNELSROUT::Init(vector<float> param1, unsigned int param2, unsigned int param3, unsigned int param4, float param5, string param6)
{
    iProb = param1;
    depthSync = param2;
    depth = param3;
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

    traceReg.resize(depth);
    for (int i = 0; i < depth; ++i)
    {
        traceReg[i] = i%2;
    }

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
            theoProb[i] = iProb[0]/iProb[1];
            speed[i] = 0;
        }
    #endif
}

void CORDIVKERNELSROUT::Calc(vector<char> param1, vector<unsigned int> param2)
{
    iBit = param1;
    randNum = param2;

    // *****************************************************************************
    // shift reg based for correlation
    // *****************************************************************************

    if (iBit[1] == 1)
    {
        for (int index = 0; index < depth-1; ++index)
        {
            traceReg[index] = traceReg[index+1];
        }
        traceReg[depth-1] = iBit[0];
    }
    oBit[0] = traceReg[randNum[1]];
    
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

vector<char> CORDIVKERNELSROUT::OutBit()
{
    return oBit;
}

#ifdef PERFSIM
    vector<float> CORDIVKERNELSROUT::WProb()
    {
        return wProb;
    }

    vector<float> CORDIVKERNELSROUT::TheoProb()
    {
        return theoProb;
    }

    vector<float> CORDIVKERNELSROUT::WBias()
    {
        return wBias;
    }

    vector<unsigned int> CORDIVKERNELSROUT::Speed()
    {
        return speed;
    }
#endif
