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
// #include "skewedsynchronizer.hpp"

// int main()
// {
    
//     srand(time(NULL));
//     unsigned int sobolNum = 2;
//     unsigned int sobolBitLen = 8;
//     // string mode = "incremental";
//     string mode = "delayed";
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
//         vector<float> mse(sobolBitLen);
//         for (int i = 0; i < sobolBitLen; ++i)
//         {
//             mse[i] = 0;
//         }

//         for (int i = 0; i < sobolBitLen; ++i)
//         {
//             depth = i+1;
//             for (int iter = 0; iter < 100000; ++iter)
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

//                 RandNum2BitMulti num2bitMultiInst;
//                 num2bitMultiInst.Init(probVec,bitLengthVec,sobolinst.OutSeq(),"num2bitMultiInst");
//                 num2bitMultiInst.SeqGen();
//                 // num2bitMultiInst.Report();
//                 // num2bitMultiInst.SeqPrint();

//                 SkewedSynchronizer syncInst;
//                 syncInst.Init(num2bitMultiInst.OutSeq(),depth,"syncInst");
//                 // syncInst.Report();
//                 syncInst.SeqGen();
//                 // syncInst.ErrPrint();
//                 // mse[i] += syncInst.ErrRate() * syncInst.ErrRate();
//                 mse[i] += syncInst.OutCC() * syncInst.OutCC();
//                 // syncInst.CCPrint();
//                 // syncInst.ProbPrint();
//                 // syncInst.SeqPrint();
//             }
//             mse[i] = sqrt(mse[i]/100000);
//         }

//         printf("start index, %u, delay, %u,\n", sobolInitIdx, delay);
//         for (int i = 0; i < sobolBitLen; ++i)
//         {
//             printf("%-.3f, ", mse[i]);
//         }
//         printf("\n");
//     }
    
// }