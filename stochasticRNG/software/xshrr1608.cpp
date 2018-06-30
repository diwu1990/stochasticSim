#include "xshrr1608.hpp"

// #include <stdint.h>
// static uint64_t       state      = 0x4d595df4d0f33173;      // Or something seed-dependent
// static uint64_t const multiplier = 6364136223846793005u;
// static uint64_t const increment  = 1442695040888963407u;    // Or an arbitrary odd constant

// static uint32_t rotr32(uint32_t x, unsigned r)
// {
//     return x >> r | x << (-r & 31);
// }

// uint32_t pcg32(void)
// {
//     uint64_t x = state;
//     unsigned count = (unsigned)(x >> 59);       // 59 = 64 - 5

//     state = x * multiplier + increment;
//     x ^= x >> 18;                               // 18 = (64 - 27)/2
//     return rotr32((uint32_t)(x >> 27), count);  // 27 = 32 - 5
// }

// void pcg32_init(uint64_t seed)
// {
//     state = seed + increment;
//     (void)pcg32();
// }

// XSH: An xorshift operation, x ^= x >> constant. The constant is chosen to be half of the bits not discarded by the next operation (rounded down).
// RR: A random (input-dependent) rotation, with output half the size of input. Given a 2^b-bit input word, the top b−1 bits are used for the rotate amount, the next-most-significant 2^(b−1) bits are rotated right and used as the output, and the low 2^(b−1)+1−b bits are discarded.
// RS: A random (input-dependent) shift, for cases where rotates are more expensive. Again, the output is half the size of the input. Beginning with a 2^b-bit input word, the top b−3 bits are used for a shift amount, which is applied to the next-most-significant 2^(b−1)+2^(b−3)−1 bits, and the least significant 2^(b−1) bits of the result are output. The low 2^(b−1)−2^(b−3)−b+4 bits are discarded.

XSHRR1608::XSHRR1608(){}
XSHRR1608::~XSHRR1608(){}
void XSHRR1608::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling XSHRR1608 Help. Following are instructions to XSHRR1608 Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the XSHRR1608 inst.\n");
    printf("Initial Parameters: Output Dimension, Input Random Number Sequence, Instance Name.\n");

    printf("2. inst.SeqGen() method:\n");
    printf("Generate random sequence based on parameters from inst.Init().\n");

    printf("3. inst.OutSeq() method:\n");
    printf("Return the vector containing random number sequence from inst.SeqGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current XSHRR1608 instance.\n");

    printf("5. inst.SeqPrint() method:\n");
    printf("Print the content in the output sequence of current XSHRR1608 instance.\n");

    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void XSHRR1608::Init(unsigned int param1, vector<unsigned int> param2, string param3)
{
    bitLen = 16;
    unsigned int log2bitLen = (unsigned int)log2((float)bitLen);
    // RR: A random (input-dependent) rotation, with output half the size of input. 
    // Given a 2^b-bit input word, the top b−1 bits are used for the rotate amount, 
    // the next-most-significant 2^(b−1) bits are rotated right and used as the output, 
    // and the low 2^(b−1)+1−b bits are discarded.
    rrShift1 = bitLen - (log2bitLen-1); // get the rotation value for rr
    rrShift3 = bitLen/2 - (log2bitLen-1); // get the shift value for rr
    rrShift2 = (bitLen - rrShift3)/2; // get the shift value for XSH
    outDim = param1;
    if (outDim > 8 || outDim < 1)
    {
        printf("Error: Output Sequence Dimension is illegal. (1~8)\n");
    }
    inSeq = param2;
    m_name = param3;
    seqLength = (unsigned int)inSeq.size();
    outSeq.resize(outDim);
    for (int i = 0; i < outDim; ++i)
    {
        outSeq[i].resize(seqLength);
    }
}

void XSHRR1608::Report()
{
    printf("Current XSHRR1608:\n");
    std::cout << "Instance name:          " << m_name << std::endl;
    printf("Bit Length:             %-5u\n", bitLen);
    printf("Output Dimension:       %-5u\n", outDim);
    printf("Sequence Length:        %-5u\n", seqLength);
    printf("First Shift Offset:     %-5u\n", rrShift1);
    printf("Second Shift Offset:    %-5u\n", rrShift2);
    printf("Third Rotation Offset:  %-5u\n", rrShift3);
}

void XSHRR1608::SeqGen()
{
    for (int i = 0; i < outDim; ++i)
    {
        for (int j = 0; j < seqLength; ++j)
        {
            outSeq[i][j] = inSeq[j];
            unsigned int count = (unsigned int)(outSeq[i][j] >> rrShift1);
            // printf("%u shift %u => %u\n", outSeq[i][j],rrShift1,count);
            // outSeq[i][j] ^= (outSeq[i][j] >> rrShift2);
            outSeq[i][j] ^= (outSeq[i][j] >> (rrShift2-4+i));
            // printf("%u xorshift %u\n", outSeq[i][j],rrShift2);
            // printf("%u shift %u: expected => %u, ", outSeq[i][j], rrShift3, outSeq[i][j] >> rrShift3);
            outSeq[i][j] >>= rrShift3;
            outSeq[i][j] %=  (unsigned int)pow(2,bitLen/2);
            // printf("actual => %u\n",outSeq[i][j]);
            // printf("origin: %u; right shift %u: %d; left shift %u: %d\n", outSeq[i][j], count, outSeq[i][j] >> count, (bitLen/2-count), ((outSeq[i][j] << (bitLen/2-count)) % ((unsigned int)pow(2,bitLen/2))));
            int realCount = (signed int)(count-4+i);
            if (realCount >= 0)
            {
                outSeq[i][j] = (outSeq[i][j] >> realCount) | ((outSeq[i][j] << (bitLen/2-realCount)) % ((unsigned int)pow(2,bitLen/2))); // rotate right the outSeq[i][j] by count
            }
            else
            {
                outSeq[i][j] = (outSeq[i][j] << (-realCount)) | ((outSeq[i][j] >> (bitLen/2+realCount)) % ((unsigned int)pow(2,bitLen/2))); // rotate right the outSeq[i][j] by count
            }
        }
    }
}

unsigned int& XSHRR1608::SeqLen()
{
    return seqLength;
}

unsigned int& XSHRR1608::SeqDim()
{
    return outDim;
}

void XSHRR1608::SeqPrint()
{
    printf("Calling SeqPrint for XSHRR1608 instance: ");
    std::cout << m_name << std::endl;
    for (int i = 0; i < outDim; ++i)
    {
        for (int j = 0; j < seqLength; ++j)
        {
            printf("%u,", outSeq[i][j]);
        }
        printf("\n");
    }
}

vector<vector<unsigned int>>& XSHRR1608::OutSeq()
{
    return outSeq;
}
