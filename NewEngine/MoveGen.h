#ifndef MOVE_GEN
#define MOVE_GEN

#include <iostream>
#include <immintrin.h>
#include <fstream>
#include "../Types/Types.h"
#include "../Types/Constants.h"
#include "IMoveGen.h"

using namespace MoveGenTypes;
using namespace MoveGenConstants;

class MoveGen : public IMoveGen
{
private:
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

	/* Caches arrays for not-sliding peaces include all possible moves
	  at empty board for every cell */
	Bitboard kingCache[64];
	Bitboard knightCache[64];
	Bitboard wPawnCache[64];
	Bitboard bPawnCache[64];
	Bitboard wPawnMoveCache[64];
	Bitboard bPawnMoveCache[64];
	
public:

	// All precalculated arrays were initialized in the constructor
	MoveGen();

	// Унаследовано через IMoveGen
    void makeMove(Position& position, TMove move);

	void unMakeMove(Position& position, TMove move) override;

	std::vector<TMove> generateAndSortAllCaptures(Position& position) override;

	std::vector<TMove> generateAndSortAllMoves(Position& position) override;

//private:
	// View bitboard to stdout or file
	void viewBitboard(const Bitboard& bb, const std::string& filePath = "");

	// A bitscan reverse is used to find the index of the most significant 1 bit 
	int bitScanReverse(Bitboard bb);

	//  A bitscan forward is used to find the index of the least significant 1 bit
	int bitScanForward(Bitboard bb);

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
	Bitboard getBishopMoves(int sq, Bitboard blockers);

	// Returns bitboard with all rook moves for cell and a set of blockers
	Bitboard getRookMoves(int sq, Bitboard blockers);

	// Returns bitboard with all queen moves for cell and a set of blockers
	Bitboard getQueenMoves(int sq, Bitboard blockers);



};

#endif 