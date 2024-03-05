
#include "CPU.h"
#include "gmock/gmock.h"

class CPUClassTest : public ::emuzeta80::CPU
{
	FRIEND_TEST(EmuZeta80Test, NOP);
	FRIEND_TEST(EmuZeta80Test, LD_BC_MEM);
	FRIEND_TEST(EmuZeta80Test, LD_BC_A);
	FRIEND_TEST(EmuZeta80Test, INC_BC);
	FRIEND_TEST(EmuZeta80Test, INC_B_1);
	FRIEND_TEST(EmuZeta80Test, INC_B_2);
	FRIEND_TEST(EmuZeta80Test, INC_B_3);

	FRIEND_TEST(EmuZeta80Test, LD_MEM_HL);
	FRIEND_TEST(EmuZeta80Test, LD_HL_MEM);

public:
	CPUClassTest() : ::emuzeta80::CPU(16384) { }

};

class EmuZeta80Test : public ::testing::Test
{

protected:
	EmuZeta80Test()
	{
		cpu = new CPUClassTest();
	}
    ~EmuZeta80Test() { }

protected:
    CPUClassTest* cpu;
};
