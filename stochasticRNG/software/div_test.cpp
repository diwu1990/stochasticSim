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
//     // string mode = "incremental";
//     string mode = "delayed";
//     unsigned int totalIter = 10000;
//     unsigned int TotalDepth = 4;
//     unsigned int seqLength = 512;
//     for (int index = 0; index < 10; ++index)
//     {
//         unsigned int sobolInitIdx = 1+index;
//         unsigned int delay = 1;
//         SOBOLMulti sobolinst;
//         sobolinst.Init(sobolNum,sobolInitIdx,delay,sobolBitLen,mode,"sobolinst1");
//         sobolinst.SeqGen();
//         // sobolinst.SeqPrint();

//         vector<unsigned int> bitLengthVec(2);
//         vector<float> probVec(2);
//         vector<float> val(2);
//         unsigned int depth;
//         vector<float> mse(TotalDepth);
//         for (int i = 0; i < TotalDepth; ++i)
//         {
//             mse[i] = 0;
//         }

//         for (int i = 0; i < TotalDepth; ++i)
//         {
//             depth = (unsigned int)pow(2,i);
//             for (int iter = 0; iter < totalIter; ++iter)
//             {
//                 /* code */
//                 float prob0 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
//                 float prob1 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
//                 val[0] = min(prob0,prob1);
//                 val[1] = max(prob0,prob1);
//                 for (int l = 0; l < 2; ++l)
//                 {
//                     bitLengthVec[l] = sobolBitLen;
//                     probVec[l] = val[l];
//                 }

//                 vector<vector<unsigned int>> inRandNum(2);
//                 inRandNum[0].resize(seqLength);
//                 inRandNum[1].resize(seqLength);
//                 for (int z = 0; z < seqLength; ++z)
//                 {
//                     inRandNum[0][z] = sobolinst.OutSeq()[0][z%256];
//                     inRandNum[1][z] = sobolinst.OutSeq()[1][z%256];
//                 }
//                 vector<unsigned int> RandSeq;
//                 RandSeq.resize(seqLength);
//                 for (int z = 0; z < seqLength; ++z)
//                 {
//                     RandSeq[z] = sobolinst.OutSeq()[2][z%256];
//                 }
//                 RandNum2BitMulti num2bitMultiInst;
//                 num2bitMultiInst.Init(probVec,bitLengthVec,inRandNum,"num2bitMultiInst");
//                 num2bitMultiInst.SeqGen();
//                 // num2bitMultiInst.Report();
//                 // num2bitMultiInst.SeqPrint();

//                 // for (int ooo = 0; ooo < sobolinst.SeqLen(); ++ooo)
//                 // {
//                 //     printf("%d: %u, %u", i, sobolinst.OutSeq()[2][ooo], sobolinst.OutSeq()[2][ooo]);
//                 //     /* code */
//                 // }
//                 DIV divInst;
//                 divInst.Init(num2bitMultiInst.OutSeq(),RandSeq,sobolBitLen,depth,"divInst");
//                 // divInst.Report();
//                 divInst.CalcQuot();
//                 // divInst.OutPrint();
//                 // for (int len = 0; len < divInst.SeqLen(); ++len)
//                 // {
//                 //     printf("%u, ", divInst.OutSeq()[len]);
//                 // }
//                 // for (int len = 0; len < divInst.SeqLen(); ++len)
//                 // {
//                 //     printf("%f, ", divInst.RealProb()[len]);
//                 // }
//                 // printf("\n");
//                 mse[i] += divInst.FinalErrRate() * divInst.FinalErrRate();
//             }
//             mse[i] = sqrt(mse[i]/totalIter);
//         }

//         printf("start index, %u, delay, %u,\n", sobolInitIdx, delay);
//         for (int i = 0; i < TotalDepth; ++i)
//         {
//             printf("%-.3f, ", mse[i]);
//         }
//         printf("\n");
//     }
    
// }
