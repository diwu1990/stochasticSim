#include "seqsearch.hpp"
#include "randNum2BitMulti.hpp"
#include "crosscorrelation.hpp"

void SeqSearch::Init(
    vector<vector<unsigned int>> param1, 
    unsigned int param2, 
    float param3,
    unsigned int param4, 
    string param5)
{
    inSeq = param1;
    bitLength = param2;
    threshold = param3;
    unipolar = param3;
    m_name = param5;
    inDim = (unsigned int)inSeq.size();
    inLen = (unsigned int)inSeq[0].size();
    for (int i = 1; i < inDim; ++i)
    {
        if (inLen != (unsigned int)inSeq[i].size())
        {
            printf("Error: Input Sequences have different length.\n");
        }
    }
    combNum = 0;
    outComb.resize(2);
    msCC = 0;
    outComb[0].push_back(0);
    outComb[1].push_back(0);
    totalComb = inDim*(inDim-1)/2;
    meansquareCC.resize(totalComb);
}

SeqSearch::SeqSearch(){}
SeqSearch::~SeqSearch(){}
void SeqSearch::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SeqSearch Help. Following are instructions to SeqSearch Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SeqSearch inst.\n");
    printf("Initial Parameters: Input Random Number Vector, Threshold of Cross-Correlation, Instance Name.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the combination of input vectors with low discrepency from inst.Init().\n");

    printf("3. inst.OutComb() method:\n");
    printf("Return the calculated combination in vector format.\n");

    printf("4. inst.CombNum() method:\n");
    printf("Return the number of combinations.\n");

    printf("5. inst.CombPrint() method:\n");
    printf("Print the calculated combination.\n");

    printf("6. inst.Report() method:\n");
    printf("Report the configuration of current SeqSearch instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

vector<vector<unsigned int>> SeqSearch::OutComb()
{
    return outComb;
}

float SeqSearch::MSCC()
{
    return msCC;
}

vector<float> SeqSearch::MSCCVec()
{
    return meansquareCC;
}

void SeqSearch::CombPrint()
{
    printf("Calling CombPrint for SeqSearch instance: ");
    std::cout << m_name << std::endl;
    printf("Total number of combination: %u\n", combNum);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < combNum; ++j)
        {
            printf("%-3u,", outComb[i][j]);
        }
        printf("\n");
    }
}

void SeqSearch::MSCCPrint()
{
    printf("Calling MSCCPrint for SeqSearch instance: ");
    std::cout << m_name << std::endl;
    printf("Mean Square Cross-Correlation: %f\n", msCC);
}

void SeqSearch::Report()
{
    printf("Current SeqSearch:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Input Sequences Number: %u\n", inDim);
    printf("Input Sequences Length: %u\n", inLen);
    printf("Input Bit Length:       %u\n", bitLength);
    printf("Good CC Threshold:      %.3f\n", threshold);
}

unsigned int SeqSearch::CombNum()
{
    return combNum;
}

void SeqSearch::Calc()
{
    srand(time(NULL));
    msCC = 0;
    int index = 0;
    for (int i = 0; i < inDim; ++i)
    {
        for (int j = 0; j < inDim; ++j)
        {
            if (i < j)
            {
                // printf("%-3d,%-3d\n", i, j);
                meansquareCC[index] = 0;
                vector<vector<unsigned int>> inSeq2(2);
                inSeq2[0] = inSeq[i];
                inSeq2[1] = inSeq[j];
                vector<vector<char>> outSeq2(2);
                outSeq2[0].resize(inSeq2[0].size());
                outSeq2[1].resize(inSeq2[1].size());
                // unsigned int lowCCFlag = 1;
                vector<unsigned int> bitLengthVec(2);
                vector<float> probVec(2);
                // float prob;
                
                for (int k = 0; k < 10000; ++k)
                {   
                    RandNum2BitMulti num2bitMultiInst;
                    CrossCorrelation CCInst;
                    for (int l = 0; l < 2; ++l)
                    {
                        bitLengthVec[l] = bitLength;
                        probVec[l] = (float)((float)(rand()%(int)pow(2,bitLength))/(float)pow(2,bitLength));
                    }

                    num2bitMultiInst.Init(probVec,bitLengthVec,inSeq2,unipolar,"num2bitMultiInst");
                    num2bitMultiInst.SeqGen();
                    // num2bitMultiInst.SeqPrint();
                    outSeq2 = num2bitMultiInst.OutSeq();

                    CCInst.Init(outSeq2,threshold,"CCInst");
                    CCInst.Calc();
                    // if (isnan(CCInst.OutCC()[0]))
                    // {
                    //     printf("prob:%f,%f\n", probVec[0], probVec[1]);
                    //     printf("CC:%f\n", CCInst.OutCC()[0]);
                    //     num2bitMultiInst.SeqPrint();
                    // }

                    meansquareCC[index] += CCInst.OutCC()[0]*CCInst.OutCC()[0];
                }
                // printf("\n%f\n", meansquareCC[index]);

                // avg cc for (i,j) comb
                meansquareCC[index] = meansquareCC[index]/10000;
                msCC += meansquareCC[index];
                index++;
            }
        }
    }
    // printf("%f\n", msCC);
    msCC = sqrt(msCC/totalComb);
    // printf("%f\n", msCC);
}