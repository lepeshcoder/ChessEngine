#ifndef TYPES
#define TYPES

#include<vector>
#include<algorithm>
#include<forward_list>
#include<map>


typedef unsigned long long Bitboard;
typedef long long int_64;
typedef unsigned long long  uint_64;
typedef int int_32;
typedef unsigned int uint_32;
typedef unsigned char uint_8;
typedef char int_8;


enum MoveTypes { DEFAULT_MOVE = 0, CAPTURE, FIRST_PAWN_MOVE, EN_PASSANT, LONG_CASTLE, SHORT_CASTLE,
				 TRANSFORM, CAPTURE_TRANSFORM };

enum PieceTypes { KING = 0, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NO_TYPE };

enum PieceColors { WHITE = 0, BLACK, NO_COLOR };

constexpr int PieceCosts[6]{ 1000,9,5,3,3,1 };



// 6 bytes
struct TMove
{
	TMove(uint_8 from, uint_8 to, uint_8 movetype,
		uint_8 opfigure, uint_8 transformpiece, int_8 sortfield)
	{
		this->from = from;
		this->to = to;
		this->moveType = movetype;
		this->opFigure = opfigure;
		this->transformPiece = transformpiece;
		this->sortField = sortfield;
	}

	TMove(){
		from = to = moveType = opFigure = transformPiece = sortField = 0;
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
	Piece(uint_8 color,uint_8 type)
	{
		this->color = color;
		this->type = type;
	}

	Piece() {
		this->color = uint_8(PieceColors::NO_COLOR);
		this->type =uint_8(PieceTypes::NO_TYPE);
	}

	uint_8 color;
	uint_8 type;
};


struct Position
{
	// Piece list
	Piece pieceHash[64];
	Bitboard pos[2][6]; // Position Description in Bitboards 
	Bitboard allBlackPeaces, allWhitePeaces, allPeaces; // Bitboard includes all white / black peaces
	Bitboard enPassantField; 
	PieceColors activeColor;
	bool wShortCastle; // May black/white can do short/long castle
	bool bShortCastle;
	bool wLongCastle;
	bool bLongCastle;
	uint_8 fiftyMovesRule, prevFiftyMovesRule; 
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

namespace UCITypes {

}

namespace OpeningBookTypes {

}

namespace EndGameTableTypes {

}


#endif
