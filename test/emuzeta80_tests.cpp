#include "emuzeta80_tests.h"
#include <stdio.h>
#include "gtest/gtest.h"

TEST_F(EmuZeta80Test, 00_NOP)
{
	cpu->memory->poke(0, 0x00);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
}

TEST_F(EmuZeta80Test, 01_LD_BC_NN)
{
	cpu->memory->poke(0, 0x01);
	cpu->memory->poke(1, 0x17);
	cpu->memory->poke(2, 0xD2);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->mainBank.bc.value, 0xD217);
}

TEST_F(EmuZeta80Test, 02_LD_BC_A)
{
	cpu->mainBank.af.bytes.H = 0x4f;
	cpu->mainBank.bc.value = 0x110B;
	cpu->memory->poke(0, 0x02);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x110B), 0x4f);
}

TEST_F(EmuZeta80Test, 03_INC_BC)
{
	cpu->mainBank.bc.value = 0x1004;
	cpu->memory->poke(0, 0x03);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->mainBank.bc.value, 0x1005);
}

TEST_F(EmuZeta80Test, 04_INC_B)
{
	cpu->mainBank.bc.bytes.H = 0xF1;
	cpu->memory->poke(0, 0x04);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0xF2);
}

TEST_F(EmuZeta80Test, 05_DEC_B)
{
	cpu->mainBank.bc.bytes.H = 0xF1;
	cpu->memory->poke(0, 0x05);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0xF0);
}

TEST_F(EmuZeta80Test, 06_LD_B_N)
{
	cpu->memory->poke(0, 0x06);
	cpu->memory->poke(1, 0x1B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x1B);
}

TEST_F(EmuZeta80Test, 07_RLCA)
{
	cpu->mainBank.af.bytes.H = 0b01101100;
	cpu->memory->poke(0, 0x07);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11011000);
}

TEST_F(EmuZeta80Test, 09_ADD_HL_BC)
{
	cpu->mainBank.hl.value = 0x2D4B;
	cpu->mainBank.bc.value = 0004;
	cpu->memory->poke(0, 0x09);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->mainBank.hl.value, 0x2D4F);
}

TEST_F(EmuZeta80Test, 0A_LD_A_BC)
{
	cpu->mainBank.bc.value = 0x1717;
	cpu->memory->poke(0x1717, 0xF7);
	cpu->memory->poke(0, 0x0A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0xF7);
}

TEST_F(EmuZeta80Test, 0B_DEC_BC)
{
	cpu->mainBank.bc.value = 0x4BF2;
	cpu->memory->poke(0, 0x0B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->mainBank.bc.value, 0x4BF1);
}

TEST_F(EmuZeta80Test, 0C_INC_C)
{
	cpu->mainBank.bc.bytes.L = 0xF1;
	cpu->memory->poke(0, 0x0C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0xF2);
}

TEST_F(EmuZeta80Test, 0D_DEC_C)
{
	cpu->mainBank.bc.bytes.L = 0xF1;
	cpu->memory->poke(0, 0x0D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0xF0);
}

TEST_F(EmuZeta80Test, 0E_LD_C_N)
{
	cpu->memory->poke(0, 0x0E);
	cpu->memory->poke(1, 0x1B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x1B);
}

TEST_F(EmuZeta80Test, 0F_RRCA)
{
	cpu->mainBank.af.bytes.H = 0b01101100;
	cpu->memory->poke(0, 0x0F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b00110110);
}

TEST_F(EmuZeta80Test, 10_DJNZ)
{
	cpu->mainBank.bc.bytes.H = 4;
	cpu->memory->poke(0, 0x10);
	cpu->memory->poke(1, 0x0B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 13);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 3);
	ASSERT_EQ(cpu->pc.value, 0x0C);
}

TEST_F(EmuZeta80Test, 11_LD_DE_NN)
{
	cpu->memory->poke(0, 0x11);
	cpu->memory->poke(1, 0x17);
	cpu->memory->poke(2, 0xD2);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->mainBank.de.value, 0xD217);
}

TEST_F(EmuZeta80Test, 12_LD_DE_A)
{
	cpu->mainBank.af.bytes.H = 0x4f;
	cpu->mainBank.de.value = 0x110B;
	cpu->memory->poke(0, 0x12);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x110B), 0x4f);
}

TEST_F(EmuZeta80Test, 13_INC_DE)
{
	cpu->mainBank.de.value = 0x1004;
	cpu->memory->poke(0, 0x13);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->mainBank.de.value, 0x1005);
}

TEST_F(EmuZeta80Test, 14_INC_D)
{
	cpu->mainBank.de.bytes.H = 0xF1;
	cpu->memory->poke(0, 0x14);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0xF2);
}

TEST_F(EmuZeta80Test, 15_DEC_D)
{
	cpu->mainBank.de.bytes.H = 0xF1;
	cpu->memory->poke(0, 0x15);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0xF0);
}

TEST_F(EmuZeta80Test, 16_LD_D_N)
{
	cpu->memory->poke(0, 0x16);
	cpu->memory->poke(1, 0x1B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x1B);
}

TEST_F(EmuZeta80Test, 17_RLA)
{
	cpu->mainBank.af.bytes.H = 0b01101100;
	cpu->memory->poke(0, 0x17);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11011000);
}

TEST_F(EmuZeta80Test, 18_JR_D)
{
	cpu->memory->poke(0, 0x18);
	cpu->memory->poke(1, 0x1F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 12);
	ASSERT_EQ(cpu->pc.value, 0x20);
}

