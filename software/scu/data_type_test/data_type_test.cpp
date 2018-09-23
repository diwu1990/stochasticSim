#include <cstdlib>
#include <ctime>
#include <vector>
#include <bitset>
using namespace std;

int main()
{
	double elapsed_secs[4];
	for (int i = 0; i < 4; ++i)
	{
		elapsed_secs[i] = 0;
	}
	for (int iter = 0; iter < 100000; ++iter)
	{
		int num = 4096;
	    vector<char> v1(num,0);
	    vector<bool> v2(num,0);
	    vector<int> v3(num,0);
	    bitset<4096> v4(0);
		clock_t begin = clock();
	    for (int i = 0; i < num; ++i)
	    {
	    	v1[i] = 1;
	    }
		clock_t end = clock();
		elapsed_secs[0] += double(end - begin) / CLOCKS_PER_SEC;
		// printf("%f\n", elapsed_secs);

		clock_t begin_1 = clock();
	    for (int i = 0; i < num; ++i)
	    {
	    	v2[i] = 245;
	    }
		clock_t end_1 = clock();
		elapsed_secs[1] += double(end_1 - begin_1) / CLOCKS_PER_SEC;
		// printf("%f\n", elapsed_secs_1);

		clock_t begin_2 = clock();
	    for (int i = 0; i < num; ++i)
	    {
	    	v3[i] = 1;
	    }
		clock_t end_2 = clock();
		elapsed_secs[2] += double(end_2 - begin_2) / CLOCKS_PER_SEC;
		// printf("%f\n", elapsed_secs_2);

		clock_t begin_3 = clock();
	    for (int i = 0; i < num; ++i)
	    {
	    	v4[i] = 1;
	    }
		clock_t end_3 = clock();
		elapsed_secs[3] += double(end_3 - begin_3) / CLOCKS_PER_SEC;
		// printf("%f\n", elapsed_secs_3);
	}
	for (int i = 0; i < 4; ++i)
	{
		printf("%f\n", elapsed_secs[i]);
	}




    // printf("%d\n", &v2[0]);
    // printf("%d\n", &v3[0]);

    // for (int index = 10; index < 15; ++index)
    // {
    //     for (int i = 0; i < foldNum; ++i)
    //     {
    //         tenFoldErr[i] = 0;
    //         tenFoldCorr[i] = 0;
    //         tenFoldBias[i] = 0;
    //         tenFoldNum[i] = 0;
    //         tenFoldLowErrLen[i] = 0;
    //     }
    //     unsigned int sobolInitIdx = 1+index;
    //     unsigned int delay = 0;
    //     // SystemRandMulti sobolinst;
    //     SOBOLMulti sobolinst;
    //     // LFSRMulti sobolinst;
    //     sobolinst.Init(sobolNum,sobolInitIdx,delay,sobolBitLen,mode,"sobolinst1");
    //     sobolinst.SeqGen();
    //     // sobolinst.SeqPrint();

    //     vector<unsigned int> bitLengthVec(2);
    //     vector<float> probVec(2);
    //     vector<float> val(2);
    //     unsigned int depth;
    //     // depth = (unsigned int)pow(2,3);
    //     depth = 2;
    //     for (int iter = 0; iter < totalIter; ++iter)
    //     {
    //         /* code */
    //         float prob0 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
    //         float prob1 = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen));
    //         val[0] = min(prob0,prob1);
    //         val[1] = max(prob0,prob1);
    //         for (int l = 0; l < 2; ++l)
    //         {
    //             bitLengthVec[l] = sobolBitLen;
    //             probVec[l] = val[l];
    //             // probVec[l] = (float)((float)(rand()%(int)pow(2,sobolBitLen))/(float)pow(2,sobolBitLen))/2+0.5;
    //         }

    //         vector<vector<unsigned int>> inRandNum(2);
    //         inRandNum[0].resize(seqLength);
    //         inRandNum[1].resize(seqLength);
    //         for (int z = 0; z < seqLength; ++z)
    //         {
    //             inRandNum[0][z] = sobolinst.OutSeq()[0][z%(unsigned int)(pow(2,sobolBitLen))];
    //             inRandNum[1][z] = sobolinst.OutSeq()[1][z%(unsigned int)(pow(2,sobolBitLen))];
    //         }
            
    //         vector<unsigned int> RandSeq;
    //         RandSeq.resize(seqLength);
    //         for (int z = 0; z < seqLength; ++z)
    //         {
    //             RandSeq[z] = sobolinst.OutSeq()[2][z%(unsigned int)(pow(2,sobolBitLen))];
    //         }
    //         RandNum2BitMulti num2bitMultiInst;
    //         num2bitMultiInst.Init(probVec,bitLengthVec,inRandNum,"num2bitMultiInst");
    //         num2bitMultiInst.SeqGen();
    //         // num2bitMultiInst.Report();
    //         // num2bitMultiInst.SeqPrint();

    //         // DeSynchronizer syncInst;
    //         Synchronizer syncInst;
    //         syncInst.Init(num2bitMultiInst.OutSeq(),1,"syncInst");
    //         syncInst.SeqGen();

    //         // for (int ooo = 0; ooo < sobolinst.SeqLen(); ++ooo)
    //         // {
    //         //     printf("%d: %u, %u", i, sobolinst.OutSeq()[2][ooo], sobolinst.OutSeq()[2][ooo]);
    //         //     /* code */
    //         // }
    //         MUXADD computingInst;
    //         // computingInst.Init(num2bitMultiInst.OutSeq(),RandSeq,sobolBitLen,"computingInst");
    //         computingInst.Init(syncInst.OutSeq(),RandSeq,sobolBitLen,"computingInst");
    //         computingInst.Calc();
    //         // printf("%f,%f\n", computingInst.InProb()[0], computingInst.InProb()[1]);
    //         // printf("%f\n", computingInst.TheoProb());
    //         // printf("%f\n", computingInst.FinalRealProb());

    //         tenFoldErr[(unsigned int)floor(computingInst.TheoProb()*10)] += computingInst.FinalErrRate() * computingInst.FinalErrRate();
    //         tenFoldBias[(unsigned int)floor(computingInst.TheoProb()*10)] += computingInst.FinalErrRate();
    //         tenFoldNum[(unsigned int)floor(computingInst.TheoProb()*10)] += 1;
    //         tenFoldLowErrLen[(unsigned int)floor(computingInst.TheoProb()*10)] += computingInst.LowErrLen();
    //         tenFoldCorr[(unsigned int)floor(computingInst.TheoProb()*10)] += computingInst.InCC()[0];
    //     }
    //     for (int y = 0; y < foldNum; ++y)
    //     {
    //         // printf("11111\n");
    //         tenFoldErr[y] = sqrt(tenFoldErr[y]/tenFoldNum[y]);
    //         tenFoldBias[y] = tenFoldBias[y]/tenFoldNum[y];
    //         tenFoldLowErrLen[y] = (tenFoldLowErrLen[y]/tenFoldNum[y]);
    //         tenFoldCorr[y] = tenFoldCorr[y]/tenFoldNum[y];
    //     }
        

    //     printf("Ten Fold, initial sobolIdx %u, delay %u.\n", sobolInitIdx, delay);
    //     printf("Range, Freq, Correlation, Error Rate, Stat Bias, LowErrLen:\n");
    //     for (int i = 0; i < foldNum; ++i)
    //     {
    //         printf("%*.1f, %*u, %*.4f, %*.4f, %*.4f, %*.4f\n", 5, ((float)i/10.0), 4, tenFoldNum[i], 11, tenFoldCorr[i], 10, tenFoldErr[i], 9, tenFoldBias[i], 9, tenFoldLowErrLen[i]);
    //     }
    //     printf("\n");
    // }
    
}
