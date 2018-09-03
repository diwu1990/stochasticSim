#include "apcadd.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

APCADD::APCADD(){}

APCADD::~APCADD(){}

void APCADD::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling APCADD Help. Following are instructions to APCADD Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the APCADD inst.\n");
    printf("Initial Parameters: Two Input Vectors.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the product of two input sequences.\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the calculated result.\n");

    printf("4. inst.OutPrint() method:\n");
    printf("Print the information of result.\n");

    printf("5. inst.InCC() method:\n");
    printf("Return the input crosscorrelation.\n");

    printf("6. inst.InProb() method:\n");
    printf("Return the input probability.\n");

    printf("7. inst.TheoProb() method:\n");
    printf("Return the theoretical output probability.\n");

    printf("8. inst.RealProb() method:\n");
    printf("Return the array of progressive output probability.\n");

    printf("9. inst.FinalRealProb() method:\n");
    printf("Return the final output probability.\n");

    printf("10. inst.ErrRate() method:\n");
    printf("Return the array of progressive output error rate.\n");

    printf("11. inst.FinalErrRate() method:\n");
    printf("Return the final output error rate.\n");

    printf("12. inst.SeqLen() method:\n");
    printf("Return the sequence length.\n");

    printf("13. inst.LowErrLen() method:\n");
    printf("Return the sequence length required to converge with less than 5 percent error rate.\n");

    printf("14. inst.PPStage() method:\n");
    printf("Return the pipline stages required by hardware.\n");

    printf("15. inst.Report() method:\n");
    printf("Report the current instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void APCADD::Init(vector<vector<unsigned int>> param1, vector<unsigned int> param2, unsigned int param3, string param4)
{
    inSeq = param1;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randNum = param2;
    bitLength = param3;
    m_name = param4;
    if ((unsigned int)inSeq.size() == (unsigned int)inProb.size() && (unsigned int)inSeq.size() == 16)
    {
        inDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Dimension is not 16.\n");
    }

    seqLength = (unsigned int)inSeq[0].size();
    for (int i = 0; i < inDim; ++i)
    {
        if ((unsigned int)inSeq[i].size() != seqLength)
        {
            printf("Error: Input Length is not the same.\n");
            break;
        }
    }

    theoProb = 0;
    for (int i = 0; i < inDim; ++i)
    {
        theoProb += inProb[i];
    }
    theoProb /= 16;
    if (theoProb == 0)
    {
        theoProb == 0.0001;
    }
    outSeq.resize(seqLength);
    realProb.resize(seqLength);
    errRate.resize(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        outSeq[i] = 0;
        realProb[i] = 0;
        errRate[i] = 0;
    }
    lowErrLen = 0;
    ppStage = 0;
}

void APCADD::Report()
{
    printf("Current APCADD:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Theoretical Probability:%f\n", theoProb);
}

void APCADD::Calc()
{
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC");
    inputCC.Calc();
    inCC = inputCC.OutCC();
    float oneCount = 0;

    vector<vector<unsigned int>> sumCnt(4);
    for (int i = 0; i < 4; ++i)
    {
        sumCnt[i].resize(seqLength);
    }

    vector<vector<vector<unsigned int>>> fa(4);
    // fa[i][0] => a
    // fa[i][1] => b
    // fa[i][2] => ci
    // fa[i][3] => sum
    // fa[i][4] => co
    for (int i = 0; i < 4; ++i)
    {
        fa[i].resize(5);
        for (int j = 0; j < 5; ++j)
        {
            fa[i][j].resize(seqLength);
        }
    }

    for (int i = 0; i < seqLength; ++i)
    {
        // approximate parallel counter
        // fulladder 0
        fa[0][0][i] = inSeq[0][i] | inSeq[1][i];
        fa[0][1][i] = inSeq[2][i] & inSeq[3][i];
        fa[0][2][i] = inSeq[4][i] | inSeq[5][i];
        fa[0][3][i] = (fa[0][0][i] + fa[0][1][i] + fa[0][2][i]) % 2;
        fa[0][4][i] = (fa[0][0][i] + fa[0][1][i] + fa[0][2][i]) >> 1;

        // fulladder 1
        fa[1][0][i] = inSeq[6][i] & inSeq[7][i];
        fa[1][1][i] = inSeq[8][i] | inSeq[9][i];
        fa[1][2][i] = inSeq[10][i] & inSeq[11][i];
        fa[1][3][i] = (fa[1][0][i] + fa[1][1][i] + fa[1][2][i]) % 2;
        fa[1][4][i] = (fa[1][0][i] + fa[1][1][i] + fa[1][2][i]) >> 1;

        // fulladder 2
        fa[2][0][i] = fa[0][3][i];
        fa[2][1][i] = fa[1][3][i];
        fa[2][2][i] = inSeq[12][i] | inSeq[13][i];
        fa[2][3][i] = (fa[2][0][i] + fa[2][1][i] + fa[2][2][i]) % 2;
        fa[2][4][i] = (fa[2][0][i] + fa[2][1][i] + fa[2][2][i]) >> 1;

        // fulladder 3
        fa[3][0][i] = fa[0][4][i];
        fa[3][1][i] = fa[1][4][i];
        fa[3][2][i] = fa[2][4][i];
        fa[3][3][i] = (fa[3][0][i] + fa[3][1][i] + fa[3][2][i]) % 2;
        fa[3][4][i] = (fa[3][0][i] + fa[3][1][i] + fa[3][2][i]) >> 1;

        sumCnt[3][i] = fa[3][4][i];
        sumCnt[2][i] = fa[3][3][i];
        sumCnt[1][i] = fa[2][3][i];
        sumCnt[0][i] = inSeq[14][i] & inSeq[15][i];

        if ((sumCnt[3][i]*8+sumCnt[2][i]*4+sumCnt[1][i]*2+sumCnt[0][i]*2) >= (randNum[i] >> (bitLength-4)))
        {
            outSeq[i] = 1;
        }
        else
        {
            outSeq[i] = 0;
        }


        // // accumulative parallel counter
        // unsigned int temp = 0;
        // for (int j = 0; j < inDim; ++j)
        // {
        //     temp += inSeq[j][i];
        // }
        // if (temp >= (randNum[i] >> (bitLength-4)))
        // {
        //     outSeq[i] = 1;
        // }
        // else
        // {
        //     outSeq[i] = 0;
        // }

        // // 16-1 mux
        // unsigned int temp = 0;
        // outSeq[i] = inSeq[randNum[i] >> (bitLength-4)][i];

    }

    unsigned int accuracyLength = 128;
    for (int i = 0; i < seqLength; ++i)
    {
        oneCount += outSeq[i];
        // printf("%f\n", oneCount);
        if (i < accuracyLength)
        {
            realProb[i] = (float)oneCount/(float)(i+1);
        }
        else
        {
            realProb[i] = (realProb[i-1]*(float)accuracyLength+outSeq[i]-outSeq[i-accuracyLength])/(float)accuracyLength;
        }
        errRate[i] = (theoProb - realProb[i])/theoProb;
    }
    // find the convergence point
    for (int i = 0; i < seqLength; ++i)
    {
        if (errRate[seqLength-1-i] > 0.05 || errRate[seqLength-1-i] < -0.05)
        {
            lowErrLen = seqLength-i;
            break;
        }
    }
}

vector<unsigned int> APCADD::OutSeq()
{
    return outSeq;
}

unsigned int APCADD::PPStage()
{
    return ppStage;
}

void APCADD::OutPrint()
{
    printf("Calling OutPrint for APCADD instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: %.3f\n", theoProb);
    printf("Final Probability: %.3f with Error Rate: %.3f\n", realProb[seqLength-1], errRate[seqLength-1]);
    printf("Low Error Length (5 percent approximation): %u\n", lowErrLen);
    // for (int i = 0; i < seqLength; ++i)
    // {
    //     printf("%.3f,", errRate[i]);
    // }
    // printf("\n");
    // printf("Output Probability:\n");
    // for (int i = 0; i < seqLength; ++i)
    // {
    //     printf("%.3f,", realProb[i]);
    // }
    // printf("\n");
}

vector<float> APCADD::InCC()
{
    return inCC;
}

vector<float> APCADD::InProb()
{
    return inProb;
}

float APCADD::TheoProb()
{
    return theoProb;
}

vector<float> APCADD::RealProb()
{
    return realProb;
}

float APCADD::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> APCADD::ErrRate()
{
    return errRate;
}

float APCADD::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int APCADD::SeqLen()
{
    return seqLength;
}

unsigned int APCADD::LowErrLen()
{
    return lowErrLen;
}


