#include "autocorrelation.hpp"

void AutoCorrelation::Init(vector<unsigned int> param1, unsigned int param2, float param3, string param4)
{
    bitVec = param1;
    offset = param2;
    expectation = param3;
    m_name = param4;
}

AutoCorrelation::AutoCorrelation()
{

}

AutoCorrelation::~AutoCorrelation()
{
        
}

void AutoCorrelation::Report()
{
    printf("Current AutoCorrelation:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Expectation:            %-5f\n", expectation);
    printf("Shift Offset:           %-5u\n", offset);
}

void AutoCorrelation::Help()
{
    // init params: lfsrLen, initState, polyIdx
    // the legal range of lfsrLen is 4~16
    // the legal range of initState is non-zero
    // the legal range of polyIdx is 2,6,6,18,16,48,60,100,100,100,100,100,100 for 4~16
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("\nCalling AutoCorrelation Help. Following are instructions to AutoCorrelation Instance Usage:\n");
    printf("\n1. inst.Init() method:\n");
    printf("Configure the AutoCorrelation inst.\n");
    printf("Initial Parameters: Bit Vector, Auto-Correlation Offset, Orignal Expectation of Bit Vector, Instance Name.\n");

    printf("\n2. inst.Calc() method:\n");
    printf("Calculate the Auto-Correlation of input bit vector from inst.Init().\n");

    printf("\n3. inst.OutAC() method:\n");
    printf("Return the calculated Auto-Correlation from inst.CalcAC().\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");

}

float AutoCorrelation::OutAC()
{
    return outAC;
}

void AutoCorrelation::Calc()
{
    // bitVec is the input stochastic bit array, which is to be calculated for auto-correlation
    // offset is the shifting offset for the input array
    // expectation is the desired value of input bit array
    // outAC is the value of auto-correlation for input array

    float denominator = 0;
    for (int i = 0; i < (unsigned int)bitVec.size(); ++i)
    {
        if (bitVec[i] != 1)
        {
            if (bitVec[i] != 0)
            {
                printf("Error: Input bit array is not binary value in class AutoCorrelation.\n");
            }
        }
        denominator = denominator + (bitVec[i] - expectation) * (bitVec[i] - expectation);
    }

    float numerator = 0;
    for (int i = 0; i < ((unsigned int)bitVec.size() - offset); ++i)
    {
        numerator = numerator + (bitVec[i] - expectation) * (bitVec[i+offset] - expectation);
    }

    outAC = numerator/denominator;
}
