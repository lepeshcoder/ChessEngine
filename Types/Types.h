#ifndef TYPES
#define TYPES

#include<vector>
#include<algorithm>

typedef unsigned long long Bitboard;
typedef long long int_64;
typedef unsigned long long  uint_64;
typedef int int_32;
typedef unsigned int uint_32;
typedef unsigned char uint_8;
typedef char int_8;


enum MoveTypes { DEFAULT_MOVE = 0, CAPTURE, FIRST_PAWN_MOVE, EN_PASSANT, LONG_CASTLE, SHORT_CASTLE,
				 TRANSFORM, CAPTURE_TRANSFORM };

enum PieceTypes { KING = 0, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NOTHING };

enum PieceColors { WHITE = 0, BLACK };

struct TMove
{
	int_8 from, to;
	MoveTypes moveType;

	// PieceType of piece that have eaten
	PieceTypes opFigure; 
	// in case CAPTURE_TRANSFORM contains the type of piece the pawn has become
	PieceTypes transformPiece;
};

struct Position
{
	Bitboard pos[2][6]; // Position Description in Bitboards 
	Bitboard allBlackPeaces, allWhitePeaces; // Bitboard includes all white / black peaces
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
