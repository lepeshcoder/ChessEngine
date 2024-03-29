#include"MoveGen.h"

using namespace MoveGen;

// Rays array includes rays form every cell in every direction (See DIRECTIONS in Types.h) 
Bitboard rays[64][8];

// Masks array includes all possible moves at empty board from every cell
Bitboard rookMasks[64];
Bitboard bishopMasks[64];
Bitboard queenMasks[64];

/* Caches arrays for sliding peaces includes
   all pseudolegal moves from every cell for every Blocker set */
Bitboard** bishopCache;
Bitboard** rookCache;
Bitboard** queenCache;

/* Caches arrays for non-sliding peaces include all possible moves
  at empty board for every cell */
Bitboard kingCache[64];
Bitboard knightCache[64];
Bitboard wPawnCache[64];
Bitboard bPawnCache[64];
Bitboard wPawnMoveCache[64];
Bitboard bPawnMoveCache[64];

uint_64 MoveGen::zobristCache[2][6][64] = {};



void MoveGen::InitMoveGen()
{
	initZobristCache();

	initRays();

	initRookMasks();
	initBishopMasks();
	initQueenMasks();

	bishopCache = new Bitboard * [64];
	for (int i = 0; i < 64; i++)
	{
		bishopCache[i] = new Bitboard[Bitboard(1) << bishop_bits[i]];
	}

	rookCache = new Bitboard * [64];
	for (int i = 0; i < 64; i++)
	{
		rookCache[i] = new Bitboard[Bitboard(1) << rook_bits[i]];
	}

	queenCache = new Bitboard * [64];
	for (int i = 0; i < 64; i++)
	{
		queenCache[i] = new Bitboard[Bitboard(1) << queen_bits[i]];
	}

	initPawnMoveCache();
	initPawnCaptureCache();
	initKnightCache();
	initKingCache();
	initBishopCache();
	initRookCache();
	initQueenCache();
}

