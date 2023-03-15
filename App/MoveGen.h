#ifndef MOVE_GEN
#define MOVE_GEN


#include <iostream>
#include <immintrin.h>
#include <fstream>
#include "Types.h"
#include "Constants.h"
#include "Game.h"

using namespace MoveGenTypes;
using namespace MoveGenConstants;



namespace MoveGen{

	// All precalculated arrays were initialized in the constructor
	void InitMoveGen();

	// Унаследовано через IMoveGen
	void makeMove(Position& position, TMove move);

	void unMakeMove(Position& position, TMove move);

	TMove* generateAndSortAllCaptures(Position& position);

	TMove* generateAndSortAllMoves(Position& position);


		// View bitboard to stdout or file
	void viewBitboard(const Bitboard& bb, const std::string& filePath = "");

	// A bitscan reverse is used to find the index of the most significant 1 bit 
	// msb - 63 lsb - 0
	inline int bitScanReverse(Bitboard bb);

	//  A bitscan forward is used to find the index of the least significant 1 bit
	inline int bitScanForward(Bitboard bb);

	// Initialize Rays array
	void initRays();

	// Additional function for initialazing rays array
	Bitboard eastN(Bitboard board, int n);

	// Additional function for initialazing rays array
	Bitboard westN(Bitboard board, int n);

	// Additional function for getting column at chess board from cell index
	inline int col(int sq);

	// Additional function for getting row at chess board from cell index
	inline int row(int sq);

	// Initialize array of rookMasks
	void initRookMasks();

	// Initialize array of bishopMasks
	void initBishopMasks();

	// Initialize array of queenMasks
	void initQueenMasks();

	// Initialize Cache of non-sliding peaces
	void initPawnMoveCache();

	void initPawnCaptureCache();

	void initKnightCache();

	void initKingCache();

	void initBishopCache();

	void initRookCache();

	void initQueenCache();

	// Return bitboard with all moves of pawns without captures ( including FIRST_PAWN_MOVE )
	Bitboard getPawnMoves(PieceColors color, Bitboard pawns);

	// Return bitboard with all captures of pawns
	Bitboard getPawnCaptures(PieceColors color, Bitboard pawns);

	// Return bitboard with all knights moves
	Bitboard getKnightMoves(Bitboard knights);

	// Return bitboard with all king moves
	Bitboard getKingMoves(Bitboard king);

	// Returns bitboard with all bishop moves for cell and set of blockers 
	// Blockers - all peaces without a piece for which moves are counted
	// bottome left cell is 0 and top right is 63
	Bitboard getBishopMoves(int sq, Bitboard blockers);

	// Returns bitboard with all rook moves for cell and a set of blockers
	Bitboard getRookMoves(int sq, Bitboard blockers);

	// Returns bitboard with all queen moves for cell and a set of blockers
	Bitboard getQueenMoves(int sq, Bitboard blockers);


	// Returns cached pseudolegal moves
	inline Bitboard getCachedPawnMoves(int sq, PieceColors color);

	inline Bitboard getCachedPawnCaptures(int sq, PieceColors color);

	inline Bitboard getCachedKnightMoves(int sq);

	inline Bitboard getCachedBishopMoves(int sq, Bitboard blockers);

	inline Bitboard getCachedRookMoves(int sq, Bitboard blockers);

	inline Bitboard getCachedQueenMoves(int sq, Bitboard blockers);

	inline Bitboard getCachedKingMoves(int sq);

	inline Bitboard getCachedPieceMoves(int pieceType, int sq, Bitboard blockers, PieceColors color);


	// Check detector that returns true if king is under attack by figure that move last 
	// blockers - all pieces without piece for that we count
	bool checkDetector(int kingsq, int kingcolor,
		int lastmovefiguretype, int lastmovefiguresq, Bitboard blockers);

	// Show Move List at Native 
	void showMoveList(TMove* list);

	bool isCastlingPossible(Position& pos, int CastleType);

};


#endif 