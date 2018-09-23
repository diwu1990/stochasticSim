#include "simplified_iscbdiv_for_bisqrt.hpp"
#include "seqprob.hpp"
#include "autocorrelation.hpp"
#include "crosscorrelation.hpp"

SIMPLIFIED_ISCBDIV_FOR_BISQRT::SIMPLIFIED_ISCBDIV_FOR_BISQRT(){}
SIMPLIFIED_ISCBDIV_FOR_BISQRT::~SIMPLIFIED_ISCBDIV_FOR_BISQRT(){}
void SIMPLIFIED_ISCBDIV_FOR_BISQRT::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SIMPLIFIED_ISCBDIV_FOR_BISQRT Help. Following are instructions to SIMPLIFIED_ISCBDIV_FOR_BISQRT Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SIMPLIFIED_ISCBDIV_FOR_BISQRT inst.\n");
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

void SIMPLIFIED_ISCBDIV_FOR_BISQRT::Init(vector<char> param1, vector<unsigned int> param2, unsigned int param3, string param4)
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
    theoProb = 1/(1+inProb);
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

void SIMPLIFIED_ISCBDIV_FOR_BISQRT::Report()
{
    printf("Current SIMPLIFIED_ISCBDIV_FOR_BISQRT:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Bit Length of RandNum:  %u\n", bitLength);
    printf("Seqsence Length:        %u\n", seqLength);
    printf("Input Probability:      %f\n", inProb);
    printf("Theoretical Probability:%f\n", theoProb);
}

// void SIMPLIFIED_ISCBDIV_FOR_BISQRT::CalcQuot()
void SIMPLIFIED_ISCBDIV_FOR_BISQRT::Calc()
{
    AutoCorrelation autocorrelationInst;
    autocorrelationInst.Init(inSeq, 1, inProb, "autocorrelationInst");
    autocorrelationInst.Calc();
    inAC = autocorrelationInst.OutAC();
    unsigned int accuracyLength = seqLength/2;

    // *****************************************************************************
    // bit inserting with simplified iscvdiv for 1/(1+Po)
    // *****************************************************************************
    vector<char> DFF(seqLength);
    vector<char> INV(seqLength);
    vector<char> andGate(seqLength);
    vector<char> orGate(seqLength);
    vector<char> regOut(seqLength);
    unsigned int depth = 2;
    unsigned int logDepth = (unsigned int)log2(depth);
    vector<char> traceReg(depth);

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
    vector<char> mux1out(seqLength);
    for (int i = 0; i < seqLength; ++i)
    {
        mux1out[i] = 1;
    }

    for (int i = 0; i < seqLength; ++i)
    {
        regOut[i] = traceReg[(randNum[i] >> (bitLength - logDepth))];
        andGate[i] = inSeq[i] & INV[i];
        orGate[i] = andGate[i] | DFF[i];
        mux1out[i] = orGate[i] ? DFF[i] : regOut[i];


        if (orGate[i] == 1)
        {
            for (int index = 0; index < depth-1; ++index)
            {
                traceReg[index] = traceReg[index+1];
            }
            traceReg[depth-1] = mux1out[i];
        }
        
        outSeq[i] = regOut[i];
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

vector<char> SIMPLIFIED_ISCBDIV_FOR_BISQRT::OutSeq()
{
    return outSeq;
}

unsigned int SIMPLIFIED_ISCBDIV_FOR_BISQRT::PPStage()
{
    return ppStage;
}

void SIMPLIFIED_ISCBDIV_FOR_BISQRT::OutPrint()
{
    printf("Calling OutPrint for SIMPLIFIED_ISCBDIV_FOR_BISQRT instance: ");
    std::cout << m_name << std::endl;
    printf("Theoretical Probability: 1/(1+%.3f) = %.3f with input autocorrelation %.3f\n", inProb, theoProb, inAC);
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

float SIMPLIFIED_ISCBDIV_FOR_BISQRT::InAC()
{
    return inAC;
}

float SIMPLIFIED_ISCBDIV_FOR_BISQRT::InProb()
{
    return inProb;
}

float SIMPLIFIED_ISCBDIV_FOR_BISQRT::TheoProb()
{
    return theoProb;
}

vector<float> SIMPLIFIED_ISCBDIV_FOR_BISQRT::RealProb()
{
    return realProb;
}

float SIMPLIFIED_ISCBDIV_FOR_BISQRT::FinalRealProb()
{
    return realProb[seqLength-1];
}

vector<float> SIMPLIFIED_ISCBDIV_FOR_BISQRT::ErrRate()
{
    return errRate;
}

float SIMPLIFIED_ISCBDIV_FOR_BISQRT::FinalErrRate()
{
    return errRate[seqLength-1];
}

unsigned int SIMPLIFIED_ISCBDIV_FOR_BISQRT::SeqLen()
{
    return seqLength;
}

unsigned int SIMPLIFIED_ISCBDIV_FOR_BISQRT::LowErrLen()
{
    return lowErrLen;
}