void MoveGen::makeMove(Position& position, TMove move)
{
	
	int pieceType = position.pieceHash[move.from].type;
	int activeColor = position.activeColor;

	
	Game::positionInfo[position.moveCounter].enPassantField = position.enPassantField;
	Game::positionInfo[position.moveCounter].fiftyMovesRule = position.fiftyMovesRule;

	Game::positionInfo[position.moveCounter].wShortCastle = position.wShortCastle;
	Game::positionInfo[position.moveCounter].wLongCastle = position.wLongCastle;
	Game::positionInfo[position.moveCounter].bShortCastle = position.bShortCastle;
	Game::positionInfo[position.moveCounter].bLongCastle = position.bLongCastle;

	Game::positionInfo[position.moveCounter].prevMove = position.prevMove;

	++position.moveCounter;

	if (move.moveType == CAPTURE || move.moveType == CAPTURE_TRANSFORM || pieceType == PAWN) position.fiftyMovesRule = 0;
	else ++position.fiftyMovesRule;

	position.prevMove = move;

	position.enPassantField = Bitboard(0);

	// if rook goes king cant castle
	if (pieceType == ROOK)
	{
		if (activeColor == WHITE)
		{
			if (move.from == 0 && position.wShortCastle) position.wShortCastle = false;
			else if (move.from == 7 && position.wLongCastle) position.wLongCastle = false;
		}
		else
		{
			if (move.from == 56 && position.bShortCastle) position.bShortCastle = false;
			else if (move.from == 63 && position.bLongCastle) position.bLongCastle = false;
		}
	}


	switch (move.moveType)
	{
	case DEFAULT_MOVE:
	{
		position.pos[activeColor][pieceType] &= ~(Bitboard(1) << move.from);
		position.pos[activeColor][pieceType] |= (Bitboard(1) << move.to);
		position.pieceHash[move.from].type = PieceTypes::NO_TYPE;
		position.pieceHash[move.from].color = PieceColors::NO_COLOR;
		position.pieceHash[move.to].type = pieceType;
		position.pieceHash[move.to].color = activeColor;
		if (activeColor == WHITE)
		{
			position.allWhitePeaces &= ~(Bitboard(1) << move.from);
			position.allWhitePeaces |= (Bitboard(1) << move.to);
			if (pieceType == KING) position.wShortCastle = position.wLongCastle = false;
			position.zobristHash ^= zobristCache[WHITE][pieceType][move.from];
			position.zobristHash ^= zobristCache[WHITE][pieceType][move.to];
		}
		else
		{
			position.allBlackPeaces &= ~(Bitboard(1) << move.from);
			position.allBlackPeaces |= (Bitboard(1) << move.to);
			if (pieceType == KING) position.bShortCastle = position.bLongCastle = false;
			position.zobristHash ^= zobristCache[BLACK][pieceType][move.from];
			position.zobristHash ^= zobristCache[BLACK][pieceType][move.to];
		}
		break;
	}
	case  CAPTURE:
	{
		position.pos[activeColor][pieceType] &= ~(Bitboard(1) << move.from);
		position.pos[activeColor][pieceType] |= (Bitboard(1) << move.to);
		position.pieceHash[move.from].color = NO_COLOR;
		position.pieceHash[move.from].type = NO_TYPE;
		position.pieceHash[move.to].color = activeColor;
		position.pieceHash[move.to].type = pieceType;
		if (activeColor == WHITE)
		{
			position.zobristHash ^= zobristCache[WHITE][pieceType][move.from];
			position.zobristHash ^= zobristCache[WHITE][pieceType][move.to];
			position.zobristHash ^= zobristCache[BLACK][move.opFigure][move.to];
			position.allWhitePeaces &= ~(Bitboard(1) << move.from);
			position.allWhitePeaces |= (Bitboard(1) << move.to);
			position.allBlackPeaces &= ~(Bitboard(1) << move.to);
			position.pos[BLACK][move.opFigure] &= ~(Bitboard(1) << move.to);
			position.material += PieceCosts[move.opFigure];
			if (pieceType == KING) position.wShortCastle = position.wLongCastle = false;
			if (position.bShortCastle && move.opFigure == ROOK && move.to == 56) position.bShortCastle = false;
			if (position.bLongCastle && move.opFigure == ROOK && move.to == 63) position.bLongCastle = false;
		}
		else
		{
			position.zobristHash ^= zobristCache[BLACK][pieceType][move.from];
			position.zobristHash ^= zobristCache[BLACK][pieceType][move.to];
			position.zobristHash ^= zobristCache[WHITE][move.opFigure][move.to];
			position.allBlackPeaces &= ~(Bitboard(1) << move.from);
			position.allBlackPeaces |= (Bitboard(1) << move.to);
			position.allWhitePeaces &= ~(Bitboard(1) << move.to);
			position.pos[WHITE][move.opFigure] &= ~(Bitboard(1) << move.to);
			position.material -= PieceCosts[move.opFigure];
			if (pieceType == KING) position.bShortCastle = position.bLongCastle = false;
			if (position.wShortCastle && move.opFigure == ROOK && move.to == 0) position.wShortCastle = false;
			if (position.wLongCastle && move.opFigure == ROOK && move.to == 7) position.wLongCastle = false;
			
		}
		break;
	}
	case FIRST_PAWN_MOVE:
	{
		position.pos[activeColor][PAWN] &= ~(Bitboard(1) << move.from);
		position.pos[activeColor][PAWN] |= (Bitboard(1) << move.to);
		position.pieceHash[move.from].color = NO_COLOR;
		position.pieceHash[move.from].type = NO_TYPE;
		position.pieceHash[move.to].color = activeColor;
		position.pieceHash[move.to].type = pieceType;
		position.zobristHash ^= zobristCache[activeColor][PAWN][move.from];
		position.zobristHash ^= zobristCache[activeColor][PAWN][move.to];
		if (activeColor == WHITE)
		{
			position.enPassantField = (Bitboard(1) << (move.to - 8));
			position.allWhitePeaces &= ~(Bitboard(1) << move.from);
			position.allWhitePeaces |= (Bitboard(1) << move.to);
		}
		else
		{
			position.enPassantField = (Bitboard(1) << (move.to + 8));
			position.allBlackPeaces &= ~(Bitboard(1) << move.from);
			position.allBlackPeaces |= (Bitboard(1) << move.to);
		}
		break;
	}
	case EN_PASSANT:
	{
		position.pos[activeColor][PAWN] &= ~(Bitboard(1) << move.from);
		position.pos[activeColor][PAWN] |= (Bitboard(1) << move.to);
		position.pieceHash[move.from].color = NO_COLOR;
		position.pieceHash[move.from].type = NO_TYPE;
		position.pieceHash[move.to].color = activeColor;
		position.pieceHash[move.to].type = pieceType;
		if (activeColor == WHITE)
		{
			position.zobristHash ^= zobristCache[WHITE][PAWN][move.from];
			position.zobristHash ^= zobristCache[WHITE][PAWN][move.to];
			position.zobristHash ^= zobristCache[BLACK][PAWN][move.to - 8];
			position.pos[BLACK][PAWN] &= ~(Bitboard(1) << (move.to - 8));
			position.allBlackPeaces &= ~(Bitboard(1) << (move.to - 8));
			position.allWhitePeaces &= ~(Bitboard(1) << move.from);
			position.allWhitePeaces |= (Bitboard(1) << move.to);
			++position.material;
			position.pieceHash[move.to - 8].color = NO_COLOR;
			position.pieceHash[move.to - 8].type = NO_TYPE;
		}
		else
		{
			position.zobristHash ^= zobristCache[BLACK][PAWN][move.from];
			position.zobristHash ^= zobristCache[BLACK][PAWN][move.to];
			position.zobristHash ^= zobristCache[WHITE][PAWN][move.to + 8];
			position.pos[WHITE][PAWN] &= ~(Bitboard(1) << (move.to + 8));
			position.allWhitePeaces &= ~(Bitboard(1) << (move.to + 8));
			position.allBlackPeaces &= ~(Bitboard(1) << move.from);
			position.allBlackPeaces |= (Bitboard(1) << move.to);
			--position.material;
			position.pieceHash[move.to + 8].color = NO_COLOR;
			position.pieceHash[move.to + 8].type = NO_TYPE;
		}
		break;
	}
	case LONG_CASTLE:
	{
		if (activeColor == WHITE)
		{
			position.wShortCastle = position.wLongCastle = false;
			position.pos[WHITE][KING] = 0X20;
			position.pos[WHITE][ROOK] &= ~(0X80);
			position.pos[WHITE][ROOK] |= 0X10;
			position.allWhitePeaces &= ~(0X88);
			position.allWhitePeaces |= 0X30;
			position.pieceHash[3].color = NO_COLOR;
			position.pieceHash[3].type = NO_TYPE;
			position.pieceHash[7].color = NO_COLOR;
			position.pieceHash[7].type = NO_TYPE;
			position.pieceHash[5].color = WHITE;
			position.pieceHash[5].type = KING;
			position.pieceHash[4].color = WHITE;
			position.pieceHash[4].type = ROOK;
			position.zobristHash ^= zobristCache[WHITE][KING][3];
			position.zobristHash ^= zobristCache[WHITE][KING][5];
			position.zobristHash ^= zobristCache[WHITE][ROOK][7];
			position.zobristHash ^= zobristCache[WHITE][ROOK][4];
		}
		else
		{
			position.bShortCastle = position.bLongCastle = false;
			position.pos[BLACK][KING] = Bitboard(1) << 61;
			position.pos[BLACK][ROOK] &= ~(Bitboard(1) << 63);
			position.pos[BLACK][ROOK] |= (Bitboard(1) << 60);
			position.allBlackPeaces &= ~0X8800000000000000;
			position.allBlackPeaces |= 0X3000000000000000;
			position.pieceHash[59].color = NO_COLOR;
			position.pieceHash[59].type = NO_TYPE;
			position.pieceHash[63].color = NO_COLOR;
			position.pieceHash[63].type = NO_TYPE;
			position.pieceHash[61].color = BLACK;
			position.pieceHash[61].type = KING;
			position.pieceHash[60].color = BLACK;
			position.pieceHash[60].type = ROOK;
			position.zobristHash ^= zobristCache[BLACK][KING][59];
			position.zobristHash ^= zobristCache[BLACK][KING][61];
			position.zobristHash ^= zobristCache[BLACK][ROOK][63];
			position.zobristHash ^= zobristCache[BLACK][ROOK][60];
		}
		break;
	}
	case SHORT_CASTLE:
	{
		if (activeColor == WHITE)
		{
			position.wShortCastle = position.wLongCastle = false;
			position.pos[WHITE][KING] = 0X2;
			position.pos[WHITE][ROOK] &= ~0X1;
			position.pos[WHITE][ROOK] |= 0X4;
			position.allWhitePeaces &= ~(0X9);
			position.allWhitePeaces |= 0X6;
			position.pieceHash[3].color = NO_COLOR;
			position.pieceHash[3].type = NO_TYPE;
			position.pieceHash[0].color = NO_COLOR;
			position.pieceHash[0].type = NO_TYPE;
			position.pieceHash[1].color = WHITE;
			position.pieceHash[1].type = KING;
			position.pieceHash[2].color = WHITE;
			position.pieceHash[2].type = ROOK;
			position.zobristHash ^= zobristCache[WHITE][KING][3];
			position.zobristHash ^= zobristCache[WHITE][KING][1];
			position.zobristHash ^= zobristCache[WHITE][ROOK][0];
			position.zobristHash ^= zobristCache[WHITE][ROOK][2];
		}
		else
		{
			position.bShortCastle = position.bLongCastle = false;
			position.pos[BLACK][KING] = 0X0200000000000000;
			position.pos[BLACK][ROOK] &= ~(0X0100000000000000);
			position.pos[BLACK][ROOK] |= 0X0400000000000000;
			position.allBlackPeaces &= ~0X0900000000000000;
			position.allBlackPeaces |= 0X0600000000000000;
			position.pieceHash[56].color = NO_COLOR;
			position.pieceHash[56].type = NO_TYPE;
			position.pieceHash[59].color = NO_COLOR;
			position.pieceHash[59].type = NO_TYPE;
			position.pieceHash[57].color = BLACK;
			position.pieceHash[57].type = KING;
			position.pieceHash[58].color = BLACK;
			position.pieceHash[58].type = ROOK;
			position.zobristHash ^= zobristCache[BLACK][KING][59];
			position.zobristHash ^= zobristCache[BLACK][KING][57];
			position.zobristHash ^= zobristCache[BLACK][ROOK][56];
			position.zobristHash ^= zobristCache[BLACK][ROOK][58];
		}
		break;
	}
	case TRANSFORM:
	{
		position.pieceHash[move.from].color = NO_COLOR;
		position.pieceHash[move.from].type = NO_TYPE;
		position.pieceHash[move.to].color = activeColor;
		position.pieceHash[move.to].type = move.transformPiece; 
		position.zobristHash ^= zobristCache[activeColor][PAWN][move.from];
		position.zobristHash ^= zobristCache[activeColor][move.transformPiece][move.to];
		if (activeColor == WHITE)
		{
			position.pos[WHITE][PAWN] &= ~(Bitboard(1) << move.from);
			position.pos[WHITE][move.transformPiece] |= (Bitboard(1) << move.to);
			position.allWhitePeaces &= ~(Bitboard(1) << move.from);
			position.allWhitePeaces |= (Bitboard(1) << move.to);
			position.material += PieceCosts[move.transformPiece] - 1;
		}
		else
		{
			position.pos[BLACK][PAWN] &= ~(Bitboard(1) << move.from);
			position.pos[BLACK][move.transformPiece] |= (Bitboard(1) << move.to);
			position.allBlackPeaces &= ~(Bitboard(1) << move.from);
			position.allBlackPeaces |= (Bitboard(1) << move.to);
			position.material -= PieceCosts[move.transformPiece] - 1;
		}
		break;
	}
	case CAPTURE_TRANSFORM:
	{
		position.pieceHash[move.from].color = NO_COLOR;
		position.pieceHash[move.from].type = NO_TYPE;
		position.pieceHash[move.to].color = activeColor;
		position.pieceHash[move.to].type = move.transformPiece;
		position.zobristHash ^= zobristCache[activeColor][PAWN][move.from];
		position.zobristHash ^= zobristCache[activeColor][move.transformPiece][move.to];
		if (activeColor == WHITE)
		{
			position.pos[BLACK][move.opFigure] &= ~(Bitboard(1) << move.to);
			position.allBlackPeaces &= ~(Bitboard(1) << move.to);
			position.pos[WHITE][PAWN] &= ~(Bitboard(1) << move.from);
			position.pos[WHITE][move.transformPiece] |= (Bitboard(1) << move.to);
			position.allWhitePeaces &= ~(Bitboard(1) << move.from);
			position.allWhitePeaces |= (Bitboard(1) << move.to);
			position.material += PieceCosts[move.transformPiece] - 1 + PieceCosts[move.opFigure];
			position.zobristHash ^= zobristCache[BLACK][move.opFigure][move.to];
			if (position.bShortCastle && move.opFigure == ROOK && move.to == 56) position.bShortCastle = false;
			if (position.bLongCastle && move.opFigure == ROOK && move.to == 63) position.bLongCastle = false;
		}
		else
		{
			position.pos[WHITE][move.opFigure] &= ~(Bitboard(1) << move.to);
			position.allWhitePeaces &= ~(Bitboard(1) << move.to);
			position.pos[BLACK][PAWN] &= ~(Bitboard(1) << move.from);
			position.pos[BLACK][move.transformPiece] |= (Bitboard(1) << move.to);
			position.allBlackPeaces &= ~(Bitboard(1) << move.from);
			position.allBlackPeaces |= (Bitboard(1) << move.to);
			position.material -= PieceCosts[move.transformPiece] - 1 + PieceCosts[move.opFigure];
			position.zobristHash ^= zobristCache[WHITE][move.opFigure][move.to];
			if (position.wShortCastle && move.opFigure == ROOK && move.to == 0) position.wShortCastle = false;
			if (position.wLongCastle && move.opFigure == ROOK && move.to == 7) position.wLongCastle = false;
		}
		break;
	}
	}
	position.allPeaces = position.allBlackPeaces | position.allWhitePeaces;
	position.activeColor = ((activeColor == WHITE) ? BLACK : WHITE);
}

