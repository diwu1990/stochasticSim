// #include "main.hpp"
// #include "autocorrelation.hpp"
// #include "crosscorrelation.hpp"
// #include "lfsr.hpp"
// #include "sobol.hpp"
// #include "xshrr1608.hpp"
// #include "randNum2Bit.hpp"
// #include "randNum2BitMulti.hpp"
// #include "mul.hpp"
// #include "sobolmulti.hpp"
// #include "lfsrmulti.hpp"
// #include "seqsearch.hpp"
// #include <cstdlib>
// #include <ctime>
// #include "sobolmerge.hpp"
// #include "synchronizer.hpp"
// #include "div.hpp"

// int main()
// {
    
//     srand(time(NULL));
//     unsigned int sobolNum = 3;
//     unsigned int sobolBitLen = 8;
//     string mode = "incremental";
//     // string mode = "delayed";
//     unsigned int totalIter = 100000;
//     unsigned int seqLength = (unsigned int)pow(2,sobolBitLen);
//     unsigned int foldNum = 11;
//     vector<float> tenFoldErr(foldNum);
//     vector<unsigned int> tenFoldNum(foldNum);
//     vector<float> tenFoldLowErrLen(foldNum);
//     for (int index = 0; index < 10; ++index)
//     {
//         for (int i = 0; i < foldNum; ++i)
//         {
//             tenFoldErr[i] = 0;
//             tenFoldNum[i] = 0;
//             tenFoldLowErrLen[i] = 0;
//         }
//         unsigned int sobolInitIdx = 1+index;
//         unsigned int delay = 0;
//          SOBOLMulti sobolinst;
//         // LFSRMulti sobolinst;
//         sobolinst.Init(sobolNum,sobolInitIdx,delay,sobolBitLen,mode,"sobolinst1");
//         sobolinst.SeqGen();
//         // sobolinst.SeqPrint();

//         vector<unsigned int> bitLengthVec(2);
//         vector<float> probVec(2);
//         vector<float> val(2);
//         unsigned int depth;
//         depth = (unsigned int)pow(2,3);
//         // depth = 5;
//         for (int iter = 0; iter < totalIter; ++iter)
//         {
//             /* code */
//             float prob0 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
//             float prob1 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
//             val[0] = min(prob0,prob1);
//             val[1] = max(prob0,prob1);
//             for (int l = 0; l < 2; ++l)
//             {
//                 bitLengthVec[l] = sobolBitLen;
//                 probVec[l] = val[l];
//             }

//             vector<vector<unsigned int>> inRandNum(2);
//             inRandNum[0].resize(seqLength);
//             inRandNum[1].resize(seqLength);
//             for (int z = 0; z < seqLength; ++z)
//             {
//                 inRandNum[0][z] = sobolinst.OutSeq()[0][z%(unsigned int)(pow(2,sobolBitLen))];
//                 inRandNum[1][z] = sobolinst.OutSeq()[1][z%(unsigned int)(pow(2,sobolBitLen))];
//             }
//             vector<unsigned int> RandSeq;
//             RandSeq.resize(seqLength);
//             for (int z = 0; z < seqLength; ++z)
//             {
//                 RandSeq[z] = sobolinst.OutSeq()[2][z%(unsigned int)(pow(2,sobolBitLen))];
//             }
//             RandNum2BitMulti num2bitMultiInst;
//             num2bitMultiInst.Init(probVec,bitLengthVec,inRandNum,"num2bitMultiInst");
//             num2bitMultiInst.SeqGen();
//             // num2bitMultiInst.Report();
//             // num2bitMultiInst.SeqPrint();

//             // for (int ooo = 0; ooo < sobolinst.SeqLen(); ++ooo)
//             // {
//             //     printf("%d: %u, %u", i, sobolinst.OutSeq()[2][ooo], sobolinst.OutSeq()[2][ooo]);
//             //     /* code */
//             // }
//             DIV divInst;
//             divInst.Init(num2bitMultiInst.OutSeq(),RandSeq,sobolBitLen,depth,"divInst");
//             // divInst.Report();
//             divInst.Calc();

//             tenFoldErr[(unsigned int)floor(divInst.TheoProb()*10)] += divInst.FinalErrRate() * divInst.FinalErrRate();
//             tenFoldNum[(unsigned int)floor(divInst.TheoProb()*10)] += 1;
//             // printf("%u\n", divInst.LowErrLen());
//             tenFoldLowErrLen[(unsigned int)floor(divInst.TheoProb()*10)] += divInst.LowErrLen();
//             // printf("%u\n", tenFoldLowErrLen[(unsigned int)floor(divInst.TheoProb()*10)]);
//         }
//         for (int y = 0; y < foldNum; ++y)
//         {
//             // printf("11111\n");
//             tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
//             tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
//         }
        

//         printf("Ten Fold with Depth %u, initial sobolIdx %u, delay %u.\n", depth, sobolInitIdx, delay);
//         printf("Value range index, Freq, Final Error, LowErrLen:\n");
//         for (int i = 0; i < foldNum; ++i)
//         {
//             printf("%3d, %5u, %-.3f, %-3.3f\n", i, tenFoldNum[i], tenFoldErr[i], tenFoldLowErrLen[i]);
//         }
//         printf("\n");
//     }
    
// }
