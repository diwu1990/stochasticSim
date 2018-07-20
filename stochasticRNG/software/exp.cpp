#include "exp.hpp"
#include "seqprobmulti.hpp"
#include "crosscorrelation.hpp"

EXP::EXP(){}

EXP::~EXP(){}

void EXP::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling EXP Help. Following are instructions to EXP Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the EXP inst.\n");
    printf("Initial Parameters: Input Vectors and Selection Signal.\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the exponent of input.\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the calculated result.\n");

    printf("4. inst.OutPrint() method:\n");
    printf("Print the information of result.\n");

    printf("5. inst.InAC() method:\n");
    printf("Return the input autocorrelation.\n");

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

    printf("14. inst.Report() method:\n");
    printf("Report the status of current instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void EXP::Init(vector<unsigned int> param1, vector<vector<unsigned int>> param2, unsigned int param3, string param4)
{
    inSeq = param1;
    inSel = param2;
    SeqProb probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    bitLength = param3;
    m_name = param4;
    if ((unsigned int)inSel.size() != 4)
    {
        printf("Error: Input Sel Dimension is not 4.\n");
    }

    seqLength = (unsigned int)inSeq.size();
    for (int i = 0; i < 4; ++i)
    {
        if ((unsigned int)inSeq[i].size() != seqLength)
        {
            printf("Error: Input Seqsence Length is not the same with the Sel Length.\n");
        }
    }

    theoProb = exp(inProb)/4;
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
}

void EXP::Report()
{
    printf("Current EXP:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Random Num Bit Length:  %u\n", bitLength);
    printf("Input Probability:      %f\n", inProb);
    //printf("Input sel:      %f\n", inProb[2]); // input probability of selection signal??**
    printf("Theoretical Probability:%f\n", theoProb);
}

void EXP::Calc()
{
    AutoCorrelation inputAC;
    inputAC.Init(inSeq,1,inProb,"inputAC"); 
    inputAC.Calc();
    inAC = inputAC.OutAC();

    vector<unsigned int> selSeq(4);
    for (int i = 0; i < 4; ++i)
    {
        /* code */
    }

    float oneCount = 0;

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
    printf("Theoretical Probability: exp (%.3f) = %.3f with input crosscorrelation %.3f\n", inProb, theoProb, inAC);
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

float  EXP::InAC()
{
    return inAC;
}

float EXP::InProb()
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