void MoveGen::unMakeMove(Position& position, TMove move)
{
	
	int prevPieceType = position.pieceHash[move.to].type;
	int prevActiveColor = ((position.activeColor == WHITE) ? BLACK : WHITE);

	--position.moveCounter;

	position.fiftyMovesRule = Game::positionInfo[position.moveCounter].fiftyMovesRule;
	position.enPassantField = Game::positionInfo[position.moveCounter].enPassantField;

	position.wShortCastle = Game::positionInfo[position.moveCounter].wShortCastle;
	position.wLongCastle = Game::positionInfo[position.moveCounter].wLongCastle;
	position.bShortCastle = Game::positionInfo[position.moveCounter].bShortCastle;
	position.bLongCastle = Game::positionInfo[position.moveCounter].bLongCastle;

	position.prevMove = Game::positionInfo[position.moveCounter].prevMove;

	
	switch (move.moveType)
	{
	case DEFAULT_MOVE:
	{
		position.pos[prevActiveColor][prevPieceType] &= ~(Bitboard(1) << move.to);
		position.pos[prevActiveColor][prevPieceType] |= (Bitboard(1) << move.from);
		position.pieceHash[move.to].type = PieceTypes::NO_TYPE;
		position.pieceHash[move.to].color = PieceColors::NO_COLOR;
		position.pieceHash[move.from].type = prevPieceType;
		position.pieceHash[move.from].color = prevActiveColor;
		position.zobristHash ^= zobristCache[prevActiveColor][prevPieceType][move.to];
		position.zobristHash ^= zobristCache[prevActiveColor][prevPieceType][move.from];
		if (prevActiveColor == WHITE)
		{
			position.allWhitePeaces &= ~(Bitboard(1) << move.to);
			position.allWhitePeaces |= (Bitboard(1) << move.from);
		}
		else
		{
			position.allBlackPeaces &= ~(Bitboard(1) << move.to);
			position.allBlackPeaces |= (Bitboard(1) << move.from);
		}
		break;
	}
	case CAPTURE:
	{
		position.pos[prevActiveColor][prevPieceType] &= ~(Bitboard(1) << move.to);
		position.pos[prevActiveColor][prevPieceType] |= (Bitboard(1) << move.from);
		position.pieceHash[move.to].color = position.activeColor;
		position.pieceHash[move.to].type = move.opFigure ;
		position.pieceHash[move.from].color = prevActiveColor;
		position.pieceHash[move.from].type = prevPieceType;
		if (prevActiveColor == WHITE)
		{
			position.allWhitePeaces &= ~(Bitboard(1) << move.to);
			position.allWhitePeaces |= (Bitboard(1) << move.from);
			position.allBlackPeaces |= (Bitboard(1) << move.to);
			position.pos[BLACK][move.opFigure] |= (Bitboard(1) << move.to);
			position.material -= PieceCosts[move.opFigure];
			position.zobristHash ^= zobristCache[WHITE][prevPieceType][move.from];
			position.zobristHash ^= zobristCache[WHITE][prevPieceType][move.to];
			position.zobristHash ^= zobristCache[BLACK][move.opFigure][move.to];
		}
		else
		{
			position.allBlackPeaces &= ~(Bitboard(1) << move.to);
			position.allBlackPeaces |= (Bitboard(1) << move.from);
			position.allWhitePeaces |= (Bitboard(1) << move.to);
			position.pos[WHITE][move.opFigure] |= (Bitboard(1) << move.to);
			position.material += PieceCosts[move.opFigure];
			position.zobristHash ^= zobristCache[BLACK][prevPieceType][move.from];
			position.zobristHash ^= zobristCache[BLACK][prevPieceType][move.to];
			position.zobristHash ^= zobristCache[WHITE][move.opFigure][move.to];
		}
		break;
	}
	case FIRST_PAWN_MOVE:
	{
		position.pos[prevActiveColor][PAWN] &= ~(Bitboard(1) << move.to);
		position.pos[prevActiveColor][PAWN] |= (Bitboard(1) << move.from);
		position.pieceHash[move.to].color = NO_COLOR;
		position.pieceHash[move.to].type = NO_TYPE;
		position.pieceHash[move.from].color = prevActiveColor;
		position.pieceHash[move.from].type = prevPieceType;
		position.zobristHash ^= zobristCache[prevActiveColor][PAWN][move.from];
		position.zobristHash ^= zobristCache[prevActiveColor][PAWN][move.to];
		if (prevActiveColor == WHITE)
		{
			position.allWhitePeaces &= ~(Bitboard(1) << move.to);
			position.allWhitePeaces |= (Bitboard(1) << move.from);
		}
		else
		{
			position.allBlackPeaces &= ~(Bitboard(1) << move.to);
			position.allBlackPeaces |= (Bitboard(1) << move.from);
		}
		break;
	}
	case EN_PASSANT:
	{
		position.pos[prevActiveColor][PAWN] &= ~(Bitboard(1) << move.to);
		position.pos[prevActiveColor][PAWN] |= (Bitboard(1) << move.from);
		position.pieceHash[move.from].color = prevActiveColor;
		position.pieceHash[move.from].type = PAWN;
		position.pieceHash[move.to].color = NO_COLOR;
		position.pieceHash[move.to].type = NO_TYPE;
		position.zobristHash ^= zobristCache[prevActiveColor][PAWN][move.from];
		position.zobristHash ^= zobristCache[prevActiveColor][PAWN][move.to];
		if (prevActiveColor == WHITE)
		{
			position.pos[BLACK][PAWN] |= (Bitboard(1) << (move.to - 8));
			position.allBlackPeaces |= (Bitboard(1) << (move.to - 8));
			position.allWhitePeaces |= (Bitboard(1) << move.from);
			position.allWhitePeaces &= ~(Bitboard(1) << move.to);
			--position.material;
			position.pieceHash[move.to - 8].color = BLACK;
			position.pieceHash[move.to - 8].type = PAWN;
			position.zobristHash ^= zobristCache[BLACK][PAWN][move.to - 8];
		}
		else
		{
			position.pos[WHITE][PAWN] |= (Bitboard(1) << (move.to + 8));
			position.allWhitePeaces |= (Bitboard(1) << (move.to + 8));
			position.allBlackPeaces |= (Bitboard(1) << move.from);
			position.allBlackPeaces &= ~(Bitboard(1) << move.to);
			++position.material;
			position.pieceHash[move.to + 8].color = WHITE;
			position.pieceHash[move.to + 8].type = PAWN;
			position.zobristHash ^= zobristCache[WHITE][PAWN][move.to + 8];
		}
		break;
	}
	case SHORT_CASTLE:
	{
		if (prevActiveColor == WHITE)
		{
			position.pos[WHITE][KING] = 0X8;
			position.pos[WHITE][ROOK] |= 0X1;
			position.pos[WHITE][ROOK] &= ~0X4;
			position.allWhitePeaces |= 0X9;
			position.allWhitePeaces &= ~0X6;
			position.pieceHash[3].color = WHITE;
			position.pieceHash[3].type = KING;
			position.pieceHash[0].color = WHITE;
			position.pieceHash[0].type = ROOK;
			position.pieceHash[1].color = NO_COLOR;
			position.pieceHash[1].type = NO_TYPE;
			position.pieceHash[2].color = NO_COLOR;
			position.pieceHash[2].type = NO_TYPE;
			position.zobristHash ^= zobristCache[WHITE][KING][3];
			position.zobristHash ^= zobristCache[WHITE][KING][1];
			position.zobristHash ^= zobristCache[WHITE][ROOK][0];
			position.zobristHash ^= zobristCache[WHITE][ROOK][2];
		}
		else
		{
			position.pos[BLACK][KING] = Bitboard(1) << 59;
			position.pos[BLACK][ROOK] |= (0X0100000000000000);
			position.pos[BLACK][ROOK] &= ~0X0400000000000000;
			position.allBlackPeaces |= 0X0900000000000000;
			position.allBlackPeaces &= ~0X0600000000000000;
			position.pieceHash[56].color = BLACK;
			position.pieceHash[56].type = ROOK;
			position.pieceHash[59].color = BLACK;
			position.pieceHash[59].type = KING;
			position.pieceHash[57].color = NO_COLOR;
			position.pieceHash[57].type = NO_TYPE;
			position.pieceHash[58].color = NO_COLOR;
			position.pieceHash[58].type = NO_TYPE;
			position.zobristHash ^= zobristCache[BLACK][KING][59];
			position.zobristHash ^= zobristCache[BLACK][KING][57];
			position.zobristHash ^= zobristCache[BLACK][ROOK][56];
			position.zobristHash ^= zobristCache[BLACK][ROOK][58];
		}
		break;
	}
	case LONG_CASTLE:
	{
		if (prevActiveColor == WHITE)
		{
			position.pos[WHITE][KING] = 0X8;
			position.pos[WHITE][ROOK] |= (0X80);
			position.pos[WHITE][ROOK] &= ~0X10;
			position.allWhitePeaces |= (0X88);
			position.allWhitePeaces &= ~0X30;
			position.pieceHash[3].color = WHITE;
			position.pieceHash[3].type = KING;
			position.pieceHash[7].color = WHITE;
			position.pieceHash[7].type = ROOK;
			position.pieceHash[5].color = NO_COLOR;
			position.pieceHash[5].type = NO_TYPE;
			position.pieceHash[4].color = NO_COLOR;
			position.pieceHash[4].type = NO_TYPE;
			position.zobristHash ^= zobristCache[WHITE][KING][3];
			position.zobristHash ^= zobristCache[WHITE][KING][5];
			position.zobristHash ^= zobristCache[WHITE][ROOK][7];
			position.zobristHash ^= zobristCache[WHITE][ROOK][4];
		}
		else
		{
			position.pos[BLACK][KING] = Bitboard(1) << 59;
			position.pos[BLACK][ROOK] |= (Bitboard(1) << 63);
			position.pos[BLACK][ROOK] &= ~(Bitboard(1) << 60);
			position.allBlackPeaces |= 0X8800000000000000;
			position.allBlackPeaces &= ~0X3000000000000000;
			position.pieceHash[59].color = BLACK;
			position.pieceHash[59].type = KING;
			position.pieceHash[63].color = BLACK;
			position.pieceHash[63].type = ROOK;
			position.pieceHash[61].color = NO_COLOR;
			position.pieceHash[61].type = NO_TYPE;
			position.pieceHash[60].color = NO_COLOR;
			position.pieceHash[60].type = NO_TYPE;
			position.zobristHash ^= zobristCache[BLACK][KING][59];
			position.zobristHash ^= zobristCache[BLACK][KING][61];
			position.zobristHash ^= zobristCache[BLACK][ROOK][63];
			position.zobristHash ^= zobristCache[BLACK][ROOK][60];
		}
		break;
	}
	case TRANSFORM:
	{
		position.pieceHash[move.to].color = NO_COLOR;
		position.pieceHash[move.to].type = NO_TYPE;
		position.pieceHash[move.from].color = prevActiveColor;
		position.pieceHash[move.from].type = PAWN;
		position.zobristHash ^= zobristCache[prevActiveColor][PAWN][move.from];
		position.zobristHash ^= zobristCache[prevActiveColor][move.transformPiece][move.to];
		if (prevActiveColor == WHITE)
		{
			position.pos[WHITE][PAWN] |= (Bitboard(1) << move.from);
			position.pos[WHITE][move.transformPiece] &= ~(Bitboard(1) << move.to);
			position.allWhitePeaces &= ~(Bitboard(1) << move.to);
			position.allWhitePeaces |= (Bitboard(1) << move.from);
			position.material -= PieceCosts[move.transformPiece] - 1;
		}
		else
		{
			position.pos[BLACK][PAWN] |= (Bitboard(1) << move.from);
			position.pos[BLACK][move.transformPiece] &= ~(Bitboard(1) << move.to);
			position.allBlackPeaces &= ~(Bitboard(1) << move.to);
			position.allBlackPeaces |= (Bitboard(1) << move.from);
			position.material += PieceCosts[move.transformPiece] - 1;
		}
		break;
	}
	case CAPTURE_TRANSFORM:
	{
		position.pieceHash[move.from].color = prevActiveColor;
		position.pieceHash[move.from].type = PAWN;
		position.pieceHash[move.to].color = position.activeColor;
		position.pieceHash[move.to].type = move.opFigure;
		position.zobristHash ^= zobristCache[prevActiveColor][PAWN][move.from];
		position.zobristHash ^= zobristCache[prevActiveColor][move.transformPiece][move.to];
		if (prevActiveColor == WHITE)
		{
			position.pos[BLACK][move.opFigure] |= (Bitboard(1) << move.to);
			position.allBlackPeaces |= (Bitboard(1) << move.to);
			position.pos[WHITE][PAWN] |= (Bitboard(1) << move.from);
			position.pos[WHITE][move.transformPiece] &= ~(Bitboard(1) << move.to);
			position.allWhitePeaces |= (Bitboard(1) << move.from);
			position.allWhitePeaces &= ~(Bitboard(1) << move.to);
			position.material -= PieceCosts[move.transformPiece] - 1 + PieceCosts[move.opFigure];
			position.zobristHash ^= zobristCache[BLACK][move.opFigure][move.to];
		}
		else
		{
			position.pos[WHITE][move.opFigure] |= (Bitboard(1) << move.to);
			position.allWhitePeaces |= (Bitboard(1) << move.to);
			position.pos[BLACK][PAWN] |= (Bitboard(1) << move.from);
			position.pos[BLACK][move.transformPiece] &= ~(Bitboard(1) << move.to);
			position.allBlackPeaces |= (Bitboard(1) << move.from);
			position.allBlackPeaces &= ~(Bitboard(1) << move.to);
			position.material += PieceCosts[move.transformPiece] - 1 + PieceCosts[move.opFigure];
			position.zobristHash ^= zobristCache[WHITE][move.opFigure][move.to];
		}
		break;
	}
	}
	position.allPeaces = position.allBlackPeaces | position.allWhitePeaces;
	position.activeColor = prevActiveColor; 
}