TEST_F(EmuZeta80Test, 19_ADD_HL_DE)
{
	cpu->mainBank.hl.value = 0x2D4B;
	cpu->mainBank.de.value = 0004;
	cpu->memory->poke(0, 0x19);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->mainBank.hl.value, 0x2D4F);
}

TEST_F(EmuZeta80Test, 1A_LD_A_DE)
{
	cpu->mainBank.de.value = 0x1717;
	cpu->memory->poke(0x1717, 0xF7);
	cpu->memory->poke(0, 0x1A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0xF7);
}

TEST_F(EmuZeta80Test, 1B_DEC_DE)
{
	cpu->mainBank.de.value = 0x4BF2;
	cpu->memory->poke(0, 0x1B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->mainBank.de.value, 0x4BF1);
}

TEST_F(EmuZeta80Test, 1C_INC_E)
{
	cpu->mainBank.de.bytes.L = 0xF1;
	cpu->memory->poke(0, 0x1C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0xF2);
}

TEST_F(EmuZeta80Test, 1D_DEC_E)
{
	cpu->mainBank.de.bytes.L = 0xF1;
	cpu->memory->poke(0, 0x1D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0xF0);
}

TEST_F(EmuZeta80Test, 1E_LD_E_N)
{
	cpu->memory->poke(0, 0x1E);
	cpu->memory->poke(1, 0x1B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x1B);
}

TEST_F(EmuZeta80Test, 1F_RRA)
{
	cpu->mainBank.af.bytes.H = 0b01101100;
	cpu->memory->poke(0, 0x1F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b00110110);
}

TEST_F(EmuZeta80Test, 20_JR_NZ_D)
{
	cpu->memory->poke(0, 0x20);
	cpu->memory->poke(1, 0x1F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 12);
	ASSERT_EQ(cpu->pc.value, 0x20);
}

TEST_F(EmuZeta80Test, 21_LD_HL_NN)
{
	cpu->memory->poke(0, 0x21);
	cpu->memory->poke(1, 0x17);
	cpu->memory->poke(2, 0xD2);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->mainBank.hl.value, 0xD217);
}

TEST_F(EmuZeta80Test, 22_LD_NN_HL)
{
	cpu->mainBank.hl.value = 0x0F22;
	cpu->memory->poke(0, 0x22);
	cpu->memory->poke(1, 0x0D);
	cpu->memory->poke(2, 0xF1);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 16);
	ASSERT_EQ(cpu->memory->peek(0xF10D), 0x22);
	ASSERT_EQ(cpu->memory->peek(0xF10E), 0x0F);
}

TEST_F(EmuZeta80Test, 23_INC_HL)
{
	cpu->mainBank.hl.value = 0x1004;
	cpu->memory->poke(0, 0x23);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->mainBank.hl.value, 0x1005);
}

TEST_F(EmuZeta80Test, 24_INC_H)
{
	cpu->mainBank.hl.bytes.H = 0xF1;
	cpu->memory->poke(0, 0x24);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0xF2);
}

TEST_F(EmuZeta80Test, 25_DEC_H)
{
	cpu->mainBank.hl.bytes.H = 0xF1;
	cpu->memory->poke(0, 0x25);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0xF0);
}

TEST_F(EmuZeta80Test, 26_LD_H_N)
{
	cpu->memory->poke(0, 0x26);
	cpu->memory->poke(1, 0x1B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x1B);
}

TEST_F(EmuZeta80Test, 28_JR_Z_D)
{
	cpu->mainBank.af.bytes.L = 0b01000000;
	cpu->memory->poke(0, 0x28);
	cpu->memory->poke(1, 0x1F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 12);
	ASSERT_EQ(cpu->pc.value, 0x20);
}

TEST_F(EmuZeta80Test, 29_ADD_HL_HL)
{
	cpu->mainBank.hl.value = 0x01F2;
	cpu->memory->poke(0, 0x29);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->mainBank.hl.value, 0x03E4);
}

TEST_F(EmuZeta80Test, 2A_LD_HL_NN)
{
	cpu->memory->poke(0x041A, 0x17);
	cpu->memory->poke(0x041B, 0xFB);
	cpu->memory->poke(0, 0x2A);
	cpu->memory->poke(1, 0x1A);
	cpu->memory->poke(2, 0x04);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 16);
	ASSERT_EQ(cpu->mainBank.hl.value, 0xFB17);
}

TEST_F(EmuZeta80Test, 2B_DEC_HL)
{
	cpu->mainBank.hl.value = 0x4BF2;
	cpu->memory->poke(0, 0x2B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->mainBank.hl.value, 0x4BF1);
}

TEST_F(EmuZeta80Test, 2C_INC_L)
{
	cpu->mainBank.hl.bytes.L = 0xF1;
	cpu->memory->poke(0, 0x2C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0xF2);
}

TEST_F(EmuZeta80Test, 2D_DEC_L)
{
	cpu->mainBank.hl.bytes.L = 0xF1;
	cpu->memory->poke(0, 0x2D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0xF0);
}

TEST_F(EmuZeta80Test, 2E_LD_L_N)
{
	cpu->memory->poke(0, 0x2E);
	cpu->memory->poke(1, 0x1B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x1B);
}

TEST_F(EmuZeta80Test, 2F_CPL)
{
	cpu->mainBank.af.bytes.H = 0b01101100;
	cpu->memory->poke(0, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010011);
}

TEST_F(EmuZeta80Test, 30_JR_NC_D)
{
	cpu->memory->poke(0, 0x30);
	cpu->memory->poke(1, 0x1F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 12);
	ASSERT_EQ(cpu->pc.value, 0x20);
}

