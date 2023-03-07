#ifndef CONSTANTS 
#define CONSTANTS

#include "Types.h"

namespace MoveGenConstants
{
	// Representation Ranks on chessboard as Bitboards

	constexpr Bitboard RANK_1 = 0XFF;
	constexpr Bitboard RANK_2 = 0xFF00;
	constexpr Bitboard RANK_3 = 0XFF0000;
	constexpr Bitboard RANK_5 = 0XFF000000;
	constexpr Bitboard RANK_4 = 0XFF00000000;
	constexpr Bitboard RANK_6 = 0XFF0000000000;
	constexpr Bitboard RANK_7 = 0XFF000000000000;
	constexpr Bitboard RANK_8 = 0XFF00000000000000;

	// Representation Files on chessboard as Bitboards

	constexpr Bitboard FILE_A = 0X8080808080808080;
	constexpr Bitboard FILE_B = FILE_A >> 1;
	constexpr Bitboard FILE_C = FILE_A >> 2;
	constexpr Bitboard FILE_D = FILE_A >> 3;
	constexpr Bitboard FILE_E = FILE_A >> 4;
	constexpr Bitboard FILE_F = FILE_A >> 5;
	constexpr Bitboard FILE_G = FILE_A >> 6;
	constexpr Bitboard FILE_H = FILE_A >> 7;

	// Representation Diagonals as Bitboards

	constexpr Bitboard DIAG_A8H1 = 0X8040201008040201;
	constexpr Bitboard DIAG_A1H8 = 0X0102040810204080;

	// Arrays that represent quantity of cells attacking by rook/bishop/queen that placed on 1 of 64 square

	constexpr int rook_bits[64] = {
	12,11,11,11,11,11,11,12,
	11,10,10,10,10,10,10,11,
	11,10,10,10,10,10,10,11,
	11,10,10,10,10,10,10,11,
	11,10,10,10,10,10,10,11,
	11,10,10,10,10,10,10,11,
	11,10,10,10,10,10,10,11,
	12,11,11,11,11,11,11,12
	};

	constexpr int bishop_bits[64] = {
		6,5,5,5,5,5,5,6,
		5,5,5,5,5,5,5,5,
		5,5,7,7,7,7,5,5,
		5,5,7,9,9,7,5,5,
		5,5,7,9,9,7,5,5,
		5,5,7,7,7,7,5,5,
		5,5,5,5,5,5,5,5,
		6,5,5,5,5,5,5,6
	};

	constexpr int queen_bits[64] = {
	   18,16,16,16,16,16,16,18,
	   16,15,15,15,15,15,15,16,
	   16,15,17,17,17,17,15,16,
	   16,15,17,19,19,17,15,16,
	   16,15,17,19,19,17,15,16,
	   16,15,17,17,17,17,15,16,
	   16,15,15,15,15,15,15,16,
	   18,16,16,16,16,16,16,18,
	};

}

namespace EvaluationConstants
{

}

namespace SearchConstants
{

}

namespace UCIConstants
{

}

namespace OpeningBookConstants
{

}

namespace EndGameTableConstants
{

}

#endif // ! 