TMove* MoveGen::generateAndSortAllCaptures(Position& position,int& size)
{
	
	TMove* Captures = new TMove[50];
	int captureIndex = 0;
	int prevMovePieceType = position.pieceHash[position.prevMove.to].type;
	int activeColor = position.activeColor;
	Bitboard blockers = position.allPeaces;

	if (activeColor == WHITE)
	{
		int kingSq = bitScanForward(position.pos[WHITE][KING]);
		if (kingSq != -1)
		{
			Bitboard kingAttacks = getCachedKingMoves(kingSq) & position.allBlackPeaces;
			while (kingAttacks)
			{
				int attackSq = bitScanForward(kingAttacks);
				Captures[captureIndex].from = kingSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure];
				kingAttacks &= (kingAttacks - 1);
			}
		}
		Bitboard pawns = position.pos[WHITE][PAWN];
		while (pawns)
		{
			int pawnSq = bitScanForward(pawns);
			Bitboard pawnAttacks = getCachedPawnCaptures(pawnSq, WHITE) & position.allBlackPeaces;
			while (pawnAttacks)
			{
				int attackSq = bitScanForward(pawnAttacks);
				if (attackSq > 55)
				{
					for (int i = PieceTypes::QUEEN; i <= PieceTypes::KNIGHT; i++)
					{
						Captures[captureIndex].from = pawnSq;
						Captures[captureIndex].to = attackSq;
						Captures[captureIndex].moveType = CAPTURE_TRANSFORM;
						Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
						Captures[captureIndex].transformPiece = i;
						Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 1 + PieceCosts[i];
						++captureIndex;
					}
				}
				else
				{
					Captures[captureIndex].from = pawnSq;
					Captures[captureIndex].to = attackSq;
					Captures[captureIndex].moveType = CAPTURE;
					Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
					Captures[captureIndex].transformPiece = NO_TYPE;
					Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 1;
					++captureIndex;
				}
				pawnAttacks &= (pawnAttacks - 1);
			}
			pawns &= (pawns - 1);
		}
		if (position.enPassantField)
		{
			Bitboard enPassantPawns = position.pos[WHITE][PAWN] & RANK_5;
			while (enPassantPawns)
			{
				int enPassantPawnSq = bitScanForward(enPassantPawns);
				Bitboard enPassantCapture = getCachedPawnCaptures(enPassantPawnSq, WHITE) & position.enPassantField;
				if (enPassantCapture)
				{
					Captures[captureIndex].from = enPassantPawnSq;
					Captures[captureIndex].to = bitScanForward(enPassantCapture);
					Captures[captureIndex].moveType = EN_PASSANT;
					Captures[captureIndex].opFigure = PAWN;
					Captures[captureIndex].transformPiece = NO_TYPE;
					Captures[captureIndex].sortField = 0;
					++captureIndex;
				}
				enPassantPawns &= (enPassantPawns - 1);
			}
		}
		Bitboard knights = position.pos[WHITE][KNIGHT];
		while (knights)
		{
			int knightSq = bitScanForward(knights);
			Bitboard knightAttacks = getCachedKnightMoves(knightSq) & position.allBlackPeaces;
			while (knightAttacks)
			{
				int attackSq = bitScanForward(knightAttacks);
				Captures[captureIndex].from = knightSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 3;
				++captureIndex;
				knightAttacks &= (knightAttacks - 1);
			}
			knights &= (knights - 1);
		}
		Bitboard bishops = position.pos[WHITE][BISHOP];
		while (bishops)
		{
			int bishopSq = bitScanForward(bishops);
			Bitboard bishopAttacks = getCachedBishopMoves(bishopSq, blockers) & position.allBlackPeaces;
			while (bishopAttacks)
			{
				int attackSq = bitScanForward(bishopAttacks);
				Captures[captureIndex].from = bishopSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 3;
				++captureIndex;
				bishopAttacks &= (bishopAttacks - 1);
			}
			bishops &= (bishops - 1);
		}
		Bitboard rooks = position.pos[WHITE][ROOK];
		while (rooks)
		{
			int rookSq = bitScanForward(rooks);
			Bitboard rookAttacks = getCachedRookMoves(rookSq, blockers) & position.allBlackPeaces;
			while (rookAttacks)
			{
				int attackSq = bitScanForward(rookAttacks);
				Captures[captureIndex].from = rookSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 5;
				++captureIndex;
				rookAttacks &= (rookAttacks - 1);
			}
			rooks &= (rooks - 1);
		}
		Bitboard queens = position.pos[WHITE][QUEEN];
		while (queens)
		{
			int queenSq = bitScanForward(queens);
			Bitboard queenAttacks = getCachedQueenMoves(queenSq, blockers) & position.allBlackPeaces;
			while (queenAttacks)
			{
				int attackSq = bitScanForward(queenAttacks);
				Captures[captureIndex].from = queenSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 9;
				++captureIndex;
				queenAttacks &= (queenAttacks - 1);
			}
			queens &= (queens - 1);
		}
	}
	else
	{
		int kingSq = bitScanForward(position.pos[BLACK][KING]);
		if (kingSq != -1)
		{
			Bitboard kingAttacks = getCachedKingMoves(kingSq) & position.allWhitePeaces;
			while (kingAttacks)
			{
				int attackSq = bitScanForward(kingAttacks);
				Captures[captureIndex].from = kingSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure];
				kingAttacks &= (kingAttacks - 1);
			}
		}
		Bitboard pawns = position.pos[BLACK][PAWN];
		while (pawns)
		{
			int pawnSq = bitScanForward(pawns);
			Bitboard pawnAttacks = getCachedPawnCaptures(pawnSq, BLACK) & position.allWhitePeaces;
			while (pawnAttacks)
			{
				int attackSq = bitScanForward(pawnAttacks);
				if (attackSq < 8)
				{
					for (int i = PieceTypes::QUEEN; i <= PieceTypes::KNIGHT; i++)
					{
						Captures[captureIndex].from = pawnSq;
						Captures[captureIndex].to = attackSq;
						Captures[captureIndex].moveType = CAPTURE_TRANSFORM;
						Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
						Captures[captureIndex].transformPiece = i;
						Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 1 + PieceCosts[i];
						++captureIndex;
					}
				}
				else
				{
					Captures[captureIndex].from = pawnSq;
					Captures[captureIndex].to = attackSq;
					Captures[captureIndex].moveType = CAPTURE;
					Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
					Captures[captureIndex].transformPiece = NO_TYPE;
					Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 1;
					++captureIndex;
				}
				pawnAttacks &= (pawnAttacks - 1);
			}
			pawns &= (pawns - 1);
		}
		if (position.enPassantField)
		{
			Bitboard enPassantPawns = position.pos[BLACK][PAWN] & RANK_4;
			while (enPassantPawns)
			{
				int enPassantPawnSq = bitScanForward(enPassantPawns);
				Bitboard enPassantCapture = getCachedPawnCaptures(enPassantPawnSq, BLACK) & position.enPassantField;
				if (enPassantCapture)
				{
					Captures[captureIndex].from = enPassantPawnSq;
					Captures[captureIndex].to = bitScanForward(enPassantCapture);
					Captures[captureIndex].moveType = EN_PASSANT;
					Captures[captureIndex].opFigure = PAWN;
					Captures[captureIndex].transformPiece = NO_TYPE;
					Captures[captureIndex].sortField = 0;
					++captureIndex;
				}
				enPassantPawns &= (enPassantPawns - 1);
			}
		}
		Bitboard knights = position.pos[BLACK][KNIGHT];
		while (knights)
		{
			int knightSq = bitScanForward(knights);
			Bitboard knightAttacks = getCachedKnightMoves(knightSq) & position.allWhitePeaces;
			while (knightAttacks)
			{
				int attackSq = bitScanForward(knightAttacks);
				Captures[captureIndex].from = knightSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 3;
				++captureIndex;
				knightAttacks &= (knightAttacks - 1);
			}
			knights &= (knights - 1);
		}
		Bitboard bishops = position.pos[BLACK][BISHOP];
		while (bishops)
		{
			int bishopSq = bitScanForward(bishops);
			Bitboard bishopAttacks = getCachedBishopMoves(bishopSq, blockers) & position.allWhitePeaces;
			while (bishopAttacks)
			{
				int attackSq = bitScanForward(bishopAttacks);
				Captures[captureIndex].from = bishopSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 3;
				++captureIndex;
				bishopAttacks &= (bishopAttacks - 1);
			}
			bishops &= (bishops - 1);
		}
		Bitboard rooks = position.pos[BLACK][ROOK];
		while (rooks)
		{
			int rookSq = bitScanForward(rooks);
			Bitboard rookAttacks = getCachedRookMoves(rookSq, blockers) & position.allWhitePeaces;
			while (rookAttacks)
			{
				int attackSq = bitScanForward(rookAttacks);
				Captures[captureIndex].from = rookSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 5;
				++captureIndex;
				rookAttacks &= (rookAttacks - 1);
			}
			rooks &= (rooks - 1);
		}
		Bitboard queens = position.pos[BLACK][QUEEN];
		while (queens)
		{
			int queenSq = bitScanForward(queens);
			Bitboard queenAttacks = getCachedQueenMoves(queenSq, blockers) & position.allWhitePeaces;
			while (queenAttacks)
			{
				int attackSq = bitScanForward(queenAttacks);
				Captures[captureIndex].from = queenSq;
				Captures[captureIndex].to = attackSq;
				Captures[captureIndex].moveType = CAPTURE;
				Captures[captureIndex].opFigure = position.pieceHash[attackSq].type;
				Captures[captureIndex].transformPiece = NO_TYPE;
				Captures[captureIndex].sortField = PieceCosts[Captures[captureIndex].opFigure] - 9;
				++captureIndex;
				queenAttacks &= (queenAttacks - 1);
			}
			queens &= (queens - 1);
		}
	}

	
	// sort by sortField MVV/LVA
	for (int i = 1; i < captureIndex; i++) {
		for (int j = i; j > 0 && Captures[j - 1].sortField < Captures[j].sortField; j--) {
			TMove tmp = Captures[j - 1];
			Captures[j - 1] = Captures[j];
			Captures[j] = tmp;
		}
	}

	size = captureIndex;
	//Captures[captureIndex].transformPiece = PAWN;

	return Captures;
}