TEST_F(EmuZeta80Test, 31_LD_SP_NN)
{
	cpu->memory->poke(0, 0x31);
	cpu->memory->poke(1, 0x17);
	cpu->memory->poke(2, 0xD2);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->sp.value, 0xD217);
}

TEST_F(EmuZeta80Test, 32_LD_NN_A)
{
	cpu->mainBank.af.bytes.H = 0x4F;
	cpu->memory->poke(0, 0x32);
	cpu->memory->poke(1, 0x0D);
	cpu->memory->poke(2, 0xF1);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 13);
	ASSERT_EQ(cpu->memory->peek(0xF10D), 0x4F);
}

TEST_F(EmuZeta80Test, 33_INC_SP)
{
	cpu->sp.value = 0x1004;
	cpu->memory->poke(0, 0x33);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->sp.value, 0x1005);
}

TEST_F(EmuZeta80Test, 34_INC_mHL)
{
	cpu->mainBank.hl.value = 0x0001;
	cpu->memory->poke(0, 0x34);
	cpu->memory->poke(1, 0x4A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->memory->peek(0x0001), 0x4B);
}

TEST_F(EmuZeta80Test, 35_DEC_mHL)
{
	cpu->mainBank.hl.value = 0x0001;
	cpu->memory->poke(0, 0x35);
	cpu->memory->poke(1, 0x4A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->memory->peek(0x0001), 0x49);
}

TEST_F(EmuZeta80Test, 36_LD_mHL_N)
{
	cpu->mainBank.hl.value = 0x0F25;
	cpu->memory->poke(0, 0x36);
	cpu->memory->poke(1, 0x0D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->memory->peek(0x0F25), 0x0D);
}

TEST_F(EmuZeta80Test, 37_SCF)
{
	cpu->memory->poke(0, 0x37);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b00000001);
}

TEST_F(EmuZeta80Test, 38_JR_C_D)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->memory->poke(0, 0x38);
	cpu->memory->poke(1, 0x10);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 12);
	ASSERT_EQ(cpu->pc.value, 0x11);
}

TEST_F(EmuZeta80Test, 39_ADD_HL_SP)
{
	cpu->mainBank.hl.value = 0x01F2;
	cpu->sp.value = 0x0020;
	cpu->memory->poke(0, 0x39);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->mainBank.hl.value, 0x0212);
}

TEST_F(EmuZeta80Test, 3A_LD_A_NN)
{
	cpu->memory->poke(0x041A, 0x17);
	cpu->memory->poke(0, 0x3A);
	cpu->memory->poke(1, 0x1A);
	cpu->memory->poke(2, 0x04);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 13);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x17);
}

TEST_F(EmuZeta80Test, 3B_DEC_SP)
{
	cpu->sp.value = 0x4BF2;
	cpu->memory->poke(0, 0x3B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->sp.value, 0x4BF1);
}

TEST_F(EmuZeta80Test, 3C_INC_A)
{
	cpu->mainBank.af.bytes.H = 0xF1;
	cpu->memory->poke(0, 0x3C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0xF2);
}

TEST_F(EmuZeta80Test, 3D_DEC_A)
{
	cpu->mainBank.af.bytes.H = 0xF1;
	cpu->memory->poke(0, 0x3D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0xF0);
}

TEST_F(EmuZeta80Test, 3E_LD_A_N)
{
	cpu->memory->poke(0, 0x3E);
	cpu->memory->poke(1, 0x1B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x1B);
}

TEST_F(EmuZeta80Test, 3F_CCF)
{
	cpu->mainBank.af.bytes.L = 0b00000000;
	cpu->memory->poke(0, 0x3F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b00000001);
}

