#include "SBox4.hpp"

// ----------------------------------------------------------------------------------
// --
// -- Small Scale AES 114 - 1 Row ; 1 Column ; 4 bits words
// --
// -- SBox Component
// --
// ----------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------
// -- Company:
// -- Engineer:
// --
// -- Create Date:    10:57:49 12/07/2015
// -- Design Name:
// -- Module Name:    SBox4 - Behavioral
// -- Project Name:
// -- Target Devices:
// -- Tool versions:
// -- Description:
// --
// -- Dependencies:
// --
// -- Revision:
// -- Revision 0.01 - File Created
// -- Additional Comments:
// --
// ----------------------------------------------------------------------------------

// library ieee;
// use ieee.std_logic_1164.all;
// use ieee.std_logic_unsigned.all;
// use ieee.numeric_std.all;
// use ieee.math_real.all;

// -- Uncomment the following library declaration if using
// -- arithmetic functions with Signed or Unsigned values
// --use IEEE.NUMERIC_STD.ALL;

// -- Uncomment the following library declaration if instantiating
// -- any Xilinx primitives in this code.
// --library UNISIM;
// --use UNISIM.VComponents.all;

// entity SBox4 is

// port(

//     sbox4In : in std_logic_vector(3 downto 0);
//     sbox4Out : out std_logic_vector(3 downto 0)
    
// );
    
// end SBox4;

// architecture Behavioral of SBox4 is

// begin
    
//     with sbox4In(3 downto 0) select sbox4Out(3 downto 0) <=
//         "0110" when "0000",
//         "1011" when "0001",
//         "0101" when "0010",
//         "0100" when "0011",
//         "0010" when "0100",
//         "1110" when "0101",
//         "0111" when "0110",
//         "1010" when "0111",
//         "1001" when "1000",
//         "1101" when "1001",
//         "1111" when "1010",
//         "1100" when "1011",
//         "0011" when "1100",
//         "0001" when "1101",
//         "0000" when "1110",
//         "1000" when "1111",
//         "XXXX" when others;
    
// end Behavioral;

SBOX4::SBOX4(){}

SBOX4::~SBOX4(){}

void SBOX4::Help()
{
    printf("**********************************************************\n");
    printf("**********************************************************\n");
    printf("Calling SBOX4 Help. Following are instructions to SBOX4 Instance Usage:\n");
    printf("1. inst.Init() method:\n");
    printf("Configure the SBOX4 inst.\n");
    printf("Initial Parameters: Input Rate (Number bit length/4), Initial Internal State, Instance Name.\n");

    printf("2. inst.NumGen() method:\n");
    printf("Generate the scrambled random number on top of input random number to inst.NumGen().\n");

    printf("3. inst.OutNum() method:\n");
    printf("Return the vector containing random number sequence from inst.NumGen().\n");

    printf("4. inst.Report() method:\n");
    printf("Get the configuration of the current SBOX4 instance.\n");

    printf("5. inst.NumPrint() method:\n");
    printf("Print the output random number of current SBOX4 instance.\n");

    printf("6. inst.RatePrint() method:\n");
    printf("Print the number rate of current SBOX4 instance.\n");

    printf("7. inst.SeqLen() method:\n");
    printf("Return the length of the sequence of current SBOX4 instance.\n");
    printf("**********************************************************\n");
    printf("**********************************************************\n");
}

void SBOX4::Init(unsigned int param1, unsigned int param2, string param3)
{
    inBitLength = param1;
    if (inBitLength % 4 == 0)
    {
        inRate = inBitLength/4;
    }
    else
    {
        printf("Error: Input Length is not multiple of 4 for class SBOX4 instance.\n");
    }
    initState = param2 % (unsigned int)pow(2,inRate*4);
    boxState = initState;
    m_name = param3;
}

void SBOX4::Report()
{
    printf("Current SBOX4:\n");
    std::cout << "Instance name:            " << m_name << std::endl;
    printf("Number Rate:             0x%-x\n", inRate);
    printf("Initial State:           0x%-x\n", initState);
    printf("Current State:           0x%-x\n", boxState);
}

void SBOX4::NumPrint()
{
    printf("Calling NumPrint for SBOX4 instance: ");
    std::cout << m_name << std::endl;
    printf("%x\n", outNum);
}

void SBOX4::RatePrint()
{
    printf("Calling RatePrint for SBOX4 instance: ");
    std::cout << m_name << std::endl;
    printf("%u\n", inRate);
}

unsigned int SBOX4::OutNum()
{
    return outNum;
}

void SBOX4::NumGen(unsigned int param1)
{
    inNum = param1;
    static const unsigned int lut[16] = {LUT};
    // printf("%x\n", boxState);
    // printf("%x\n", inNum);
    boxState = boxState ^ inNum;
    // state split
    // store the splited 4-bit arrays
    // lower index stores lower 4-bit array
    // printf("sb1111\n");
    static vector<unsigned int> splited(inRate);
    // printf("sb2222\n");
    // printf("inRate:%u\n", inRate);
    // splited.resize(inRate);
    // inRate means how many 4-bit arrays in state
    // printf("%x\n", boxState);
    for (int i = 0; i < inRate; ++i)
    {
        splited[i] = boxState % 16;
        // printf("%x\n", lut[splited[i]]);
        // printf("%x\n", splited[i]);
        boxState = boxState >> 4;
    }

    // SBox4 conversion
    for (int i = 0; i < inRate; ++i)
    {
        // printf("%x\n", lut[splited[i]]);
        boxState = lut[splited[i]] * pow(2,i*4) + boxState;
        // printf("%x\n", boxState);
    }

    // Rotate to right by 1 bit

    // printf("%x\n", boxState);
    boxState = (boxState >> 1) + (boxState % 2) * pow(2,inRate*4-1);
    // printf("%x\n", boxState);
    outNum = boxState;
    boxState = boxState ^ inNum;
}

