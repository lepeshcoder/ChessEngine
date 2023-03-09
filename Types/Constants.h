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

	// Represents chess board edges as Bitboard

	constexpr Bitboard EDGES = RANK_1 | RANK_8 | FILE_A | FILE_H;

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

	// Magic Array for BitScanForward and bitScanReverse 
	constexpr int index64[64] = {
	    0, 47,  1, 56, 48, 27,  2, 60,
       57, 49, 41, 37, 28, 16,  3, 61,
       54, 58, 35, 52, 50, 42, 21, 44,
       38, 32, 29, 23, 17, 11,  4, 62,
       46, 55, 26, 59, 40, 36, 15, 53,
	   34, 51, 20, 43, 31, 22, 10, 45,
	   25, 39, 14, 33, 19, 30,  9, 24,
       13, 18,  8, 12,  7,  6,  5, 63
	};


	// Magic mask for BitScanForward and bitScanReverse 
	constexpr Bitboard mask = Bitboard(0x03f79d71b4cb0a89);
							  
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

namespace TestConstants
{
	/*
	  0 0 0 0 0 0 0 0
	  0 0 0 1 0 0 0 0
	  0 0 0 0 0 0 1 0
	  0 0 0 0 1 0 0 0 
	  0 0 1 1 0 0 0 0
	  0 0 0 0 0 0 0 1
      1 1 0 0 0 1 0 0
      0 0 0 0 0 0 0 0
	*/
	constexpr Bitboard getPawnMovesCase1W = 0x1002083001C400;

	/*
	  0 0 0 1 0 0 0 0
      0 0 0 0 0 0 1 0
      0 0 0 0 1 0 0 0
      0 0 1 1 0 0 0 0
      1 1 0 0 0 1 0 1 
      1 1 0 0 0 1 0 0
      0 0 0 0 0 0 0 0
      0 0 0 0 0 0 0 0
	*/
	constexpr Bitboard getPawnMovesCase1WExpected = 0x10020830C5C40000;

	/*
	  0 0 0 0 0 0 0 0  
	  1 1 0 0 0 0 0 0    
	  0 0 1 0 0 0 0 0    
	  0 0 0 1 0 0 0 0    
	  0 0 0 0 0 1 0 0 
	  0 0 0 1 0 0 0 0   
	  0 0 0 0 0 0 0 1    
	  0 0 0 0 0 0 0 0    
	*/
	constexpr Bitboard getPawnMovesCase1B = 0xC0201004100100;

	/*
	  0 0 0 0 0 0 0 0
      0 0 0 0 0 0 0 0
      1 1 0 0 0 0 0 0
      1 1 1 0 0 0 0 0
	  0 0 0 1 0 0 0 0  
	  0 0 0 0 0 1 0 0
	  0 0 0 1 0 0 0 0
	  0 0 0 0 0 0 0 1
	*/
	constexpr Bitboard getPawnMovesCase1BExpected = 0xC0E010041001;


	/*
	* 
	*/
	constexpr Bitboard getPawnCapturesCase1B = 0x80200008010000;
	
	/*
	* 
	*/
	constexpr Bitboard getPawnCapturesCase1W = 0x5021084204800;
	
	/*
	* 
	*/
	constexpr Bitboard getPawnCapturesCase1WExpected = 0xA05284A50B40000;
	
	/*
	* 
	*/
	constexpr Bitboard getPawnCapturesCase1BExpected = 0x405000140200;


	/*
	  1 0 0 0 0 0 0 0    
	  0 0 0 0 0 0 0 0    
	  0 0 0 0 0 0 0 0    
	  0 0 0 0 0 0 0 0   
	  0 0 0 0 1 0 0 0   
	  0 0 0 0 0 0 0 0   
	  0 0 0 0 0 0 0 0   
	  0 0 0 0 0 0 0 0   
	*/
	constexpr Bitboard getKnightMovesCase1 = 0x8000000008000000;

	/*
      0 0 0 0 0 0 0 0
      0 0 1 0 0 0 0 0
      0 1 0 1 0 1 0 0
	  0 0 1 0 0 0 1 0
	  0 0 0 0 0 0 0 0 
	  0 0 1 0 0 0 1 0
      0 0 0 1 0 1 0 0
      0 0 0 0 0 0 0 0
	*/
	constexpr Bitboard getKnightMovesCase1Expected = 0x0020542200221400;