TEST_F(EmuZeta80Test, 40_LD_B_B)
{
	cpu->mainBank.bc.bytes.H = 0x45;
	cpu->memory->poke(0, 0x40);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 41_LD_B_C)
{
	cpu->mainBank.bc.bytes.L = 0x45;
	cpu->memory->poke(0, 0x41);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 42_LD_B_D)
{
	cpu->mainBank.de.bytes.H = 0x45;
	cpu->memory->poke(0, 0x42);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 43_LD_B_E)
{
	cpu->mainBank.de.bytes.L = 0x45;
	cpu->memory->poke(0, 0x43);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 44_LD_B_H)
{
	cpu->mainBank.hl.bytes.H = 0x45;
	cpu->memory->poke(0, 0x44);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 45_LD_B_L)
{
	cpu->mainBank.hl.bytes.L = 0x45;
	cpu->memory->poke(0, 0x45);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 46_LD_B_mHL)
{
	cpu->mainBank.hl.value = 0x40B1;
	cpu->memory->poke(0x40B1, 0x45);
	cpu->memory->poke(0, 0x46);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 47_LD_B_A)
{
	cpu->mainBank.af.bytes.H = 0x45;
	cpu->memory->poke(0, 0x47);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 48_LD_C_B)
{
	cpu->mainBank.bc.bytes.H = 0x45;
	cpu->memory->poke(0, 0x48);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 49_LD_C_C)
{
	cpu->mainBank.bc.bytes.L = 0x45;
	cpu->memory->poke(0, 0x49);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 4A_LD_C_D)
{
	cpu->mainBank.de.bytes.H = 0x45;
	cpu->memory->poke(0, 0x4A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 4B_LD_C_E)
{
	cpu->mainBank.de.bytes.L = 0x45;
	cpu->memory->poke(0, 0x4B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 4C_LD_C_H)
{
	cpu->mainBank.hl.bytes.H = 0x45;
	cpu->memory->poke(0, 0x4C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 4D_LD_C_L)
{
	cpu->mainBank.hl.bytes.L = 0x45;
	cpu->memory->poke(0, 0x4D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 4E_LD_C_mHL)
{
	cpu->mainBank.hl.value = 0x40B1;
	cpu->memory->poke(0x40B1, 0x45);
	cpu->memory->poke(0, 0x4E);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 4F_LD_C_A)
{
	cpu->mainBank.af.bytes.H = 0x45;
	cpu->memory->poke(0, 0x4F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 50_LD_D_B)
{
	cpu->mainBank.bc.bytes.H = 0x45;
	cpu->memory->poke(0, 0x50);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 51_LD_D_C)
{
	cpu->mainBank.bc.bytes.L = 0x45;
	cpu->memory->poke(0, 0x51);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 52_LD_D_D)
{
	cpu->mainBank.de.bytes.H = 0x45;
	cpu->memory->poke(0, 0x52);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 53_LD_D_E)
{
	cpu->mainBank.de.bytes.L = 0x45;
	cpu->memory->poke(0, 0x53);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 54_LD_D_H)
{
	cpu->mainBank.hl.bytes.H = 0x45;
	cpu->memory->poke(0, 0x54);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 55_LD_D_L)
{
	cpu->mainBank.hl.bytes.L = 0x45;
	cpu->memory->poke(0, 0x55);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 56_LD_D_mHL)
{
	cpu->mainBank.hl.value = 0x40B1;
	cpu->memory->poke(0x40B1, 0x45);
	cpu->memory->poke(0, 0x56);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 57_LD_D_A)
{
	cpu->mainBank.af.bytes.H = 0x45;
	cpu->memory->poke(0, 0x57);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 58_LD_E_B)
{
	cpu->mainBank.bc.bytes.H = 0x45;
	cpu->memory->poke(0, 0x58);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 59_LD_E_C)
{
	cpu->mainBank.bc.bytes.L = 0x45;
	cpu->memory->poke(0, 0x59);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 5A_LD_E_D)
{
	cpu->mainBank.de.bytes.H = 0x45;
	cpu->memory->poke(0, 0x5A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 5B_LD_E_E)
{
	cpu->mainBank.de.bytes.L = 0x45;
	cpu->memory->poke(0, 0x5B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 5C_LD_E_H)
{
	cpu->mainBank.hl.bytes.H = 0x45;
	cpu->memory->poke(0, 0x5C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 5D_LD_E_L)
{
	cpu->mainBank.hl.bytes.L = 0x45;
	cpu->memory->poke(0, 0x5D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 5E_LD_E_mHL)
{
	cpu->mainBank.hl.value = 0x40B1;
	cpu->memory->poke(0x40B1, 0x45);
	cpu->memory->poke(0, 0x5E);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 5F_LD_E_A)
{
	cpu->mainBank.af.bytes.H = 0x45;
	cpu->memory->poke(0, 0x5F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 60_LD_H_B)
{
	cpu->mainBank.bc.bytes.H = 0x45;
	cpu->memory->poke(0, 0x60);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 61_LD_H_C)
{
	cpu->mainBank.bc.bytes.L = 0x45;
	cpu->memory->poke(0, 0x61);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 62_LD_H_D)
{
	cpu->mainBank.de.bytes.H = 0x45;
	cpu->memory->poke(0, 0x62);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 63_LD_H_E)
{
	cpu->mainBank.de.bytes.L = 0x45;
	cpu->memory->poke(0, 0x63);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 64_LD_H_H)
{
	cpu->mainBank.hl.bytes.H = 0x45;
	cpu->memory->poke(0, 0x64);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 65_LD_H_L)
{
	cpu->mainBank.hl.bytes.L = 0x45;
	cpu->memory->poke(0, 0x65);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 66_LD_H_mHL)
{
	cpu->mainBank.hl.value = 0x40B1;
	cpu->memory->poke(0x40B1, 0x45);
	cpu->memory->poke(0, 0x66);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 67_LD_H_A)
{
	cpu->mainBank.af.bytes.H = 0x45;
	cpu->memory->poke(0, 0x67);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 68_LD_L_B)
{
	cpu->mainBank.bc.bytes.H = 0x45;
	cpu->memory->poke(0, 0x68);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 69_LD_L_C)
{
	cpu->mainBank.bc.bytes.L = 0x45;
	cpu->memory->poke(0, 0x69);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 6A_LD_L_D)
{
	cpu->mainBank.de.bytes.H = 0x45;
	cpu->memory->poke(0, 0x6A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 6B_LD_L_E)
{
	cpu->mainBank.de.bytes.L = 0x45;
	cpu->memory->poke(0, 0x6B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 6C_LD_L_H)
{
	cpu->mainBank.hl.bytes.H = 0x45;
	cpu->memory->poke(0, 0x6C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 6D_LD_L_L)
{
	cpu->mainBank.hl.bytes.L = 0x45;
	cpu->memory->poke(0, 0x6D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 6E_LD_L_mHL)
{
	cpu->mainBank.hl.value = 0x40B1;
	cpu->memory->poke(0x40B1, 0x45);
	cpu->memory->poke(0, 0x6E);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 6F_LD_L_A)
{
	cpu->mainBank.af.bytes.H = 0x45;
	cpu->memory->poke(0, 0x6F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x45);
}

TEST_F(EmuZeta80Test, 70_LD_mH_B)
{
	cpu->mainBank.hl.value = 0x2B04;
	cpu->mainBank.bc.bytes.H = 0x45;
	cpu->memory->poke(0, 0x70);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x2B04), 0x45);
}

TEST_F(EmuZeta80Test, 71_LD_mH_C)
{
	cpu->mainBank.hl.value = 0x2B04;
	cpu->mainBank.bc.bytes.L = 0x45;
	cpu->memory->poke(0, 0x71);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x2B04), 0x45);
}

TEST_F(EmuZeta80Test, 72_LD_mH_D)
{
	cpu->mainBank.hl.value = 0x2B04;
	cpu->mainBank.de.bytes.H = 0x45;
	cpu->memory->poke(0, 0x72);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x2B04), 0x45);
}

TEST_F(EmuZeta80Test, 73_LD_mH_E)
{
	cpu->mainBank.hl.value = 0x2B04;
	cpu->mainBank.de.bytes.L = 0x45;
	cpu->memory->poke(0, 0x73);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x2B04), 0x45);
}

TEST_F(EmuZeta80Test, 74_LD_mH_H)
{
	cpu->mainBank.hl.value = 0x2B04;
	cpu->memory->poke(0, 0x74);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x2B04), 0x2B);
}

TEST_F(EmuZeta80Test, 75_LD_mH_L)
{
	cpu->mainBank.hl.value = 0x2B04;
	cpu->memory->poke(0, 0x75);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x2B04), 0x04);
}

TEST_F(EmuZeta80Test, 76_HALT)
{
	cpu->memory->poke(0, 0x76);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
}

TEST_F(EmuZeta80Test, 77_LD_mH_A)
{
	cpu->mainBank.hl.value = 0x2B04;
	cpu->mainBank.af.bytes.H = 0x45;
	cpu->memory->poke(0, 0x77);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->memory->peek(0x2B04), 0x45);
}

TEST_F(EmuZeta80Test, 78_LD_A_B)
{
	cpu->mainBank.bc.bytes.H = 0x45;
	cpu->memory->poke(0, 0x78);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 79_LD_A_C)
{
	cpu->mainBank.bc.bytes.L = 0x45;
	cpu->memory->poke(0, 0x79);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 7A_LD_A_D)
{
	cpu->mainBank.de.bytes.H = 0x45;
	cpu->memory->poke(0, 0x7A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 7B_LD_A_E)
{
	cpu->mainBank.de.bytes.L = 0x45;
	cpu->memory->poke(0, 0x7B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 7C_LD_A_H)
{
	cpu->mainBank.hl.bytes.H = 0x45;
	cpu->memory->poke(0, 0x7C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 7D_LD_A_L)
{
	cpu->mainBank.hl.bytes.L = 0x45;
	cpu->memory->poke(0, 0x7D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 7E_LD_A_mHL)
{
	cpu->mainBank.hl.value = 0x40B1;
	cpu->memory->poke(0x40B1, 0x45);
	cpu->memory->poke(0, 0x7E);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 7F_LD_A_A)
{
	cpu->mainBank.af.bytes.H = 0x45;
	cpu->memory->poke(0, 0x7F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x45);
}

TEST_F(EmuZeta80Test, 80_ADD_A_B)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.bc.bytes.H = 0x05;
	cpu->memory->poke(0, 0x80);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2C);
}

TEST_F(EmuZeta80Test, 81_ADD_A_C)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.bc.bytes.L = 0x05;
	cpu->memory->poke(0, 0x81);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2C);
}

TEST_F(EmuZeta80Test, 82_ADD_A_D)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.de.bytes.H = 0x05;
	cpu->memory->poke(0, 0x82);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2C);
}

