#ifndef TYPES
#define TYPES

#include<vector>
#include<algorithm>
#include<forward_list>
#include<map>
#include<iostream>
#include<sstream>
#include<random>
#include<thread>



typedef unsigned long long Bitboard;
typedef long long int_64;
typedef unsigned long long  uint_64;
typedef int int_32;
typedef unsigned int uint_32;
typedef unsigned char uint_8;
typedef char int_8;


enum MoveTypes {
	DEFAULT_MOVE = 0, CAPTURE, FIRST_PAWN_MOVE, EN_PASSANT, LONG_CASTLE, SHORT_CASTLE,
	TRANSFORM, CAPTURE_TRANSFORM
};



enum PieceTypes { KING = 0, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NO_TYPE };

enum PieceColors { WHITE = 0, BLACK, NO_COLOR };

constexpr int PieceCosts[7]{ 1000,9,5,3,3,1,0 };

struct TMove
{
	

	TMove() {
		from = to = moveType = opFigure = transformPiece =
			sortField = 0;
	}

	TMove operator=(TMove other)
	{
		from = other.from;
		to = other.to;
		moveType = other.moveType;
		opFigure = other.opFigure;
		transformPiece = other.transformPiece;
		sortField = other.sortField;
		return *this; // возвращает объект, который сгенерировал вызов
	}

	uint_8 from, to;
	uint_8 moveType;
	// PieceType of piece that have been eaten
	uint_8 opFigure;
	// in case CAPTURE_TRANSFORM contains the type of piece the pawn has become
	uint_8 transformPiece;
	int_32 sortField;
};

struct PosInfo {
	PosInfo() {
		fiftyMovesRule = 0;
		enPassantField = Bitboard(0);
		wShortCastle = wLongCastle = bShortCastle = bLongCastle = false;
	}
	uint_8 fiftyMovesRule;
	Bitboard enPassantField;
	bool wShortCastle, wLongCastle;
	bool bShortCastle, bLongCastle;
	TMove prevMove;
};




struct Piece
{
	Piece() {
		this->color = uint_8(PieceColors::NO_COLOR);
		this->type = uint_8(PieceTypes::NO_TYPE);
	}

	uint_8 color;
	uint_8 type;
};


struct Position
{
	
	Position() {
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				pos[i][j] = Bitboard(0);
			}
		}
		allBlackPeaces = allPeaces = allWhitePeaces = enPassantField = Bitboard(0);
		activeColor = NO_COLOR;
		wShortCastle = wLongCastle = bShortCastle = bLongCastle = false;
		fiftyMovesRule = moveCounter = material = 0;
		zobristHash = uint_64(0);
	}

	// Piece list
	Piece pieceHash[64];
	Bitboard pos[2][6]; // Position Description in Bitboards 
	Bitboard allBlackPeaces, allWhitePeaces, allPeaces; // Bitboard includes all white / black peaces
	Bitboard enPassantField;
	uint_8 activeColor;
	bool wShortCastle, wLongCastle;
	bool bShortCastle, bLongCastle;
	uint_8 fiftyMovesRule;
	uint_8 moveCounter;
	int_8 material; // position evaluation score (Including only figures) 
	TMove prevMove;
	uint_64 zobristHash; 
};


namespace MoveGenTypes {

	// Directions for Rays

	enum Directions { NORTH = 0, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST };


}

namespace SearchTypes {

}

namespace EvaluationTypes {
	typedef int_32 EvalScore;
}

namespace GameTypes{

}

namespace UCITypes {

}

namespace OpeningBookTypes {

}

namespace EndGameTableTypes {

}


#endif
