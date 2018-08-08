#include "crosscorrelation.hpp"

int main()
{

    vector<vector<unsigned int>> inSeq(2);
    inSeq[0].resize(8);
    inSeq[1].resize(8);

    // inSeq[0][0] = 1;
    // inSeq[0][1] = 1;
    // inSeq[0][2] = 1;
    // inSeq[0][3] = 1;
    // inSeq[0][4] = 0;
    // inSeq[0][5] = 0;
    // inSeq[0][6] = 0;
    // inSeq[0][7] = 0;

    inSeq[0][0] = 1;
    inSeq[0][1] = 1;
    inSeq[0][2] = 1;
    inSeq[0][3] = 1;
    inSeq[0][4] = 1;
    inSeq[0][5] = 1;
    inSeq[0][6] = 0;
    inSeq[0][7] = 0;

    // inSeq[0][0] = 1;
    // inSeq[0][1] = 1;
    // inSeq[0][2] = 0;
    // inSeq[0][3] = 0;
    // inSeq[0][4] = 0;
    // inSeq[0][5] = 0;
    // inSeq[0][6] = 0;
    // inSeq[0][7] = 0;

    // inSeq[1][0] = 1;
    // inSeq[1][1] = 1;
    // inSeq[1][2] = 0;
    // inSeq[1][3] = 0;
    // inSeq[1][4] = 1;
    // inSeq[1][5] = 1;
    // inSeq[1][6] = 0;
    // inSeq[1][7] = 0;

    // inSeq[1][0] = 1;
    // inSeq[1][1] = 1;
    // inSeq[1][2] = 1;
    // inSeq[1][3] = 1;
    // inSeq[1][4] = 0;
    // inSeq[1][5] = 0;
    // inSeq[1][6] = 0;
    // inSeq[1][7] = 0;

    inSeq[1][0] = 0;
    inSeq[1][1] = 0;
    inSeq[1][2] = 0;
    inSeq[1][3] = 0;
    inSeq[1][4] = 1;
    inSeq[1][5] = 1;
    inSeq[1][6] = 1;
    inSeq[1][7] = 1;

    // inSeq[1][0] = 1;
    // inSeq[1][1] = 1;
    // inSeq[1][2] = 1;
    // inSeq[1][3] = 0;
    // inSeq[1][4] = 0;
    // inSeq[1][5] = 0;
    // inSeq[1][6] = 0;
    // inSeq[1][7] = 1;

    // inSeq[1][0] = 1;
    // inSeq[1][1] = 1;
    // inSeq[1][2] = 1;
    // inSeq[1][3] = 1;
    // inSeq[1][4] = 1;
    // inSeq[1][5] = 1;
    // inSeq[1][6] = 0;
    // inSeq[1][7] = 0;

    CrossCorrelation inCCInst;
    inCCInst.Init(inSeq, 1, "inCCInst");
    inCCInst.Calc();
    inCCInst.CCPrint();
    float inCC = inCCInst.OutCC()[0];

    printf("%f\n", inCC);

}