TEST_F(EmuZeta80Test, 83_ADD_A_E)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.de.bytes.L = 0x05;
	cpu->memory->poke(0, 0x83);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2C);
}

TEST_F(EmuZeta80Test, 84_ADD_A_H)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.hl.bytes.H = 0x05;
	cpu->memory->poke(0, 0x84);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2C);
}

TEST_F(EmuZeta80Test, 85_ADD_A_L)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.hl.bytes.L = 0x05;
	cpu->memory->poke(0, 0x85);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2C);
}

TEST_F(EmuZeta80Test, 86_ADD_A_mHL)
{
	cpu->mainBank.af.bytes.H = 0x07;
	cpu->mainBank.hl.value = 0x51B2;
	cpu->memory->poke(0x51B2, 0x40);
	cpu->memory->poke(0, 0x86);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x47);
}

TEST_F(EmuZeta80Test, 87_ADD_A_A)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->memory->poke(0, 0x87);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x4E);
}

TEST_F(EmuZeta80Test, 88_ADC_A_B)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.bc.bytes.H = 0x05;
	cpu->memory->poke(0, 0x88);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2D);
}

TEST_F(EmuZeta80Test, 89_ADC_A_C)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.bc.bytes.L = 0x05;
	cpu->memory->poke(0, 0x89);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2D);
}

TEST_F(EmuZeta80Test, 8A_ADC_A_D)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.de.bytes.H = 0x05;
	cpu->memory->poke(0, 0x8A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2D);
}

TEST_F(EmuZeta80Test, 8B_ADC_A_E)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.de.bytes.L = 0x05;
	cpu->memory->poke(0, 0x8B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2D);
}

TEST_F(EmuZeta80Test, 8C_ADC_A_H)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.hl.bytes.H = 0x05;
	cpu->memory->poke(0, 0x8C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2D);
}

TEST_F(EmuZeta80Test, 8D_ADC_A_L)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.hl.bytes.L = 0x05;
	cpu->memory->poke(0, 0x8D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2D);
}

