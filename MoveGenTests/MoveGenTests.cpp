#include "pch.h"
#include"../NewEngine/MoveGen.h"
#include"../Types/Types.h"

TEST(bitScanReverse, defaultCase) {
	MoveGen moveGen;
	Bitboard bb = 7; // ..00111
	EXPECT_EQ(moveGen.bitScanReverse(bb), 2);
}

TEST(bitScanReverse, ZeroCase)
{
	MoveGen moveGen;
	Bitboard bb = 0;
	EXPECT_EQ(moveGen.bitScanReverse(bb), -1);
}

TEST(bitScanForward, defaultCase) {
	MoveGen moveGen;
	Bitboard bb = 7; // ..00111
	EXPECT_EQ(moveGen.bitScanForward(bb), 0);
}

TEST(bitScanForward, ZeroCase)
{
	MoveGen moveGen;
	Bitboard bb = 0;
	EXPECT_EQ(moveGen.bitScanReverse(bb), -1);
}

FRIEND_TEST()
