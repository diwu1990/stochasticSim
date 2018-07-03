#include "main.hpp"
#include "autocorrelation.hpp"
#include "crosscorrelation.hpp"
#include "lfsr.hpp"
#include "sobol.hpp"
#include "xshrr1608.hpp"
#include "randNum2Bit.hpp"
#include "randNum2BitMulti.hpp"
#include "mul.hpp"
#include "sobolmulti.hpp"
#include "lfsrmulti.hpp"
#include "seqsearch.hpp"
#include <cstdlib>
#include <ctime>
#include "sobolmerge.hpp"
#include "synchronizer.hpp"

int main()
{
    
    unsigned int dim1 = 8;
    unsigned int dim2 = 1;
    SOBOL sobolinst1;
    sobolinst1.Init(8,dim1,0,"sobolinst1");
    sobolinst1.SeqGen();
    // sobolinst1.SeqPrint();
    // sobolinst1.VecPrint();
    // sobolinst1.MemPrint();

    SOBOL sobolinst2;
    sobolinst2.Init(8,dim2,0,"sobolinst2");
    sobolinst2.SeqGen();
    // sobolinst2.SeqPrint();
    // sobolinst2.VecPrint();
    // sobolinst2.MemPrint();

    vector<vector<unsigned int>> dirVec(2);
    dirVec[0] = sobolinst1.DirVec();
    dirVec[1] = sobolinst2.DirVec();



}