TEST_F(EmuZeta80Test, 8E_ADC_A_mHL)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x07;
	cpu->mainBank.hl.value = 0x51B2;
	cpu->memory->poke(0x51B2, 0x40);
	cpu->memory->poke(0, 0x8E);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x48);
}

TEST_F(EmuZeta80Test, 8F_ADC_A_A)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->memory->poke(0, 0x8F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x4F);
}

TEST_F(EmuZeta80Test, 90_SUB_A_B)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.bc.bytes.H = 0x05;
	cpu->memory->poke(0, 0x90);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x22);
}

TEST_F(EmuZeta80Test, 91_SUB_A_C)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.bc.bytes.L = 0x05;
	cpu->memory->poke(0, 0x91);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x22);
}

TEST_F(EmuZeta80Test, 92_SUB_A_D)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.de.bytes.H = 0x05;
	cpu->memory->poke(0, 0x92);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x22);
}

TEST_F(EmuZeta80Test, 93_SUB_A_E)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.de.bytes.L = 0x05;
	cpu->memory->poke(0, 0x93);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x22);
}

TEST_F(EmuZeta80Test, 94_SUB_A_H)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.hl.bytes.H = 0x05;
	cpu->memory->poke(0, 0x94);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x22);
}

TEST_F(EmuZeta80Test, 95_SUB_A_L)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.hl.bytes.L = 0x05;
	cpu->memory->poke(0, 0x95);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x22);
}

TEST_F(EmuZeta80Test, 96_SUB_A_mHL)
{
	cpu->mainBank.af.bytes.H = 0x47;
	cpu->mainBank.hl.value = 0x51B2;
	cpu->memory->poke(0x51B2, 0x40);
	cpu->memory->poke(0, 0x96);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x07);
}

TEST_F(EmuZeta80Test, 97_SUB_A_A)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->memory->poke(0, 0x97);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x00);
}

TEST_F(EmuZeta80Test, 98_SBC_A_B)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.bc.bytes.H = 0x05;
	cpu->memory->poke(0, 0x98);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x21);
}

TEST_F(EmuZeta80Test, 99_SBC_A_C)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.bc.bytes.L = 0x05;
	cpu->memory->poke(0, 0x99);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x21);
}

TEST_F(EmuZeta80Test, 9A_SBC_A_D)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.de.bytes.H = 0x05;
	cpu->memory->poke(0, 0x9A);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x21);
}

TEST_F(EmuZeta80Test, 9B_SBC_A_E)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.de.bytes.L = 0x05;
	cpu->memory->poke(0, 0x9B);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x21);
}

TEST_F(EmuZeta80Test, 9C_SBC_A_H)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.hl.bytes.H = 0x05;
	cpu->memory->poke(0, 0x9C);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x21);
}

TEST_F(EmuZeta80Test, 9D_SBC_A_L)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->mainBank.hl.bytes.L = 0x05;
	cpu->memory->poke(0, 0x9D);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x21);
}

TEST_F(EmuZeta80Test, 9E_SBC_A_mHL)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x47;
	cpu->mainBank.hl.value = 0x51B2;
	cpu->memory->poke(0x51B2, 0x07);
	cpu->memory->poke(0, 0x9E);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x3F);
}

TEST_F(EmuZeta80Test, 9F_SBC_A_A)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->memory->poke(0, 0x9F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0xFF);
}

TEST_F(EmuZeta80Test, A0_AND_A_B)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.bc.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xA0);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10000100);
}

TEST_F(EmuZeta80Test, A1_AND_A_C)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.bc.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xA1);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10000100);
}

TEST_F(EmuZeta80Test, A2_AND_A_D)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.de.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xA2);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10000100);
}

TEST_F(EmuZeta80Test, A3_AND_A_E)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.de.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xA3);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10000100);
}

TEST_F(EmuZeta80Test, A4_AND_A_H)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xA4);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10000100);
}

TEST_F(EmuZeta80Test, A5_AND_A_L)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xA5);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10000100);
}

TEST_F(EmuZeta80Test, A6_AND_A_mHL)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.value = 0x51B2;
	cpu->memory->poke(0x51B2, 0b11000110);
	cpu->memory->poke(0, 0xA6);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10000100);
}

TEST_F(EmuZeta80Test, A7_AND_A_A)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->memory->poke(0, 0xA7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
}

TEST_F(EmuZeta80Test, A8_XOR_A_B)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.bc.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xA8);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b01010011);
}

TEST_F(EmuZeta80Test, A9_XOR_A_C)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.bc.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xA9);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b01010011);
}

TEST_F(EmuZeta80Test, AA_XOR_A_D)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.de.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xAA);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b01010011);
}

TEST_F(EmuZeta80Test, AB_XOR_A_E)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.de.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xAB);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b01010011);
}

TEST_F(EmuZeta80Test, AC_XOR_A_H)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xAC);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b01010011);
}

TEST_F(EmuZeta80Test, AD_XOR_A_L)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xAD);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b01010011);
}

TEST_F(EmuZeta80Test, AE_XOR_A_mHL)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.value = 0x51B2;
	cpu->memory->poke(0x51B2, 0b11000110);
	cpu->memory->poke(0, 0xAE);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b01010011);
}

TEST_F(EmuZeta80Test, AF_XOR_A_A)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->memory->poke(0, 0xAF);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b00000000);
}

TEST_F(EmuZeta80Test, B0_OR_A_B)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.bc.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xB0);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11010111);
}

TEST_F(EmuZeta80Test, B1_OR_A_C)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.bc.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xB1);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11010111);
}

