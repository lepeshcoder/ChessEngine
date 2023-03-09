#include "MoveGen.h"


MoveGen::MoveGen()
{
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
	
}

void MoveGen::unMakeMove(Position& position, TMove move)
{

}

std::vector<TMove> MoveGen::generateAndSortAllCaptures(Position& position)
{
	return std::vector<TMove>();
}

std::vector<TMove> MoveGen::generateAndSortAllMoves(Position& position)
{
	return std::vector<TMove>();
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
		std::fstream f(filePath,std::ios::app);
		f << result+"\n\n";
		f.close();
	}
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
	return index64[((bb ^ (bb - 1)) * mask) >> 58];
}

void MoveGen::initRays()
{
	for (int i = 0; i < 64; i++)
	{
		rays[NORTH][i] = (FILE_H & ~RANK_1) << i;
		rays[SOUTH][i] = (FILE_A & ~RANK_8) >> (63 - i);
		rays[EAST][i] = (RANK_1 >> (8 - (i & 7))) << ((i >> 3) << 3);
		rays[WEST][i] = ((RANK_1 << ((i & 7) + 1)) & ~RANK_2) << ((i >> 3) << 3);

		rays[NORTH_WEST][i] = westN(DIAG_A8H1 & ~RANK_1, col(i)) << (row(i) << 3);
		rays[NORTH_EAST][i] = eastN(DIAG_A1H8 & ~RANK_1, 7 - col(i)) << (row(i) << 3);
		rays[SOUTH_EAST][i] = eastN(DIAG_A8H1 & ~RANK_8, 7 - col(i)) >> ((7 - row(i)) << 3);
		rays[SOUTH_WEST][i] = westN(DIAG_A1H8 & ~RANK_8, col(i)) >> ((7 - row(i)) << 3);
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
		rookMasks[i] |= (rays[NORTH][i] & (~RANK_8));
		rookMasks[i] |= (rays[SOUTH][i] & (~RANK_1));
		rookMasks[i] |= (rays[EAST][i] & (~FILE_H));
		rookMasks[i] |= (rays[WEST][i] & (~FILE_A));
	}
}

void MoveGen::initBishopMasks()
{
	for (int i = 0; i < 64; i++)
	{
		bishopMasks[i] = 0;
		bishopMasks[i] |= (rays[NORTH_EAST][i]);
		bishopMasks[i] |= (rays[NORTH_WEST][i]);
		bishopMasks[i] |= (rays[SOUTH_EAST][i]);
		bishopMasks[i] |= (rays[SOUTH_WEST][i]);
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

Bitboard MoveGen::getPawnMoves(PieceColors color, Bitboard pawns)
{
	switch (color)
	{
	case WHITE:
		return ((pawns << 8) | ((pawns & RANK_2) << 16));
	case BLACK:
		return ((pawns >> 8) | ((pawns & RANK_7) >> 16));
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
	Bitboard attacks = 0;
	attacks |= rays[NORTH_EAST][sq];
	if (rays[NORTH_EAST][sq] & blockers)
	{
		attacks &= ~(rays[NORTH_EAST][bitScanReverse(rays[NORTH_EAST][sq] & blockers)]);
	}
	attacks |= rays[NORTH_WEST][sq];
	if (rays[NORTH_WEST][sq] & blockers)
	{
		attacks &= ~(rays[NORTH_WEST][bitScanReverse(rays[NORTH_WEST][sq] & blockers)]);
	}
	attacks |= rays[SOUTH_EAST][sq];
	if (rays[SOUTH_EAST][sq] & blockers)
	{
		attacks &= ~(rays[SOUTH_EAST][bitScanForward(rays[SOUTH_EAST][sq] & blockers)]);
	}
	attacks |= rays[SOUTH_WEST][sq];
	if (rays[SOUTH_WEST][sq] & blockers)
	{
		attacks &= ~(rays[SOUTH_WEST][bitScanForward(rays[SOUTH_WEST][sq] & blockers)]);
	}
	return attacks;
}

Bitboard MoveGen::getRookMoves(int sq, Bitboard blockers)
{
	Bitboard attacks = 0;
	attacks |= rays[NORTH][sq];
	if (rays[NORTH][sq] & blockers)
	{
		attacks &= ~(rays[NORTH][bitScanReverse(rays[NORTH][sq] & blockers)]);
	}
	attacks |= rays[SOUTH][sq];
	if (rays[SOUTH][sq] & blockers)
	{
		attacks &= ~(rays[SOUTH][bitScanForward(rays[SOUTH][sq] & blockers)]);
	}
	attacks |= rays[EAST][sq];
	if (rays[EAST][sq] & blockers)
	{
		attacks &= ~(rays[EAST][bitScanForward(rays[EAST][sq] & blockers)]);
	}
	attacks |= rays[WEST][sq];
	if (rays[WEST][sq] & blockers)
	{
		attacks &= ~(rays[WEST][bitScanReverse(rays[WEST][sq] & blockers)]);
	}
	return attacks;
}

Bitboard MoveGen::getQueenMoves(int sq, Bitboard blockers)
{
	return (getBishopMoves(sq, blockers) | getRookMoves(sq, blockers));
}
