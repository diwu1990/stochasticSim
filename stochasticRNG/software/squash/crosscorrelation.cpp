#include "crosscorrelation.hpp"

void CrossCorrelation::Init(vector<vector<unsigned int>> param1, float param2, string param3)
{
    bitVec = param1;
    thresholdGood = param2;
    m_name = param3;
    bitVecDim = (unsigned int)bitVec.size();
    bitVecLen = (unsigned int)bitVec[0].size();
    outCC.resize(bitVecDim*(bitVecDim-1)/2);
    for (int i = 1; i < bitVecDim; ++i)
    {
        if (bitVecLen != (unsigned int)bitVec[i].size())
        {
            printf("Error: Input bit vectors are of different length.\n");
            break;
        }
    }
}

CrossCorrelation::CrossCorrelation(){}
CrossCorrelation::~CrossCorrelation(){}

void CrossCorrelation::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling CrossCorrelation Help. Following are instructions to CrossCorrelation Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the CrossCorrelation inst.\n");
    printf("Initial Parameters: Bit Vector Array, Threshold To Be Print, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the Cross-Correlation of input bit vector from inst.Init().\n");

    printf("3. inst.OutCC() method:\n");
    printf("Return the calculated Cross-Correlation from inst.CalcCC().\n");

    printf("4. inst.MaxCC() method:\n");
    printf("Return the max calculated Cross-Correlation from inst.CalcCC().\n");

    printf("5. inst.MinCC() method:\n");
    printf("Return the min calculated Cross-Correlation from inst.CalcCC().\n");

    printf("6. inst.CCPrint() method:\n");
    printf("Print the calculated Cross-Correlation from inst.CalcCC().\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");

}

vector<float> CrossCorrelation::OutCC()
{
    return outCC;
}

float CrossCorrelation::MaxCC()
{
    return maxCC;
}

float CrossCorrelation::MinCC()
{
    return minCC;
}

void CrossCorrelation::CCPrint()
{
    printf("Calling CCPrint for CrossCorrelation instance: ");
    std::cout << m_name << std::endl;
    unsigned int CCIndex = 0;
    unsigned int goodNum = 0;
    for (int i = 0; i < bitVecDim; ++i)
    {
        for (int j = 0; j < bitVecDim; ++j)
        {
            if (i < j)
            {
                if (outCC[CCIndex] <= thresholdGood && outCC[CCIndex] >= -thresholdGood)
                {
                    printf("(%5d,%5d) => %f\n", i,j,outCC[CCIndex]);
                    ++goodNum;
                }
                CCIndex = CCIndex + 1;
            }
        }
    }
    printf("Number of All CrossCorrelation: %u\n", CCIndex);
    printf("Number of Low CrossCorrelation: %u\n", goodNum);
    printf("Max CC Index: %u, %u; Value: %f\n", maxIdx1, maxIdx2, maxCC);
    printf("Min CC Index: %u, %u; Value: %f\n", minIdx1, minIdx2, minCC);
}

void CrossCorrelation::Report()
{
    printf("Current CrossCorrelation:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Number of Seqsences:    %u\n", bitVecDim);
    printf("Seqsence Length:        %u\n", bitVecLen);
}

void CrossCorrelation::Calc()
{
    // bitVecX is the input stochastic bit array X, which is the first array to be calculated for cross-correlation
    // bitVecY is the input stochastic bit array Y, which is the second array to be calculated for cross-correlation
    // outCC is the value of auto-correlation for input array

    unsigned int CCIndex = 0;
    maxCC = -2;
    minCC = 2;
    for (int i = 0; i < bitVecDim; ++i)
    {
        for (int j = 0; j < bitVecDim; ++j)
        {
            if (i < j)
            {
                vector<unsigned int> bitVecX(bitVecLen);
                bitVecX = bitVec[i];
                vector<unsigned int> bitVecY(bitVecLen);
                bitVecY = bitVec[j];

                int a=0;
                int b=0;
                int c=0;
                int d=0;

                for (int i = 0; i < bitVecLen; ++i)
                {
                    if (bitVecX[i] == 1 && bitVecY[i] == 1)
                    {
                        a = a+1;
                    }
                    else if (bitVecX[i] == 1 && bitVecY[i] == 0)
                    {
                        b = b+1;
                    }
                    else if (bitVecX[i] == 0 && bitVecY[i] == 1)
                    {
                        c = c+1;
                    }
                    else if (bitVecX[i] == 0 && bitVecY[i] == 0)
                    {
                        d = d+1;
                    }
                    else
                    {
                        printf("Error: Input bit array is not binary value in class CrossCorrelation.\n");
                    }
                }

                // printf("%d%d%d%d\n", a,b,c,d);
                if (a*d > b*c)
                {
                    int ab = a+b;
                    int ac = a+c;
                    if ((float)(bitVecLen*min(ab,ac)-(a+b)*(a+c)) != 0)
                    {
                        outCC[CCIndex] = (float)(a*d-b*c)/(float)(bitVecLen*min(ab,ac)-(a+b)*(a+c));
                    }
                    else
                    {
                        outCC[CCIndex] = 1;
                    }
                }
                else
                {
                    int ad = a-d;
                    if ((float)((a+b)*(a+c)-(bitVecLen)*min(ad,0)) != 0)
                    {
                        outCC[CCIndex] = (float)(a*d-b*c)/(float)((a+b)*(a+c)-(bitVecLen)*min(ad,0));
                    }
                    else
                    {
                        outCC[CCIndex] = 1;
                    }
                }
                if (outCC[CCIndex] > maxCC)
                {
                    maxCC = outCC[CCIndex];
                    maxIdx1 = i;
                    maxIdx2 = j;
                }
                if (outCC[CCIndex] < minCC)
                {
                    minCC = outCC[CCIndex];
                    minIdx1 = i;
                    minIdx2 = j;
                }
                // maxCC = max(outCC[CCIndex], maxCC);
                // minCC = min(outCC[CCIndex], minCC);
                // printf("%u\n", CCIndex);
                CCIndex = CCIndex + 1;
            }
        }
    }
}
