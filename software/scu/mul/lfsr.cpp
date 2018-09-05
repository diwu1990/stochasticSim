#include "lfsr.hpp"

void LFSR::Init(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4, string param5)
{
    lfsrLen = param1;
    initState = (unsigned int)(param2 % ((unsigned int)pow(2,lfsrLen)));
    if (initState == 0)
    {
        printf("Error: Initial state is all-zero in LFSR class.\n");
    }
    polyIdx = param3;
    delay = param4;
    m_name = param5;
    outputLen = (unsigned int)(pow(2,lfsrLen)-1);
    outSeq.resize(outputLen);
    for (int i = 0; i < outputLen; ++i)
    {
        outSeq[i] = 0;
    }
    polyVal = 0;
}

LFSR::LFSR(){}

LFSR::~LFSR(){}

void LFSR::Report()
{
    printf("Current LFSR:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("LFSR Length:            %-5u\n", lfsrLen);
    printf("Initial State:          %-5u\n", initState);
    printf("Polynomial Index:       %-5u\n", polyIdx);
    printf("Output Sequence Length: %-5u\n", outputLen);
    printf("Output Sequence Delay:  %-5u\n", delay);
}

void LFSR::Help()
{
    // init params: lfsrLen, initState, polyIdx
    // the legal range of lfsrLen is 4~16
    // the legal range of initState is non-zero
    // the legal range of polyIdx is 2,6,6,18,16,48,60,100,100,100,100,100,100 for 4~16
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling LFSR Help. Following are instructions to LFSR Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the LFSR inst.\n");
    printf("Initial Parameters: LFSR Bit Length, Initial State, Polynomial Index, Output Delay, Instance Name.\n");
    printf("The legal range of LFSR Bit Length is 4~16.\n");
    printf("The legal value of Initial State is non-mod(zero).\n");
    printf("The legal range of Polynomial Index is (LFSR Bit Length -> Polynomial Index Range):\n");
    printf("( 4 -> 0~ 1);\n");
    printf("( 5 -> 0~ 5);\n");
    printf("( 6 -> 0~ 5);\n");
    printf("( 7 -> 0~17);\n");
    printf("( 8 -> 0~15);\n");
    printf("( 9 -> 0~47);\n");
    printf("(10 -> 0~59);\n");
    printf("(11 -> 0~99);\n");
    printf("(12 -> 0~99);\n");
    printf("(13 -> 0~99);\n");
    printf("(14 -> 0~99);\n");
    printf("(15 -> 0~99);\n");
    printf("(16 -> 0~99);\n");
    printf("The shift of output sequence.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing random number sequence from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current LFSR instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current LFSR instance.\n");

    printf("6. inst.PlyPrint() method:\n");
    printf("Print the value of polynomial of current LFSR instance.\n");

    printf("7. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current LFSR instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

unsigned int LFSR::SeqLen()
{
    return outputLen;
}

unsigned int LFSR::OutPly()
{
    return polyVal;
}

vector<unsigned int> LFSR::OutSeq()
{
    for (int i = 0; i < outputLen; ++i)
    {
        if (outSeq[i] == 0)
        {
            printf("Warning: SeqGen() in class LFSR is not executed to generate valid output random number sequence.\n");
            break;
        }
    }
    return outSeq;
}

void LFSR::SeqPrint()
{
    printf("Calling SeqPrint for LFSR instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < outputLen; ++i)
    {
        printf("%u,", outSeq[i]);
    }
    printf("\n");
}

void LFSR::PlyPrint()
{
    printf("Calling PlyPrint for LFSR instance: ");
    std::cout << m_name << std::endl;
    printf("%d\n", polyVal);
}

void LFSR::SeqGen()
{
    // every call on lfsr return the next random number
    // initState is the current input, and can't be all zeros
    // lfsrLen is the current lfsr length, and should be from 4 to 16
    // polyIdx is the index of poly for given input lfsrLen

    unsigned int lfsr = initState;

    static const unsigned int arr4[] = {TAP_4};
    static const unsigned int arr5[] = {TAP_5};
    static const unsigned int arr6[] = {TAP_6};
    static const unsigned int arr7[] = {TAP_7};
    static const unsigned int arr8[] = {TAP_8};
    static const unsigned int arr9[] = {TAP_9};
    static const unsigned int arr10[] = {TAP_10};
    static const unsigned int arr11[] = {TAP_11};
    static const unsigned int arr12[] = {TAP_12};
    static const unsigned int arr13[] = {TAP_13};
    static const unsigned int arr14[] = {TAP_14};
    static const unsigned int arr15[] = {TAP_15};
    static const unsigned int arr16[] = {TAP_16};
    
    int totalPoly = 0;
    int period = 0;
    vector<unsigned int> polyArray;

    if (lfsrLen == 4)
    {
        totalPoly = sizeof(arr4) / sizeof(arr4[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr4, arr4 + totalPoly);
    }
    else if (lfsrLen == 5)
    {
        totalPoly = sizeof(arr5) / sizeof(arr5[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr5, arr5 + totalPoly);
    }
    else if (lfsrLen == 6)
    {
        totalPoly = sizeof(arr6) / sizeof(arr6[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr6, arr6 + totalPoly);
    }
    else if (lfsrLen == 7)
    {
        totalPoly = sizeof(arr7) / sizeof(arr7[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr7, arr7 + totalPoly);
    }
    else if (lfsrLen == 8)
    {
        totalPoly = sizeof(arr8) / sizeof(arr8[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr8, arr8 + totalPoly);
    }
    else if (lfsrLen == 9)
    {
        totalPoly = sizeof(arr9) / sizeof(arr9[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr9, arr9 + totalPoly);
    }
    else if (lfsrLen == 10)
    {
        totalPoly = sizeof(arr10) / sizeof(arr10[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr10, arr10 + totalPoly);
    }
    else if (lfsrLen == 11)
    {
        totalPoly = sizeof(arr11) / sizeof(arr11[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr11, arr11 + totalPoly);
    }
    else if (lfsrLen == 12)
    {
        totalPoly = sizeof(arr12) / sizeof(arr12[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in file LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr12, arr12 + totalPoly);
    }
    else if (lfsrLen == 13)
    {
        totalPoly = sizeof(arr13) / sizeof(arr13[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in file LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr13, arr13 + totalPoly);
    }
    else if (lfsrLen == 14)
    {
        totalPoly = sizeof(arr14) / sizeof(arr14[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in file LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr14, arr14 + totalPoly);
    }
    else if (lfsrLen == 15)
    {
        totalPoly = sizeof(arr15) / sizeof(arr15[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in file LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr15, arr15 + totalPoly);
    }
    else if (lfsrLen == 16)
    {
        totalPoly = sizeof(arr16) / sizeof(arr16[0]);
        if (polyIdx >= totalPoly)
        {
            printf("Error: Input index of polynomial is too large (0~%d) in file LFSR class. \n",totalPoly-1);
        }
        polyArray.assign(arr16, arr16 + totalPoly);
    }
    else
    {
        printf("Error: Input lfsrLen exceeds the legal range (4~16) in LFSR class.\n");
    }

    unsigned int curPoly = polyArray[polyIdx];
    polyVal = curPoly;
    outSeq[period] = lfsr;
    do {
        unsigned int lsb = lfsr & 1;    /* get LSB, the single output bit */
        lfsr >>= 1;                  /* shift register */
        if (lsb == 1)                    /* If the output bit is 1, apply toggle mask. */
        {
            lfsr ^= curPoly;
        }
        ++period;
        if (period < outputLen)
        {
            outSeq[period] = lfsr;
        }
        // printf("%u\n", outSeq[period]);
    } while (lfsr != initState);
    // if (period = outputLen)
    // {
    //     printf("Equal length\n");
    // }
    // else
    // {
    //     printf("non-Equal length\n");
    // }
    vector<unsigned int> delayed(outputLen);
    for (int i = 0; i < outputLen; ++i)
    {
        delayed[i] = outSeq[(i+outputLen-delay) % outputLen];
    }
    for (int i = 0; i < outputLen; ++i)
    {
        outSeq[i] = delayed[i];
    }
}