	/*
	  0 0 0 0 0 0 0 0    
	  0 0 0 0 0 0 0 0    
	  0 0 0 0 0 0 0 0   
	  0 0 0 0 0 0 0 0    
	  0 0 0 0 0 0 0 0    
	  0 0 0 0 1 0 0 0   
	  0 0 0 0 0 0 0 0    
	  0 0 0 0 0 0 0 1    
	*/
	constexpr Bitboard getKingMovesCase1 = 0x80001;

	/*
      0 0 0 0 0 0 0 0
      0 0 0 0 0 0 0 0
      0 0 0 0 0 0 0 0
      0 0 0 0 0 0 0 0
      0 0 0 1 1 1 0 0 
      0 0 0 1 0 1 0 0
      0 0 0 1 1 1 1 1
      0 0 0 0 0 0 1 0
	*/
	constexpr Bitboard getKingMovesCase1Expected = 0x1C141F02;
	

	/*
		   bishops          blockers           expected
	    0 0 0 0 0 0 0 0  0 0 0 0 1 0 0 0    0 0 0 0 1 0 0 0
		0 0 0 0 0 0 0 0  0 1 0 0 0 0 0 0    0 1 0 1 0 0 0 0
		0 0 1 0 0 0 0 0  0 1 0 0 0 0 0 0    0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0    0 1 0 1 0 0 0 0
		0 0 0 0 0 0 0 0  1 0 0 0 0 0 0 0    1 0 0 0 1 0 0 0 
		0 0 0 0 0 0 0 0  0 0 0 0 0 1 0 0    0 0 0 0 0 1 0 0
		0 0 0 0 0 0 0 0  0 0 0 0 0 1 0 0    0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0  0 0 1 0 0 0 0 0    0 0 0 0 0 0 0 0
          
	*/
	constexpr int getBishopMovesSq = 45;
	
	constexpr Bitboard getBishopMovesCase1Expected = 0x850005088040000;

	constexpr Bitboard getBishopMovesCase1Blockers = 0x840400080040420;


	/*
	        rook             blockers           expected
	   0 0 0 0 0 0 0 0    0 1 0 0 0 0 0 0    0 0 0 0 0 0 0 0 
	   0 0 0 0 0 0 0 0    1 0 1 0 1 0 0 0    0 0 1 0 0 0 0 0 
	   0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0    0 0 1 0 0 0 0 0 
	   0 0 1 0 0 0 0 0    0 1 0 0 0 1 0 0    0 1 0 1 1 1 0 0  
	   0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0    0 0 1 0 0 0 0 0 
	   0 0 0 0 0 0 0 0    0 0 0 0 1 0 0 0    0 0 1 0 0 0 0 0 
	   0 0 0 0 0 0 0 0    1 0 0 0 0 0 1 0    0 0 1 0 0 0 0 0 
	   0 0 0 0 0 0 0 0    0 0 1 0 0 0 0 0    0 0 1 0 0 0 0 0 
	*/
	constexpr int getRookMovesSq = 37;

	constexpr Bitboard getRookMovesCase1Blockers = 0x40A8004400088220;

	constexpr Bitboard getRookMovesCase1Expected = 0x20205C20202020;

	/*
		     queen             blockers          expected
		0 0 0 0 0 0 0 0    0 1 0 0 0 0 0 1    0 0 0 0 0 1 0 1 
		0 0 0 0 0 0 0 0    1 0 1 0 1 0 0 0    0 0 0 0 1 1 1 0 
		0 0 0 0 0 1 0 0    0 0 1 0 0 0 0 1    0 0 1 1 1 0 1 1 
		0 0 0 0 0 0 0 0    0 1 0 0 0 1 1 0    0 0 0 0 1 1 1 0 
		0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0    0 0 0 1 0 0 0 0 
		0 0 0 0 0 0 0 0    0 0 1 0 1 0 0 0    0 0 1 0 0 0 0 0 
		0 0 0 0 0 0 0 0    1 0 0 0 0 0 1 0    0 0 0 0 0 0 0 0 
		0 0 0 0 0 0 0 0    0 0 1 0 0 0 0 0    0 0 0 0 0 0 0 0 
     
	*/
	constexpr int getQueenMovesSq = 42;

	constexpr Bitboard getQueenMovesCase1Blockers = 0x41A8214600288220;

	constexpr Bitboard getQueenMovesCase1Expected = 0x50E3B0E10200000;
}

#endif 