TMove* MoveGen::generateAndSortAllQuietMoves(Position& position,int& size)
{
	TMove* moves = new TMove[150];
	int movesIndex = 0;

	int activeColor = position.activeColor;
	Bitboard kingSq = position.pos[activeColor][KING];
	Bitboard blockers = position.allPeaces;

	if (isCastlingPossible(position, SHORT_CASTLE) && kingSq != Bitboard(0))
	{
		moves[movesIndex].from = ((activeColor == WHITE) ? 3 : 59);
		moves[movesIndex].to = ((activeColor == WHITE) ? 1 : 57);
		moves[movesIndex].moveType = SHORT_CASTLE;
		moves[movesIndex].opFigure = NO_TYPE;
		moves[movesIndex].transformPiece = NO_TYPE;
		++movesIndex;
	}
	if (isCastlingPossible(position, LONG_CASTLE) && kingSq != Bitboard(0))
	{
		moves[movesIndex].from = ((activeColor == WHITE) ? 3 : 59);
		moves[movesIndex].to = ((activeColor == WHITE) ? 5 : 61);
		moves[movesIndex].moveType = LONG_CASTLE;
		moves[movesIndex].opFigure = NO_TYPE;
		moves[movesIndex].transformPiece = NO_TYPE;
		++movesIndex;
	}

	if (activeColor == WHITE)
	{
		Bitboard queens = position.pos[WHITE][QUEEN];
		while (queens)
		{
			int queenSq = bitScanForward(queens);
			Bitboard queenMoves = getCachedQueenMoves(queenSq, blockers) & ~position.allPeaces;
			while (queenMoves)
			{
				int moveSq = bitScanForward(queenMoves);
				moves[movesIndex].from = queenSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				moves[movesIndex].sortField = 0;
				moves[movesIndex].transformPiece = NO_TYPE;
				movesIndex++;
				queenMoves &= (queenMoves - 1);
			}
			queens &= (queens - 1);
		}
		Bitboard rooks = position.pos[WHITE][ROOK];
		while (rooks)
		{
			int rookSq = bitScanForward(rooks);
			Bitboard rookMoves = getCachedRookMoves(rookSq, blockers) & ~position.allPeaces;
			while (rookMoves)
			{
				int moveSq = bitScanForward(rookMoves);
				moves[movesIndex].from = rookSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				moves[movesIndex].sortField = 0;
				moves[movesIndex].transformPiece = NO_TYPE;
				movesIndex++;
				rookMoves &= (rookMoves - 1);
			}
			rooks &= (rooks - 1);
		}
		Bitboard bishops = position.pos[WHITE][BISHOP];
		while (bishops)
		{
			int bishopSq = bitScanForward(bishops);
			Bitboard bishopMoves = getCachedBishopMoves(bishopSq, blockers) & ~position.allPeaces;
			while (bishopMoves)
			{
				int moveSq = bitScanForward(bishopMoves);
				moves[movesIndex].from = bishopSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				moves[movesIndex].sortField = 0;
				moves[movesIndex].transformPiece = NO_TYPE;
				movesIndex++;
				bishopMoves &= (bishopMoves - 1);
			}
			bishops &= (bishops - 1);
		}
		Bitboard knights = position.pos[WHITE][KNIGHT];
		while (knights)
		{
			int knightSq = bitScanForward(knights);
			Bitboard knightMoves = getCachedKnightMoves(knightSq) & ~position.allPeaces;
			while (knightMoves)
			{
				int moveSq = bitScanForward(knightMoves);
				moves[movesIndex].from = knightSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = position.pieceHash[moveSq].type;
				moves[movesIndex].sortField = 0;
				moves[movesIndex].transformPiece = NO_TYPE;
				movesIndex++;
				knightMoves &= (knightMoves - 1);
			}
			knights &= (knights - 1);
		}
		Bitboard pawns = position.pos[WHITE][PAWN];
		while (pawns)
		{
			int pawnSq = bitScanForward(pawns);
			Bitboard pawnMoves = getCachedPawnMoves(pawnSq, WHITE) & ~position.allPeaces;
			if (pawnMoves == (Bitboard(1) << (pawnSq + 16)))
			{
				pawns &= pawns - 1;
				continue;
			}
			while (pawnMoves)
			{
				int moveSq = bitScanForward(pawnMoves);
				if (moveSq > 55)
				{
					for (int i = PieceTypes::QUEEN; i <= PieceTypes::KNIGHT; i++)
					{
						moves[movesIndex].from = pawnSq;
						moves[movesIndex].to = moveSq;
						moves[movesIndex].moveType = TRANSFORM;
						moves[movesIndex].transformPiece = i;
						moves[movesIndex].opFigure = NO_TYPE;
						++movesIndex;
					}
				}
				else
				{
					moves[movesIndex].from = pawnSq;
					moves[movesIndex].to = moveSq;
					moves[movesIndex].moveType = ((moveSq - pawnSq == 16) ? FIRST_PAWN_MOVE : DEFAULT_MOVE);
					moves[movesIndex].transformPiece = NO_TYPE;
					moves[movesIndex].opFigure = NO_TYPE;
					++movesIndex;
				}				
				pawnMoves &= pawnMoves - 1;
			}
			pawns &= pawns - 1;
		}
		int kingSq = bitScanForward(position.pos[WHITE][KING]);
		if (kingSq != -1)
		{
			Bitboard kingMoves = getCachedKingMoves(kingSq) & ~position.allPeaces;
			while (kingMoves)
			{
				int moveSq = bitScanForward(kingMoves);
				moves[movesIndex].from = kingSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				moves[movesIndex].transformPiece = NO_TYPE;
				++movesIndex;
				kingMoves &= kingMoves - 1;
			}
		}
	}
	else
	{
		Bitboard queens = position.pos[BLACK][QUEEN];
		while (queens)
		{
			int queenSq = bitScanForward(queens);
			Bitboard queenMoves = getCachedQueenMoves(queenSq, blockers) & ~position.allPeaces;
			while (queenMoves)
			{
				int moveSq = bitScanForward(queenMoves);
				moves[movesIndex].from = queenSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				moves[movesIndex].sortField = 0;
				moves[movesIndex].transformPiece = NO_TYPE;
				movesIndex++;
				queenMoves &= (queenMoves - 1);
			}
			queens &= (queens - 1);
		}
		Bitboard rooks = position.pos[BLACK][ROOK];
		while (rooks)
		{
			int rookSq = bitScanForward(rooks);
			Bitboard rookMoves = getCachedRookMoves(rookSq, blockers) & ~position.allPeaces;
			while (rookMoves)
			{
				int moveSq = bitScanForward(rookMoves);
				moves[movesIndex].from = rookSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				moves[movesIndex].sortField = 0;
				moves[movesIndex].transformPiece = NO_TYPE;
				movesIndex++;
				rookMoves &= (rookMoves - 1);
			}
			rooks &= (rooks - 1);
		}
		Bitboard bishops = position.pos[BLACK][BISHOP];
		while (bishops)
		{
			int bishopSq = bitScanForward(bishops);
			Bitboard bishopMoves = getCachedBishopMoves(bishopSq, blockers) & ~position.allPeaces;
			while (bishopMoves)
			{
				int moveSq = bitScanForward(bishopMoves);
				moves[movesIndex].from = bishopSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				moves[movesIndex].sortField = 0;
				moves[movesIndex].transformPiece = NO_TYPE;
				movesIndex++;
				bishopMoves &= (bishopMoves - 1);
			}
			bishops &= (bishops - 1);
		}
		Bitboard knights = position.pos[BLACK][KNIGHT];
		while (knights)
		{
			int knightSq = bitScanForward(knights);
			Bitboard knightMoves = getCachedKnightMoves(knightSq) & ~position.allPeaces;
			while (knightMoves)
			{
				int moveSq = bitScanForward(knightMoves);
				moves[movesIndex].from = knightSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				moves[movesIndex].sortField = 0;
				moves[movesIndex].transformPiece = NO_TYPE;
				movesIndex++;
				knightMoves &= (knightMoves - 1);
			}
			knights &= (knights - 1);
		}
		Bitboard pawns = position.pos[BLACK][PAWN];
		while (pawns)
		{
			int pawnSq = bitScanForward(pawns);
			Bitboard pawnMoves = getCachedPawnMoves(pawnSq, BLACK) & ~position.allPeaces;
			if (pawnMoves == (Bitboard(1) << (pawnSq - 16)))
			{
				pawns &= pawns - 1;
				continue;
			}
			while (pawnMoves)
			{
				int moveSq = bitScanForward(pawnMoves);
				if (moveSq < 8)
				{
					for (int i = PieceTypes::QUEEN; i <= PieceTypes::KNIGHT; i++)
					{
						moves[movesIndex].from = pawnSq;
						moves[movesIndex].to = moveSq;
						moves[movesIndex].moveType = TRANSFORM;
						moves[movesIndex].transformPiece = i;
						moves[movesIndex].opFigure = NO_TYPE;
						++movesIndex;
					}
				}
				else
				{
					moves[movesIndex].from = pawnSq;
					moves[movesIndex].to = moveSq;
					moves[movesIndex].moveType = ((pawnSq - moveSq == 16) ? FIRST_PAWN_MOVE : DEFAULT_MOVE);
					moves[movesIndex].opFigure = NO_TYPE;
					++movesIndex;
				}
				pawnMoves &= pawnMoves - 1;
			}
			pawns &= pawns - 1;
		}
		int kingSq = bitScanForward(position.pos[BLACK][KING]);
		if (kingSq != -1)
		{
			Bitboard kingMoves = getCachedKingMoves(kingSq) & ~position.allPeaces;
			while (kingMoves)
			{
				int moveSq = bitScanForward(kingMoves);
				moves[movesIndex].from = kingSq;
				moves[movesIndex].to = moveSq;
				moves[movesIndex].moveType = DEFAULT_MOVE;
				moves[movesIndex].opFigure = NO_TYPE;
				++movesIndex;
				kingMoves &= kingMoves - 1;
			}
		}
	}

	size = movesIndex;
	//moves[movesIndex].transformPiece = PAWN;
	// TODO ADD SORT 

	return moves;
}

