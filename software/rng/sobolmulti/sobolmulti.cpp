#include "sobol.hpp"
#include "sobolmulti.hpp"

void SOBOLMulti::Init(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4, string param5, string param6)
{
    dimNum = param1;
    dimIndex = param2;
    if (dimIndex < 1)
    {
        printf("Error: Input Dimensional Index is invalid. (should be larger than 1)\n");
    }
    delayIndex = param3;
    sobolLen = param4;
    outputLen = (unsigned int)(pow(2,sobolLen));
    mode = param5;
    if (mode != "delayed" && mode != "incremental" && mode != "random")
    {
        printf("Error: Input mode is invalid. (delayed or incremental or random)\n");
    }

    m_name = param6;
    if (dimNum < 1 || dimNum > 1111)
    {
        printf("Error: Input dimension index exceeds the legal range (1~1111) in SOBOLMulti class.\n");
    }
    if (sobolLen < 4 || sobolLen > 16)
    {
        printf("Error: Input sobolLen exceeds the legal range (4~16) in SOBOLMulti class.\n");
    }
    outSeq.resize(dimNum);
    dirVec.resize(dimNum);
    dirMem.resize(dimNum);
    for (int i = 0; i < dimNum; ++i)
    {
        outSeq[i].resize(outputLen);
        dirVec[i].resize(sobolLen);
        dirMem[i].resize(sobolLen);
    }
}

SOBOLMulti::SOBOLMulti(){}
SOBOLMulti::~SOBOLMulti(){}
void SOBOLMulti::Report()
{
    printf("Current SOBOLMulti:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("SOBOLMulti Length:      %-5u\n", sobolLen);
    printf("Dimension Number:       %-5u\n", dimNum);
    printf("Delay Index:            %-5u\n", delayIndex);
    printf("Output Sequence Length: %-5u\n", outputLen);
}

void SOBOLMulti::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SOBOLMulti Help. Following are instructions to SOBOLMulti Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SOBOLMulti inst.\n");
    printf("Initial Parameters: Number of Dimension, Index of Initial Dimension, Delay Index, Sobol Bit Length, Sobol Mode, Instance Name.\n");
    printf("1) Number of Dimension: Number of Output Sequences.\n");
    printf("2) Index of Initial Dimension: The starting index of output dimension.\n");
    printf("3) Index of SOBOL delay: The interval index of output delay.\n");
    printf("4) Sobol Bit Length: Length of Sobol vECTORS.\n");
    printf("5) Sobol Mode: Define the relationship between each sobol sequence, delayed means the each sequence is delayed one cycle from another, incremental means sobol sequence of different dimension with same delay.\n");
    printf("6) Instance Name: Name of current SOBOLMulti Instance.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing random number sequence from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current SOBOLMulti instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current SOBOLMulti instance.\n");

    printf("6. inst.VecPrint() method:\n");
    printf("Print the initial directional vector sequence of current SOBOLMulti instance.\n");

    printf("7. inst.MemPrint() method:\n");
    printf("Print the initial directional memory sequence of current SOBOLMulti instance.\n");

    printf("8. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current SOBOLMulti instance.\n");

    printf("9. inst.VecMem() method:\n");
    printf("Return the initial directional vector sequence of current SOBOLMulti instance.\n");

    printf("10. inst.DirMem() method:\n");
    printf("Return the initial directional memory sequence of current SOBOLMulti instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

unsigned int SOBOLMulti::SeqLen()
{
    return outputLen;
}

vector<vector<unsigned int>> SOBOLMulti::OutSeq()
{
    return outSeq;
}

vector<vector<unsigned int>> SOBOLMulti::DirMem()
{
    return dirMem;
}

vector<vector<unsigned int>> SOBOLMulti::DirVec()
{
    return dirVec;
}

void SOBOLMulti::SeqPrint()
{
    printf("Calling SeqPrint for SOBOLMulti instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < dimNum; ++i)
    {
        printf("%dth Dimension: ", i);
        for (int j = 0; j < outputLen; ++j)
        {
            printf("%u,", outSeq[i][j]);
        }
        printf("\n");
    }
}

void SOBOLMulti::VecPrint()
{
    printf("Calling VecPrint for SOBOLMulti instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < dimNum; ++i)
    {
        printf("%dth Dimension: ", i);
        for (int j = 0; j < sobolLen; ++j)
        {
            printf("%u,", dirVec[i][j]);
        }
        printf("\n");
    }
}

void SOBOLMulti::MemPrint()
{
    printf("Calling MemPrint for SOBOLMulti instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < dimNum; ++i)
    {
        printf("%dth Dimension: ", i);
        for (int j = 0; j < sobolLen; ++j)
        {
            printf("%u,", dirMem[i][j]);
        }
        printf("\n");
    }
}

void SOBOLMulti::SeqGen()
{
    if (mode == "delayed")
    {
        for (int i = 0; i < dimNum; ++i)
        {
            SOBOL sobolInst;
            sobolInst.Init(sobolLen,dimIndex,i*delayIndex,"sobolInst");
            sobolInst.SeqGen();
            outSeq[i] = sobolInst.OutSeq();
            dirVec[i] = sobolInst.DirVec();
            dirMem[i] = sobolInst.DirMem();
        }
    }
    else if (mode == "incremental")
    {
        for (int i = 0; i < dimNum; ++i)
        {
            SOBOL sobolInst;
            sobolInst.Init(sobolLen,i+dimIndex,0,"sobolInst");
            sobolInst.SeqGen();
            outSeq[i] = sobolInst.OutSeq();
            dirVec[i] = sobolInst.DirVec();
            dirMem[i] = sobolInst.DirMem();
        }
    }
    else if (mode == "random")
    {
        srand(time(NULL));
        for (int i = 0; i < dimNum; ++i)
        {
            SOBOL sobolInst;
            sobolInst.Init(sobolLen,(unsigned int)rand()%100+1,(unsigned int)rand()%(unsigned int)pow(2,sobolLen),"sobolInst");
            sobolInst.SeqGen();
            outSeq[i] = sobolInst.OutSeq();
            dirVec[i] = sobolInst.DirVec();
            dirMem[i] = sobolInst.DirMem();
        }
    }
}

