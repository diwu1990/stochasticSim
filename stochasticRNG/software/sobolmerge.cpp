#include "sobolmerge.hpp"

SOBOLMerge::SOBOLMerge(){}

SOBOLMerge::~SOBOLMerge(){}

void SOBOLMerge::Help(){
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SOBOLMerge Help. Following are instructions to SOBOLMerge Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SOBOLMerge inst.\n");
    printf("Initial Parameters: Vector of SOBOL Memory Value, Instance Name.\n");

    printf("2. inst.MemGen() method:\n");
    printf("Generate the merged memory vectors.\n");

    printf("3. inst.OutMem() method:\n");
    printf("Return the merged memory vectors. The memory is a merged vector of two sobol vector arrays.\n");
    printf("The high bits of low index vectors are for first dimension.\n");
    printf("The low bits of high index vectors are for second dimension.\n");
    printf("Example of 8 bits:\n");
    printf("s0v0_0 s0v1_1 s0v2_2 s0v3_3 s0v4_4 s0v5_5 s0v6_6 s0v7_7\n");
    printf("s1v7_7 s0v1_0 s0v2_1 s0v3_2 s0v4_3 s0v5_4 s0v6_5 s0v7_6\n");
    printf("s1v7_6 s1v6_6 s0v2_0 s0v3_1 s0v4_2 s0v5_3 s0v6_4 s0v7_5\n");
    printf("s1v7_5 s1v6_5 s1v5_5 s0v3_0 s0v4_1 s0v5_2 s0v6_3 s0v7_4\n");
    printf("s1v7_4 s1v6_4 s1v5_4 s1v4_4 s0v4_0 s0v5_1 s0v6_2 s0v7_3\n");
    printf("s1v7_3 s1v6_3 s1v5_3 s1v4_3 s1v3_3 s0v5_0 s0v6_1 s0v7_2\n");
    printf("s1v7_2 s1v6_2 s1v5_2 s1v4_2 s1v3_2 s1v2_2 s0v6_0 s0v7_1\n");
    printf("s1v7_1 s1v6_1 s1v5_1 s1v4_1 s1v3_1 s1v2_1 s1v1_1 s0v7_0\n");
    printf("s1v7_0 s1v6_0 s1v5_0 s1v4_0 s1v3_0 s1v2_0 s1v1_0 s1v0_0\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current SOBOLMerge instance.\n");

    printf("5. inst.MemPrint() method:\n");
    printf("Print the merged memory vectors.\n");

    printf("6. inst.MemLen() method:\n");
    printf("Return the length of the sequence of current SOBOLMerge instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void SOBOLMerge::Init(vector<vector<unsigned int>> param1, unsigned int param2, string param3)
{
    inDirVec = param1;
    bitLength = param2;
    m_name = param3;
    inDim = (unsigned int)inDirVec.size();
    if (inDim != 2)
    {
        printf("Error: Input directional vectors has incorrect number of dimensions (Supposed to be 2).\n");
    }
    inLen = (unsigned int)inDirVec[0].size();
    if (inLen != (unsigned int)inDirVec[1].size())
    {
        printf("Error: Input directional vector length are not the same.\n");
    }
    outMemVec.resize(inLen);
}

void SOBOLMerge::Report()
{
    printf("Current SOBOLMerge:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Input Dimension:        %-5u\n", inDim);
    printf("Input Length:           %-5u\n", inLen);
    printf("Input Bit Length:       %-5u\n", bitLength);
}

void SOBOLMerge::MemGen()
{
    for (int i = 0; i < inLen; ++i)
    {
        outMemVec[i] = (inDirVec[0][i] << (inLen-i)) + inDirVec[1][inLen-1-i];
    }
}

void SOBOLMerge::MemPrint()
{
    printf("Calling MemPrint for SOBOLMerge instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < inLen; ++i)
    {
        printf("%-5u, ", outMemVec[i]);
    }
    printf("\n");
}

void SOBOLMerge::VecPrint()
{
    printf("Calling VecPrint for SOBOLMerge instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < inDim; ++i)
    {
        printf("Dimension %d: \n",i);
        for (int j = 0; j < inLen; ++j)
        {
            printf("%-5u, ", inDirVec[i][j]);
        }
        printf("\n");
    }
}

vector<unsigned int> SOBOLMerge::OutMem()
{
    return outMemVec;
}

unsigned int SOBOLMerge::MemLen()
{
    return inLen;
}