TMove* MoveGen::generateAndSortAllMoves(Position& position,int& size)
{
	int captureSize = 0, quietMovesSize = 0;
	/*TMove* captures = NULL;
	TMove* quietMoves = NULL;
	auto threadfunc1 = [captures,position, captureSize]() mutable 
		{captures = generateAndSortAllCaptures(position, captureSize); };
	auto threadfunc2 = [quietMoves, position, quietMovesSize]()
		mutable {quietMoves = generateAndSortAllQuietMoves(position, quietMovesSize); };
	
	std::thread a = std::thread(threadfunc1);
	std::thread b = std::thread(threadfunc2);
	
	a.join();
	b.join();*/
	TMove* captures = generateAndSortAllCaptures(position,captureSize);
	TMove* quietMoves = generateAndSortAllQuietMoves(position,quietMovesSize);
	size = captureSize + quietMovesSize;
	TMove* allMoves = new TMove[size];
	memcpy(allMoves, captures, sizeof(TMove) * captureSize);
	memcpy(&allMoves[captureSize], quietMoves, sizeof(TMove) * quietMovesSize);
	delete[] captures;
	delete[] quietMoves;
	return allMoves;
}

void MoveGen::viewBitboard(const Bitboard& bb, const std::string& filePath)
{
	std::string result = "";
	Bitboard mask = 0x8000000000000000;
	for (int i = 0; i < 64; i++)
	{
		(bb & mask) ? result += "1 " : result += "0 ";
		mask >>= 1;
		if (!((i + 1) & 7)) result += '\n';
	}
	result.erase(result.end() - 1);

	if (filePath == "")
	{
		std::cout << result;
	}
	else
	{
		std::fstream f(filePath, std::ios::app);
		f << result + "\n\n";
		f.close();
	}
}