TEST_F(EmuZeta80Test, B2_OR_A_D)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.de.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xB2);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11010111);
}

TEST_F(EmuZeta80Test, B3_OR_A_E)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.de.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xB3);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11010111);
}

TEST_F(EmuZeta80Test, B4_OR_A_H)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xB4);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11010111);
}

TEST_F(EmuZeta80Test, B5_OR_A_L)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xB5);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11010111);
}

TEST_F(EmuZeta80Test, B6_OR_A_mHL)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.value = 0x51B2;
	cpu->memory->poke(0x51B2, 0b11000110);
	cpu->memory->poke(0, 0xB6);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11010111);
}

TEST_F(EmuZeta80Test, B7_OR_A_A)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->memory->poke(0, 0xB7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
}

TEST_F(EmuZeta80Test, B8_CP_A_B)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.bc.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xB8);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b10000011);
}

TEST_F(EmuZeta80Test, B9_CP_A_C)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.bc.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xB9);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b10000011);
}

TEST_F(EmuZeta80Test, BA_CP_A_D)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.de.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xBA);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b10000011);
}

TEST_F(EmuZeta80Test, BB_CP_A_E)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.de.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xBB);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b10000011);
}

TEST_F(EmuZeta80Test, BC_CP_A_H)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.bytes.H = 0b11000110;
	cpu->memory->poke(0, 0xBC);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b10000011);
}

TEST_F(EmuZeta80Test, BD_CP_A_L)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.bytes.L = 0b11000110;
	cpu->memory->poke(0, 0xBD);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b10000011);
}

TEST_F(EmuZeta80Test, BE_CP_A_mHL)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->mainBank.hl.value = 0x51B2;
	cpu->memory->poke(0x51B2, 0b11000110);
	cpu->memory->poke(0, 0xBE);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b10000011);
}

TEST_F(EmuZeta80Test, BF_CP_A_A)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->memory->poke(0, 0xBF);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b01000010);
}

TEST_F(EmuZeta80Test, C0_RET_NZ)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xC0);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, C1_POP_BC)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xC1);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->mainBank.bc.bytes.H, 0x17);
	ASSERT_EQ(cpu->mainBank.bc.bytes.L, 0x4F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, C2_JP_NZ_NN)
{
	cpu->memory->poke(0, 0xC2);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, C3_JP_NN)
{
	cpu->memory->poke(0, 0xC3);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, C4_CALL_NZ_NN)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xC4);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, C5_PUSH_BC)
{
	cpu->sp.value = 0x8000;
	cpu->mainBank.bc.value = 0x17B4;
	cpu->memory->poke(0, 0xC5);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x17);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0xB4);
}

TEST_F(EmuZeta80Test, C6_ADD_A_N)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->memory->poke(0, 0xC6);
	cpu->memory->poke(1, 0x05);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2C);
}

TEST_F(EmuZeta80Test, C7_RST_00)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xC7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x0000);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x01);
}

TEST_F(EmuZeta80Test, C8_RET_Z)
{
	cpu->mainBank.af.bytes.L = 0b01000000;
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xC8);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, C9_RET)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xC9);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, CA_JP_Z_NN)
{
	cpu->mainBank.af.bytes.L = 0b01000000;
	cpu->memory->poke(0, 0xCA);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, CC_CALL_Z_NN)
{
	cpu->mainBank.af.bytes.L = 0b01000000;
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xCC);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, CD_CALL_NN)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xCD);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, CE_ADC_A_N)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->memory->poke(0, 0xCE);
	cpu->memory->poke(1, 0x05);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x2D);
}

TEST_F(EmuZeta80Test, CF_RST_08)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xCF);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x0008);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x01);
}

TEST_F(EmuZeta80Test, D0_RET_NC)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xD0);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, D1_POP_DE)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xD1);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->mainBank.de.bytes.H, 0x17);
	ASSERT_EQ(cpu->mainBank.de.bytes.L, 0x4F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, D2_JP_NC_NN)
{
	cpu->memory->poke(0, 0xD2);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, D4_CALL_NC_NN)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xD4);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, D5_PUSH_DE)
{
	cpu->sp.value = 0x8000;
	cpu->mainBank.de.value = 0x17B4;
	cpu->memory->poke(0, 0xD5);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x17);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0xB4);
}

TEST_F(EmuZeta80Test, D6_SUB_A_N)
{
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->memory->poke(0, 0xD6);
	cpu->memory->poke(1, 0x05);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x22);
}

TEST_F(EmuZeta80Test, D7_RST_10)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x0010);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x01);
}

TEST_F(EmuZeta80Test, D8_RET_C)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xD8);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, DA_JP_Z_NN)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->memory->poke(0, 0xDA);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, DC_CALL_C_NN)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xDC);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, DE_SBC_A_N)
{
	cpu->mainBank.af.bytes.L = 0b00000001;
	cpu->mainBank.af.bytes.H = 0x27;
	cpu->memory->poke(0, 0xDE);
	cpu->memory->poke(1, 0x05);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x21);
}

TEST_F(EmuZeta80Test, DF_RST_18)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xDF);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x0018);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x01);
}

