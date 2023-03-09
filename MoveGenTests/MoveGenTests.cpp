#include "pch.h"
#include"../NewEngine/MoveGen.h"
#include"../Types/Types.h"
#include"../Types/Constants.h"

using namespace TestConstants;


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

TEST(getPawnMoves, case1)
{
	MoveGen moveGen;

	EXPECT_EQ(moveGen.getPawnMoves(PieceColors::WHITE, getPawnMovesCase1W),
		getPawnMovesCase1WExpected);
	EXPECT_EQ(moveGen.getPawnMoves(PieceColors::BLACK, getPawnMovesCase1B), 
		getPawnMovesCase1BExpected);
}

TEST(getPawnCaptures, case1)
{
	MoveGen moveGen;
	EXPECT_EQ(moveGen.getPawnCaptures(PieceColors::WHITE, getPawnCapturesCase1W), 
		getPawnCapturesCase1WExpected);
	EXPECT_EQ(moveGen.getPawnCaptures(PieceColors::BLACK, getPawnCapturesCase1B), 
		getPawnCapturesCase1BExpected);
}

TEST(getKnightMoves, case1)
{
	MoveGen moveGen;
	EXPECT_EQ(moveGen.getKnightMoves(getKnightMovesCase1),getKnightMovesCase1Expected);
}

TEST(getKingMoves, case1)
{
	MoveGen moveGen;
	EXPECT_EQ(moveGen.getKingMoves(getKingMovesCase1), getKingMovesCase1Expected);
}

TEST(getBishopMoves, case1)
{
	MoveGen moveGen;
	EXPECT_EQ(moveGen.getBishopMoves(getBishopMovesSq, getBishopMovesCase1Blockers),
	   getBishopMovesCase1Expected);
}

TEST(getRookMoves, case1)
{
	MoveGen moveGen;
	EXPECT_EQ(moveGen.getRookMoves(getRookMovesSq, getRookMovesCase1Blockers),
		getRookMovesCase1Expected);
}

TEST(getQueenMoves, case1)
{
	MoveGen moveGen;
	EXPECT_EQ(moveGen.getQueenMoves(getQueenMovesSq, getQueenMovesCase1Blockers),
		getQueenMovesCase1Expected);
}