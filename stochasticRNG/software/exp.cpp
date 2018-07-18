#include "exp.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

EXP::EXP(){}

EXP::~EXP(){}

void EXP::Help()
{
    // void Report();
    // void OutPrint();
    // vector<unsigned int> OutSeq();
    // float InCC();
    // vector<float> InProb();
    // float TheoProb();
    // vector<float> RealProb();
    // float FinalRealProb();
    // vector<float> ErrRate();
    // float FinalErrRate();
    // unsigned int SeqLen();
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling EXP Help. Following are instructions to EXP Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the EXP inst.\n");
    printf("Initial Parameters: Two Input Vectors and Selection Signal.\n");

    printf("2. inst.CalcProd() method:\n");
    printf("Calculate the exponent of input.\n");

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
    printf("Return the sequence length.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void EXP::Init(vector<vector<unsigned int>> param1, vector<vector<unsigned int>> param2, string param3)
{
    inSeq = param1;
    inSel = param2;
    SeqProbMulti probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.CalcProb();
    inProb = probCalc.OutProb();
    m_name = param3;
    if ((unsigned int)inSeq.size() == (unsigned int)inProb.size() && (unsigned int)inSeq.size() == 3) // three stage delay
    {
        inDim = (unsigned int)inSeq.size();
    }
    else
    {
        printf("Error: Input Dimension is not 3.\n");
    }

    if ((unsigned int)inSeq[0].size() == (unsigned int)inSeq[1].size())
    {
        seqLength = (unsigned int)inSeq[0].size();
    }
    else
    {
        printf("Error: Input Length is not the same.\n");
    }
    theoProb = exp(inProb[0]) /4; // '+' for summation, '*' for multiplication?
    outSeq.resize(seqLength);
    realProb.resize(seqLength);
    errRate.resize(seqLength);

    vector<vector<unsigned int>> MuxConst;


    for (int i = 0; i < seqLength; ++i)
    {
        outSeq[i] = 0;
        realProb[i] = 0;
        errRate[i] = 0;
        MuxConst[0][i] = 1; // initialize the constants for addition
        MuxConst[1][i] = 0;
        MuxConst[2][i] = 0;
    }
    lowErrLen = 0;
    // for (int i = 0; i < inDim; ++i)
    // {
    //     for (int j = 0; j < seqLength; ++j)
    //     {
    //         printf("%u,", inSeq[i][j]);
    //     }
    //     printf("\n");
    // }
}

void EXP::Report()
{
    printf("Current EXP:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Number of Seqsences:    %u\n", inDim);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f\n", inProb[0]);
    //printf("Input sel:      %f\n", inProb[2]); // input probability of selection signal??**
    printf("Theoretical Probability:%f\n", theoProb);
}

void EXP::Calc()
{
    CrossCorrelation inputCC;
    inputCC.Init(inSeq,1,"inputCC"); 
    inputCC.CalcCC();
    inCC = inputCC.OutCC()[0];

    float oneCount = 0;

    /*
    if (inSeq[2][0] == 1) // inSeq[2] -> sel signal for the mux 
    {
        outSeq[0] = inSeq[0][0];
        oneCount += outSeq[0];
        realProb[0] = oneCount/(0+1);
        errRate[0] = (theoProb - realProb[0])/theoProb;
    }
    else
    {
        outSeq[0] = inSeq[1][0];
        oneCount += outSeq[0];
        realProb[0] = oneCount/1;
        errRate[0] = (theoProb - realProb[0])/theoProb;
    }
    */

    for (int i = 1; i < seqLength; ++i)
    {   
        
        vector<unsigned int> outTemp;

        for (int j = 0; j < 3; ++j) // three states 
        {
            if(j == 0)
            if(inSel[j][i] == 1)// inSel[j][i] % 2 == 1
            {
                outTemp[j] = 1; // mux constant
            }
            else
            {
                outTemp[j] = inSeq[j][i];
            }
            else if(j == 1)
            {
                if(inSeq[j][i] == 1 && inSeq[j-1][i] == 1)
                {
                    if (inSel[j][i] == 1) // inSel[j][i] % 2 == 1
                    {
                        outTemp[j] = 1;
                    }
                    else
                    {
                        outTemp[j] = 0; // mux constant
                    }
                }
                else
                {
                    outTemp[j] = 0; // can only be zero here
                }
            }
            else
            {
                if(inSeq[j][i] == 1 && inSeq[j-2][i] == 1)
                {
                    if (inSel[j][i] == 1) // may be modified to integer mod // inSel[j][i] % 4 == 1
                    {
                        outTemp[j] = 1;
                    }
                    else
                    {
                        outTemp[j] = 0; // mux constant
                    }
                }
                else
                {
                    outTemp[j] = 0; // can only be zero here
                }                   
            }
        }

        // final combination scaling logic
        if(inSel[5][i] == 1) // inSel[5][i] % 2 == 1
        {
            outSeq[i] = outTemp[0];
            oneCount += 1;
            realProb[i] = (float)oneCount/(float)(i+1);
            errRate[i] = (theoProb - realProb[i])/theoProb;
        }
        else
        {
            if(inSel[4][i] == 1) // inSel[4][i] % 2 == 1
            {
            outSeq[i] = outTemp[1];
            oneCount += 1;
            realProb[i] = (float)oneCount/(float)(i+1);
            errRate[i] = (theoProb - realProb[i])/theoProb;               
            }
            else
            {
            outSeq[i] = outTemp[2];
            oneCount += 1;
            realProb[i] = (float)oneCount/(float)(i+1);
            errRate[i] = (theoProb - realProb[i])/theoProb;                   
            }
        }

    }



    // find the convergence point
    for (int i = 0; i < seqLength; ++i)
    {
        // printf("%f\n", errRate[seqLength-1-i]);
        // if (errRate[seqLength-1-i] > 0.05)
        // {
        //     printf("larger than 0.05\n");
        // }
        // if (errRate[seqLength-1-i] < -0.05)
        // {
        //     printf("smaller than -0.05\n");
        // }
        if (errRate[seqLength-1-i] > 0.05 || errRate[seqLength-1-i] < -0.05)
        {
            lowErrLen = seqLength-i;
            break;
        }
    }
}

vector<unsigned int> EXP::OutSeq()
{
    return outSeq;
}

void EXP::OutPrint()
{
    printf("Calling OutPrint for EXP instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: exp (%.3f) = %.3f with input crosscorrelation %.3f\n", inProb[0], theoProb, inCC);
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

float  EXP::InCC()
{
    return inCC;
}

vector<float> EXP::InProb()
{
    return inProb;
}

float EXP::TheoProb()
{
    return theoProb;
}

vector<float> EXP::RealProb()
{
    return realProb;
}

float EXP::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> EXP::ErrRate()
{
    return errRate;
}

float EXP::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int EXP::SeqLen()
{
    return seqLength;
}

unsigned int EXP::LowErrLen()
{
    return lowErrLen;
}


