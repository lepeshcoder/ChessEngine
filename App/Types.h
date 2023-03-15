#ifndef TYPES
#define TYPES

#include<vector>
#include<algorithm>
#include<forward_list>
#include<map>
#include<iostream>


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

constexpr int PieceCosts[6]{ 1000,9,5,3,3,1 };


struct PosInfo {
	PosInfo() {
		fiftyMovesRule = 0;
		enPassantField = Bitboard(0);
	}

	uint_8 fiftyMovesRule;
	Bitboard enPassantField;
};

struct TMove
{

	TMove() {
		from = to = moveType = opFigure = transformPiece =
			sortField = 0;
	}

	uint_8 from, to;
	uint_8 moveType;
	// PieceType of piece that have been eaten
	uint_8 opFigure;
	// in case CAPTURE_TRANSFORM contains the type of piece the pawn has become
	uint_8 transformPiece;
	int_8 sortField;
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
		allBlackPeaces = allPeaces = allWhitePeaces = enPassantField = Bitboard(0);
		activeColor = NO_COLOR;
		wShortCastle = wLongCastle = bShortCastle = bLongCastle = false;
		fiftyMovesRule = moveCounter = material = 0;
	}

	// Piece list
	Piece pieceHash[64];
	Bitboard pos[2][6]; // Position Description in Bitboards 
	Bitboard allBlackPeaces, allWhitePeaces, allPeaces; // Bitboard includes all white / black peaces
	Bitboard enPassantField;
	PieceColors activeColor;
	bool wShortCastle, wLongCastle;
	bool bShortCastle, bLongCastle;
	uint_8 fiftyMovesRule;
	uint_8 moveCounter;
	int_8 material; // position evaluation score (Including only figures) 
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