std::string MoveGen::bbToString(Bitboard& bb)
{
	std::string str;
	Bitboard mask = 0x8000000000000000;
	for (int i = 0; i < 64; i++)
	{
		(bb & mask) ? str += "1 " : str += "0 ";
		mask >>= 1;
		if (!((i + 1) & 7)) str += '\n';
	}
	str.erase(str.end() - 1);
	return str;
}

int MoveGen::bitScanReverse(Bitboard bb)
{
	if (bb == Bitboard(0)) return -1;
	bb |= bb >> 1;
	bb |= bb >> 2;
	bb |= bb >> 4;
	bb |= bb >> 8;
	bb |= bb >> 16;
	bb |= bb >> 32;
	return index64[(bb * mask) >> 58];
}

int MoveGen::bitScanForward(Bitboard bb)
{
	if (bb == Bitboard(0)) return -1;
	return index64[((bb ^ (bb - 1)) * mask) >> 58];
}

void MoveGen::initRays()
{
	for (int i = 0; i < 64; i++)
	{
		rays[i][NORTH] = (FILE_H & ~RANK_1) << i;
		rays[i][SOUTH] = (FILE_A & ~RANK_8) >> (63 - i);
		rays[i][EAST] = (RANK_1 >> (8 - (i & 7))) << ((i >> 3) << 3);
		rays[i][WEST] = ((RANK_1 << ((i & 7) + 1)) & ~RANK_2) << ((i >> 3) << 3);

		rays[i][NORTH_WEST] = westN(DIAG_A8H1 & ~RANK_1, col(i)) << (row(i) << 3);
		rays[i][NORTH_EAST] = eastN(DIAG_A1H8 & ~RANK_1, 7 - col(i)) << (row(i) << 3);
		rays[i][SOUTH_EAST] = eastN(DIAG_A8H1 & ~RANK_8, 7 - col(i)) >> ((7 - row(i)) << 3);
		rays[i][SOUTH_WEST] = westN(DIAG_A1H8 & ~RANK_8, col(i)) >> ((7 - row(i)) << 3);
	}
}

Bitboard MoveGen::eastN(Bitboard board, int n)
{
	Bitboard newBoard = board;
	for (int i = 0; i < n; i++) {
		newBoard = (newBoard >> 1) & (~FILE_A);
	}
	return newBoard;
}

Bitboard MoveGen::westN(Bitboard board, int n)
{
	Bitboard newBoard = board;
	for (int i = 0; i < n; i++) {
		newBoard = (newBoard << 1) & (~FILE_H);
	}
	return newBoard;
}

inline int MoveGen::col(int sq)
{
	return sq & 7;
}

inline int MoveGen::row(int sq)
{
	return sq >> 3;
}

void MoveGen::initRookMasks()
{
	for (int i = 0; i < 64; i++)
	{
		rookMasks[i] = 0;
		rookMasks[i] |= (rays[i][NORTH] & (~RANK_8));
		rookMasks[i] |= (rays[i][SOUTH] & (~RANK_1));
		rookMasks[i] |= (rays[i][EAST] & (~FILE_H));
		rookMasks[i] |= (rays[i][WEST] & (~FILE_A));
	}
}

void MoveGen::initBishopMasks()
{
	for (int i = 0; i < 64; i++)
	{
		bishopMasks[i] = 0;
		bishopMasks[i] |= (rays[i][NORTH_EAST]);
		bishopMasks[i] |= (rays[i][NORTH_WEST]);
		bishopMasks[i] |= (rays[i][SOUTH_EAST]);
		bishopMasks[i] |= (rays[i][SOUTH_WEST]);
		bishopMasks[i] &= ~EDGES;
	}
}

void MoveGen::initQueenMasks()
{
	for (int i = 0; i < 64; i++)
		queenMasks[i] = rookMasks[i] | bishopMasks[i];
}

void MoveGen::initPawnMoveCache()
{
	for (int sq = 0; sq < 64; sq++)
	{
		wPawnMoveCache[sq] = getPawnMoves(WHITE, Bitboard(1) << sq);
		bPawnMoveCache[sq] = getPawnMoves(BLACK, Bitboard(1) << sq);
	}
}

void MoveGen::initPawnCaptureCache()
{
	for (int i = 0; i < 64; i++)
	{
		wPawnCache[i] = getPawnCaptures(WHITE, (Bitboard(1) << i));
		bPawnCache[i] = getPawnCaptures(BLACK, (Bitboard(1) << i));
	}
}

void MoveGen::initKnightCache()
{
	for (int sq = 0; sq < 64; sq++)
		knightCache[sq] = getKnightMoves(Bitboard(1) << sq);
}

void MoveGen::initKingCache()
{
	for (int sq = 0; sq < 64; sq++)
		kingCache[sq] = getKingMoves(Bitboard(1) << sq);
}

void MoveGen::initBishopCache()
{
	for (int sq = 0; sq < 64; sq++)
	{
		for (int blocker_index = 0; blocker_index < (1 << bishop_bits[sq]); blocker_index++)
		{
			Bitboard blockers = _pdep_u64(blocker_index, bishopMasks[sq]);
			Bitboard key = _pext_u64(blockers, bishopMasks[sq]);
			bishopCache[sq][key] = getBishopMoves(sq, blockers);
		}
	}
}

void MoveGen::initRookCache()
{
	for (int sq = 0; sq < 64; sq++)
	{
		for (int blocker_index = 0; blocker_index < (1 << rook_bits[sq]); blocker_index++)
		{
			Bitboard blockers = _pdep_u64(blocker_index, rookMasks[sq]);
			Bitboard key = _pext_u64(blockers, rookMasks[sq]);
			rookCache[sq][key] = getRookMoves(sq, blockers);
		}
	}
}

void MoveGen::initQueenCache()
{
	for (int sq = 0; sq < 64; sq++)
	{
		for (int blocker_index = 0; blocker_index < (1 << queen_bits[sq]); blocker_index++)
		{
			Bitboard blockers = _pdep_u64(blocker_index, queenMasks[sq]);
			Bitboard key = _pext_u64(blockers, queenMasks[sq]);
			queenCache[sq][key] = getQueenMoves(sq, blockers);
		}
	}
}

void MoveGen::initZobristCache()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 64; k++)
			{
				zobristCache[i][j][k] = random_uint64() ^
					(random_uint64() << 15) ^ (random_uint64() << 31) ^
					(random_uint64() << 47) ^ (random_uint64() << 59);
			}
		}
	}
}

Bitboard MoveGen::getPawnMoves(PieceColors color, Bitboard pawns)
{
	switch (color)
	{
	case WHITE:
		return ((pawns << 8) | ((pawns & RANK_2) << 16));
	case BLACK:
		return ((pawns >> 8) | ((pawns & RANK_7) >> 16));
	default:
		return Bitboard(0);
	}
}

Bitboard MoveGen::getPawnCaptures(PieceColors color, Bitboard pawns)
{
	switch (color)
	{
	case WHITE:
		return (((pawns << 9) & ~FILE_H) | ((pawns << 7) & ~FILE_A));
	case BLACK:
		return (((pawns >> 9) & ~FILE_A) | ((pawns >> 7) & ~FILE_H));
	default:
		return Bitboard(0);
	}
}

Bitboard MoveGen::getKnightMoves(Bitboard knights)
{
	Bitboard r1 = ((knights >> 1) & ~FILE_A);
	Bitboard l1 = ((knights << 1) & ~FILE_H);
	Bitboard r2 = ((knights >> 2) & ~(FILE_A | FILE_B));
	Bitboard l2 = ((knights << 2) & ~(FILE_H | FILE_G));
	return (((l1 | r1) << 16) | ((l1 | r1) >> 16) | ((l2 | r2) << 8) | ((l2 | r2) >> 8));
}

