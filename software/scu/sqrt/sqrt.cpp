#include "sqrt.hpp"
#include "seqprob.hpp"
#include "autocorrelation.hpp"
#include "crosscorrelation.hpp"

SQRT::SQRT(){}
SQRT::~SQRT(){}
void SQRT::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SQRT Help. Following are instructions to SQRT Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SQRT inst.\n");
    printf("Initial Parameters: Input Vector, Random Number Seqsence, Bit Length of Random Number, Instance Name.\n");
    printf("Recommended Tracing Memory Bit Length: 2\n");

    printf("2. inst.Calc() method:\n");
    printf("Calculate the quotient of two input sequences.\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the calculated result.\n");

    printf("4. inst.OutPrint() method:\n");
    printf("Print the information of result.\n");

    printf("5. inst.InAC() method:\n");
    printf("Return the input auto-correlation.\n");

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

void SQRT::Init(vector<unsigned int> param1, vector<unsigned int> param2, unsigned int param3, string param4)
{
    inSeq = param1;
    SeqProb probCalc;
    probCalc.Init(inSeq,"probCalc");
    probCalc.Calc();
    inProb = probCalc.OutProb();
    randNum = param2;
    bitLength = param3;
    m_name = param4;

    seqLength = (unsigned int)inSeq.size();
    theoProb = sqrt(inProb);
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

void SQRT::Report()
{
    printf("Current SQRT:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Bit Length of RandNum:  %u\n", bitLength);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f\n", inProb);
    printf("Theoretical Probability:%f\n", theoProb);
}

// void SQRT::CalcQuot()
void SQRT::Calc()
{
    AutoCorrelation autocorrelationInst;
    autocorrelationInst.Init(inSeq, 1, inProb, "autocorrelationInst");
    autocorrelationInst.Calc();
    inAC = autocorrelationInst.OutAC();
    unsigned int accuracyLength = seqLength/2;

    // // *****************************************************************************
    // // counter based no correlation
    // // *****************************************************************************
    // unsigned int depth = 5;
    // unsigned int upperBound = (unsigned int)pow(2,depth)-1;
    // unsigned int halfBound = (unsigned int)pow(2,depth-1);
    // unsigned int traceReg = halfBound;
    // unsigned int oneCount = 0;

    // for (int i = 0; i < seqLength; ++i)
    // {
    //     if (traceReg >= (randNum[i] >> (bitLength-depth)))
    //     {
    //         outSeq[i] = 1;
    //     }
    //     else
    //     {
    //         outSeq[i] = 0;
    //     }
    //     oneCount += outSeq[i];
    //     if (i < accuracyLength)
    //     {
    //         realProb[i] = (float)oneCount/(float)(i+1);
    //     }
    //     else
    //     {
    //         realProb[i] = (realProb[i-1]*accuracyLength+outSeq[i]-outSeq[i-accuracyLength])/accuracyLength;
    //     }
    //     errRate[i] = (theoProb - realProb[i]);
    //     unsigned int andGate = outSeq[i] & outSeq[(i+seqLength-1)%seqLength];
    //     unsigned int inc = inSeq[i];
    //     unsigned int dec = andGate;
    //     if (inc == 1 && dec == 0)
    //     {
    //         if (traceReg < upperBound)
    //         {
    //             traceReg = traceReg + 1;
    //         }
    //     }
    //     else if (inc == 0 && dec == 1)
    //     {
    //         if (traceReg > 0)
    //         {
    //             traceReg = traceReg - 1;
    //         }
    //     }
    // }

    // AutoCorrelation jkACout;
    // jkACout.Init(JKFF, 1, 1/(1+theoProb), "jkACout");
    // jkACout.Calc();


    // // *****************************************************************************
    // // bit inserting
    // // *****************************************************************************
    // vector<unsigned int> JKFF(seqLength);
    // for (int i = 0; i < seqLength; ++i)
    // {
    //     JKFF[i] = 1;
    // }
    // unsigned int oneCount = 0;
    // unsigned int sel = 1;

    // for (int i = 0; i < seqLength; ++i)
    // {
    //     // printf("%u,", sel);
    //     if (sel == 1)
    //     {
    //         outSeq[i] = inSeq[i];
    //     }
    //     else
    //     {
    //         outSeq[i] = 1;
    //     }
    //     oneCount += outSeq[i];
    //     if (i < accuracyLength)
    //     {
    //         realProb[i] = (float)oneCount/(float)(i+1);
    //     }
    //     else
    //     {
    //         realProb[i] = (realProb[i-1]*(float)accuracyLength+outSeq[i]-outSeq[i-accuracyLength])/(float)accuracyLength;
    //     }
    //     // errRate[i] = (theoProb - realProb[i])/theoProb;
    //     errRate[i] = (theoProb - realProb[i]);

    //     // applying a JK FF
    //     // J is always 1.
    //     // K is outSeq[i].
    //     // if (outSeq[(i+seqLength-1)%seqLength] == 1)
    //     if (outSeq[i] == 1)
    //     {
    //         sel = 1-sel;
    //         JKFF[i] = sel;
    //     }
    //     else
    //     {
    //         sel = 1;
    //         JKFF[i] = sel;
    //     }
    // }

    // SeqProb jkProb;
    // jkProb.Init(JKFF, "jkProb");
    // jkProb.Calc();
    // AutoCorrelation jkACin;
    // jkACin.Init(outSeq, 1, theoProb, "jkACin");
    // jkACin.Calc();
    // AutoCorrelation jkACout;
    // jkACout.Init(JKFF, 1, 1/(1+theoProb), "jkACout");
    // jkACout.Calc();
    // vector<vector<unsigned int>> selinSeq(2);
    // selinSeq[0] = inSeq;
    // selinSeq[1] = JKFF;
    // CrossCorrelation selCC;
    // selCC.Init(selinSeq, 1, "selCC");
    // selCC.Calc();


    // printf("jkACin: %f\n", jkACin.OutAC());
    // printf("jkACout: %f\n", jkACout.OutAC());
    // printf("selinCC: %f\n", selCC.OutCC());
    // printf("InProb: %.5f\n", inProb);
    // printf("Theo OutProb: %.5f\n", theoProb);
    // printf("Real OutProb: %.5f\n", realProb[seqLength-1]);
    // printf("Theo Sel Prob: %.5f\n", 1/(1+sqrt(inProb)));
    // printf("Real Sel Prob: %.5f\n", jkProb.OutProb());


    // *****************************************************************************
    // bit inserting with simplified iscvdiv for 1/(1+Po)
    // *****************************************************************************
    vector<unsigned int> DFF(seqLength);
    vector<unsigned int> INV(seqLength);
    vector<unsigned int> andGate(seqLength);
    vector<unsigned int> orGate(seqLength);
    unsigned int depth = 2;
    unsigned int logDepth = (unsigned int)log2(depth);
    vector<unsigned int> traceReg(depth);

    for (int i = 0; i < depth; ++i)
    {
        traceReg[i] = 0;
    }

    for (int i = 0; i < seqLength; ++i)
    {
        DFF[i] = i%2;
        INV[i] = 1-DFF[i];
    }
    unsigned int oneCount = 0; // calc the ones in output seq
    vector<unsigned int> mux0sel(seqLength);
    vector<unsigned int> mux1out(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        mux1out[i] = 1;
    }

    for (int i = 0; i < seqLength; ++i)
    {
        mux0sel[i] = traceReg[(randNum[i] >> (bitLength - logDepth))];
        // mux0sel[i] = mux1out[(i+seqLength-1)%seqLength];
        outSeq[i] = mux0sel[i] ? inSeq[i] : 1;
        andGate[i] = outSeq[i] & INV[i];
        orGate[i] = andGate[i] | DFF[i];
        mux1out[i] = orGate[i] ? DFF[i] : mux0sel[i];

        if (orGate[i] == 1)
        {
            for (int index = 0; index < depth-1; ++index)
            {
                traceReg[index] = traceReg[index+1];
            }
            traceReg[depth-1] = mux1out[i];
        }
        
        oneCount += outSeq[i];
        if (i < accuracyLength)
        {
            realProb[i] = (float)oneCount/(float)(i+1);
        }
        else
        {
            realProb[i] = (realProb[i-1]*(float)accuracyLength+outSeq[i]-outSeq[i-accuracyLength])/(float)accuracyLength;
        }
        // errRate[i] = (theoProb - realProb[i])/theoProb;
        errRate[i] = (theoProb - realProb[i]);
        // printf("(inSeq)%u, (mux0sel)%u, (outSeq)%u, (andGate)%u, (orGate)%u, (mux1out)%u\n", inSeq[i], mux0sel[i], outSeq[i], andGate[i], orGate[i], mux1out[i]);
    }

    for (int i = 0; i < seqLength; ++i)
    {
        if (errRate[seqLength-1-i] > 0.05 || errRate[seqLength-1-i] < -0.05)
        {
            lowErrLen = seqLength-i;
            break;
        }
    }
    // AutoCorrelation outACinst;
    // outACinst.Init(outSeq, 1, theoProb, "outACinst");
    // outACinst.Calc();
    // printf("%f\n", outACinst.OutAC());


    // printf("%f\n", inProb);

    // SeqProb mux0selProb;
    // mux0selProb.Init(mux0sel, "mux0selProb");
    // mux0selProb.Calc();
    // mux0selProb.ProbPrint();

    // SeqProb andGateProb;
    // andGateProb.Init(andGate, "andGateProb");
    // andGateProb.Calc();
    // andGateProb.ProbPrint();

    // SeqProb orGateProb;
    // orGateProb.Init(orGate, "orGateProb");
    // orGateProb.Calc();
    // orGateProb.ProbPrint();

    // SeqProb mux1outProb;
    // mux1outProb.Init(mux1out, "mux1outProb");
    // mux1outProb.Calc();
    // mux1outProb.ProbPrint();

    // SeqProb outSeqProb;
    // outSeqProb.Init(outSeq, "outSeqProb");
    // outSeqProb.Calc();
    // outSeqProb.ProbPrint();

    // printf("%f, %f\n", 1/(1+outSeqProb.OutProb()), mux1outProb.OutProb());
    // vector<vector<unsigned int>> selinSeq(2);
    // selinSeq[0] = inSeq;
    // selinSeq[1] = mux0sel;

    // CrossCorrelation selCC;
    // selCC.Init(selinSeq, 1, "selCC");
    // selCC.Calc();
    // selCC.CCPrint();
}

vector<unsigned int> SQRT::OutSeq()
{
    return outSeq;
}

unsigned int SQRT::PPStage()
{
    return ppStage;
}

void SQRT::OutPrint()
{
    printf("Calling OutPrint for SQRT instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: sqrt(%.3f) = %.3f with input autocorrelation %.3f\n", inProb, theoProb, inAC);
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

float SQRT::InAC()
{
    return inAC;
}

float SQRT::InProb()
{
    return inProb;
}

float SQRT::TheoProb()
{
    return theoProb;
}

vector<float> SQRT::RealProb()
{
    return realProb;
}

float SQRT::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> SQRT::ErrRate()
{
    return errRate;
}

float SQRT::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int SQRT::SeqLen()
{
    return seqLength;
}

unsigned int SQRT::LowErrLen()
{
    return lowErrLen;
}