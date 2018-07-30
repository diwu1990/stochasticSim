#include <stdio.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

//         "0110" when "0000",0
//         "1011" when "0001",1
//         "0101" when "0010",2
//         "0100" when "0011",3
//         "0010" when "0100",4
//         "1110" when "0101",5
//         "0111" when "0110",6
//         "1010" when "0111",7
//         "1001" when "1000",8
//         "1101" when "1001",9
//         "1111" when "1010",10
//         "1100" when "1011",11
//         "0011" when "1100",12
//         "0001" when "1101",13
//         "0000" when "1110",14
//         "1000" when "1111",15
//         "XXXX" when others;

#define LUT 6,11,5,4,2,14,7,10,9,13,15,12,3,1,0,8


class SBOX4
{
    // value of input random number
    unsigned int inNum;
    // input bit length
    unsigned int inBitLength;
    // rate of input bit length divied by 4
    unsigned int inRate;
    // 4 bit internal SBox state
    unsigned int initState;
    unsigned int boxState;
    // value of output random number
    unsigned int outNum;
    string m_name;

    public:
        SBOX4();
        ~SBOX4();
        void Help();
        void Init(unsigned int, unsigned int, string);
        void Report();
        void NumGen(unsigned int);
        void NumPrint();
        void RatePrint();
        unsigned int OutNum();
    
};