Bitboard MoveGen::getKingMoves(Bitboard king)
{
	return (((king << 7) | (king >> 9) | (king >> 1)) & ~FILE_A) |
		(((king << 9) | (king >> 7) | (king << 1)) & ~FILE_H) |
		((king >> 8) | (king << 8));
}

Bitboard MoveGen::getBishopMoves(int sq, Bitboard blockers)
{
	Bitboard attacks = Bitboard(0);
	attacks |= rays[sq][NORTH_EAST];
	if (rays[sq][NORTH_EAST] & blockers)
	{
		attacks &= ~(rays[bitScanForward(rays[sq][NORTH_EAST] & blockers)][NORTH_EAST]);
	}
	attacks |= rays[sq][NORTH_WEST];
	if (rays[sq][NORTH_WEST] & blockers)
	{
		attacks &= ~(rays[bitScanForward(rays[sq][NORTH_WEST] & blockers)][NORTH_WEST]);
	}
	attacks |= rays[sq][SOUTH_EAST];
	if (rays[sq][SOUTH_EAST] & blockers)
	{
		attacks &= ~(rays[bitScanReverse(rays[sq][SOUTH_EAST] & blockers)][SOUTH_EAST]);
	}
	attacks |= rays[sq][SOUTH_WEST];
	if (rays[sq][SOUTH_WEST] & blockers)
	{
		attacks &= ~(rays[bitScanReverse(rays[sq][SOUTH_WEST] & blockers)][SOUTH_WEST]);
	}
	return attacks;
}

Bitboard MoveGen::getRookMoves(int sq, Bitboard blockers)
{
	Bitboard attacks = 0;
	attacks |= rays[sq][NORTH];
	if (rays[sq][NORTH] & blockers)
	{
		attacks &= ~(rays[bitScanForward(rays[sq][NORTH] & blockers)][NORTH]);
	}
	attacks |= rays[sq][SOUTH];
	if (rays[sq][SOUTH] & blockers)
	{
		attacks &= ~(rays[bitScanReverse(rays[sq][SOUTH] & blockers)][SOUTH]);
	}
	attacks |= rays[sq][EAST];
	if (rays[sq][EAST] & blockers)
	{
		attacks &= ~(rays[bitScanReverse(rays[sq][EAST] & blockers)][EAST]);
	}
	attacks |= rays[sq][WEST];
	if (rays[sq][WEST] & blockers)
	{
		attacks &= ~(rays[bitScanForward(rays[sq][WEST] & blockers)][WEST]);
	}
	return attacks;
}

Bitboard MoveGen::getQueenMoves(int sq, Bitboard blockers)
{
	return (getBishopMoves(sq, blockers) | getRookMoves(sq, blockers));
}

Bitboard MoveGen::getCachedPawnMoves(int sq, PieceColors color)
{
	if (color == WHITE) return wPawnMoveCache[sq];
	else return bPawnMoveCache[sq];
}

Bitboard MoveGen::getCachedPawnCaptures(int sq, PieceColors color)
{
	if (color == WHITE) return wPawnCache[sq];
	else return bPawnCache[sq];
}

Bitboard MoveGen::getCachedKnightMoves(int sq)
{
	return knightCache[sq];
}

Bitboard MoveGen::getCachedBishopMoves(int sq, Bitboard blockers)
{
	blockers &= bishopMasks[sq];
	Bitboard key = _pext_u64(blockers, bishopMasks[sq]);
	return bishopCache[sq][key];
}

Bitboard MoveGen::getCachedRookMoves(int sq, Bitboard blockers)
{
	blockers &= rookMasks[sq];
	Bitboard key = _pext_u64(blockers, rookMasks[sq]);
	return rookCache[sq][key];
}

Bitboard MoveGen::getCachedQueenMoves(int sq, Bitboard blockers)
{
	blockers &= queenMasks[sq];
	Bitboard key = _pext_u64(blockers, queenMasks[sq]);
	return queenCache[sq][key];
}

Bitboard MoveGen::getCachedKingMoves(int sq)
{
	return kingCache[sq];
}

Bitboard MoveGen::getCachedPieceMoves(int pieceType, int sq, Bitboard blockers, PieceColors color)
{
	switch (pieceType)
	{
	case PieceTypes::KING:
	{
		return getCachedKingMoves(sq);
		break;
	}
	case PieceTypes::KNIGHT:
	{
		return getCachedKnightMoves(sq);
		break;
	}
	case PieceTypes::PAWN:
	{
		return getCachedPawnMoves(sq, color);
		break;
	}
	case PieceTypes::BISHOP:
	{
		return getCachedBishopMoves(sq, blockers);
		break;
	}
	case PieceTypes::ROOK:
	{
		return getCachedRookMoves(sq, blockers);
		break;
	}
	case PieceTypes::QUEEN:
	{
		return getCachedQueenMoves(sq, blockers);
		break;
	}
	}
}

bool MoveGen::checkDetector(int kingsq, int kingcolor,
	int lastmovefiguretype, int lastmovefiguresq, Bitboard blockers)
{
	uint_8 translateKingSq = checkTranslationTable[kingsq];
	uint_8 translateLastMoveFigureSq = checkTranslationTable[lastmovefiguresq];

	if (!(checkArray[base + translateKingSq - translateLastMoveFigureSq] &
		checkFigureCode[lastmovefiguretype])) return false;

	switch (lastmovefiguretype)
	{
	case PieceTypes::PAWN:
	{
		if (kingcolor == PieceColors::WHITE)
		{
			if (lastmovefiguresq > kingsq) return true;
			else return false;
		}
		if (kingcolor == PieceColors::BLACK)
		{
			if (lastmovefiguresq < kingsq) return true;
			else return false;
		}
		break;
	}
	case PieceTypes::KNIGHT:
	{
		return true;
		break;
	}
	case PieceTypes::BISHOP:
	{
		Bitboard king = Bitboard(1) << kingsq;
		return ((getCachedBishopMoves(lastmovefiguresq, blockers) & king) ? true : false);
		break;
	}
	case PieceTypes::ROOK:
	{
		Bitboard king = Bitboard(1) << kingsq;
		return ((getCachedRookMoves(lastmovefiguresq, blockers) & king) ? true : false);
		break;
	}
	case PieceTypes::QUEEN:
	{
		Bitboard king = Bitboard(1) << kingsq;
		return ((getCachedQueenMoves(lastmovefiguresq, blockers) & king) ? true : false);
		break;
	}
	}
}

void MoveGen::showMoveList(TMove* list,int size)
{
	int i = 0;
	while (i < size)
	{
		std::string pieces[7]{ "KING","QUEEN","ROOK","BISHOP","KNIGHT","PAWN","NO_TYPE"};
		std::string moveTypes[8]{ "DEFAULT_MOVE", "CAPTURE", "FIRST_PAWN_MOVE", "EN_PASSANT", "LONG_CASTLE", "SHORT_CASTLE", "TRANSFORM", "CAPTURE_TRANSFORM" };
		char files[8]{ 'A','B','C','D','E','F','G','H' };
		std::cout << "From: " << files[7 - (list[i].from % 8)] << int(list[i].from / 8 + 1) << "\n";
		std::cout << "To: " << files[7 - (list[i].to % 8)] << int(list[i].to / 8 + 1) << "\n";
		std::cout << "MoveType: " << moveTypes[list[i].moveType] << "\n";
		//if (list[i].moveType == CAPTURE || list[i].moveType == CAPTURE_TRANSFORM)
		std::cout << "Eaten Piece: " << pieces[list[i].opFigure] << "\n";
		//if (list[i].moveType == CAPTURE_TRANSFORM || list[i].moveType == TRANSFORM)
		std::cout << "Transform into: " << pieces[list[i].transformPiece] << "\n";
		std::cout << "Sort Field: " << std::to_string(list[i].sortField) << "\n\n\n";
		++i;
	}
}

bool MoveGen::isCastlingPossible(Position& pos, int CastleType)
{
	if (pos.activeColor == WHITE)
	{
		if (CastleType == SHORT_CASTLE)
			return pos.wShortCastle && (pos.pieceHash[1].type == NO_TYPE) && (pos.pieceHash[2].type == NO_TYPE);
		else
			return pos.wLongCastle && (pos.pieceHash[4].type == NO_TYPE) && (pos.pieceHash[5].type == NO_TYPE) &&
			(pos.pieceHash[6].type == NO_TYPE);
	}
	else
	{
		if (CastleType == SHORT_CASTLE)
			return pos.bShortCastle &&( pos.pieceHash[57].type == NO_TYPE) && (pos.pieceHash[58].type == NO_TYPE);
		else
			return pos.bLongCastle && (pos.pieceHash[60].type == NO_TYPE) && (pos.pieceHash[61].type == NO_TYPE) &&
			(pos.pieceHash[62].type == NO_TYPE);
	}
}

uint_64 MoveGen::random_uint64()
{
	std::random_device rd;
	std::mt19937_64 eng(rd());
	std::uniform_int_distribution<uint_64> distr;
	return distr(eng);
}





