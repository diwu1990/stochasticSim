#include "sobol.hpp"
#include "sobolmerge.hpp"

int main()
{
    SOBOL sobolinst1;
    sobolinst1.Init(8,1,0,"sobolinst1");
    sobolinst1.SeqGen();
    sobolinst1.SeqPrint();
    sobolinst1.VecPrint();
    sobolinst1.MemPrint();

    SOBOL sobolinst2;
    sobolinst2.Init(8,2,0,"sobolinst2");
    sobolinst2.SeqGen();
    // sobolinst2.SeqPrint();
    // sobolinst2.VecPrint();
    // sobolinst2.MemPrint();

    vector<vector<unsigned int>> dirVec(2);
    dirVec[0] = sobolinst1.DirVec();
    dirVec[1] = sobolinst2.DirVec();
    SOBOLMerge sobolmergeinst;
    sobolmergeinst.Init(dirVec,8,"sobolmergeinst");
    sobolmergeinst.Report();
    sobolmergeinst.Calc();
    sobolmergeinst.VecPrint();
    sobolmergeinst.MemPrint();
}