TEST_F(EmuZeta80Test, E0_RET_PO)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xE0);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, E1_POP_HL)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xE1);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->mainBank.hl.bytes.H, 0x17);
	ASSERT_EQ(cpu->mainBank.hl.bytes.L, 0x4F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, E2_JP_PO_NN)
{
	cpu->memory->poke(0, 0xE2);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, E3_EX_mSP_HL)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0xF2);
	cpu->memory->poke(0x8001, 0xAB);
	cpu->mainBank.hl.value = 0x14B2;
	cpu->memory->poke(0, 0xE3);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 19);
	ASSERT_EQ(cpu->memory->peek(0x8000), 0xB2);
	ASSERT_EQ(cpu->memory->peek(0x8001), 0x14);
	ASSERT_EQ(cpu->mainBank.hl.value, 0xABF2);
}

TEST_F(EmuZeta80Test, E4_CALL_PO_NN)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xE4);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, E5_PUSH_HL)
{
	cpu->sp.value = 0x8000;
	cpu->mainBank.hl.value = 0x17B4;
	cpu->memory->poke(0, 0xE5);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x17);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0xB4);
}

TEST_F(EmuZeta80Test, E6_AND_A_N)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->memory->poke(0, 0xE6);
	cpu->memory->poke(1, 0b11000110);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10000100);
}

TEST_F(EmuZeta80Test, E7_RST_20)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xE7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x0020);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x01);
}

TEST_F(EmuZeta80Test, E8_RET_PE)
{
	cpu->mainBank.af.bytes.L = 0b00000100;
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xE8);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, E9_JP_mHL)
{
	cpu->mainBank.hl.value = 0x2040;
	cpu->memory->poke(0x2040, 0x30);
	cpu->memory->poke(0x2041, 0x5B);
	cpu->memory->poke(0, 0xE9);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x5B30);
}

TEST_F(EmuZeta80Test, EA_JP_PE_NN)
{
	cpu->mainBank.af.bytes.L = 0b00000100;
	cpu->memory->poke(0, 0xEA);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, EB_EX_DE_HL)
{
	cpu->mainBank.de.value = 0xAD45;
	cpu->mainBank.hl.value = 0x14B2;
	cpu->memory->poke(0, 0xEB);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 4);
	ASSERT_EQ(cpu->mainBank.de.value, 0x14B2);
	ASSERT_EQ(cpu->mainBank.hl.value, 0xAD45);
}

TEST_F(EmuZeta80Test, EC_CALL_PE_NN)
{
	cpu->mainBank.af.bytes.L = 0b00000100;
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xEC);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, EE_XOR_A_N)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->memory->poke(0, 0xEE);
	cpu->memory->poke(1, 0b11000110);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b01010011);
}

TEST_F(EmuZeta80Test, EF_RST_28)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xEF);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x0028);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x01);
}

TEST_F(EmuZeta80Test, F0_RET_P)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xF0);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, F1_POP_AF)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xF1);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0x17);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0x4F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, F2_JP_P_NN)
{
	cpu->memory->poke(0, 0xF2);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, F4_CALL_P_NN)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xF4);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, F5_PUSH_AF)
{
	cpu->sp.value = 0x8000;
	cpu->mainBank.af.value = 0x17B4;
	cpu->memory->poke(0, 0xF5);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x17);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0xB4);
}

TEST_F(EmuZeta80Test, F6_OR_A_N)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->memory->poke(0, 0xF6);
	cpu->memory->poke(1, 0b11000110);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b11010111);
}

TEST_F(EmuZeta80Test, F7_RST_30)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xF7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x0030);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x01);
}

TEST_F(EmuZeta80Test, F8_RET_M)
{
	cpu->mainBank.af.bytes.L = 0b10000000;
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0x8000, 0x4F);
	cpu->memory->poke(0x8001, 0x17);
	cpu->memory->poke(0, 0xF8);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x174F);
	ASSERT_EQ(cpu->sp.value, 0x8002);
}

TEST_F(EmuZeta80Test, F9_LD_SP_HL)
{
	cpu->mainBank.hl.value = 0x1FB2;
	cpu->memory->poke(0, 0xF9);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 6);
	ASSERT_EQ(cpu->sp.value, 0x1FB2);
}

TEST_F(EmuZeta80Test, FA_JP_M_NN)
{
	cpu->mainBank.af.bytes.L = 0b10000000;
	cpu->memory->poke(0, 0xFA);
	cpu->memory->poke(1, 0x10);
	cpu->memory->poke(2, 0x2F);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 10);
	ASSERT_EQ(cpu->pc.value, 0x2F10);
}

TEST_F(EmuZeta80Test, FC_CALL_M_NN)
{
	cpu->mainBank.af.bytes.L = 0b10000000;
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xFC);
	cpu->memory->poke(1, 0x2B);
	cpu->memory->poke(2, 0xD7);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 17);
	ASSERT_EQ(cpu->pc.value, 0xD72B);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x03);
}

TEST_F(EmuZeta80Test, FE_CP_A_N)
{
	cpu->mainBank.af.bytes.H = 0b10010101;
	cpu->memory->poke(0, 0xFE);
	cpu->memory->poke(1, 0b11000110);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 7);
	ASSERT_EQ(cpu->mainBank.af.bytes.H, 0b10010101);
	ASSERT_EQ(cpu->mainBank.af.bytes.L, 0b10000011);
}

TEST_F(EmuZeta80Test, FF_RST_38)
{
	cpu->sp.value = 0x8000;
	cpu->memory->poke(0, 0xFF);
	cpu->execute();

	ASSERT_EQ(cpu->clockCycles, 11);
	ASSERT_EQ(cpu->pc.value, 0x0038);
	ASSERT_EQ(cpu->memory->peek(0x7FFF), 0x00);
	ASSERT_EQ(cpu->memory->peek(0x7FFE), 0x01);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleMock(&argc, argv);
     return RUN_ALL_TESTS();
}
