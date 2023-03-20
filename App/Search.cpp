#include"Search.h"

using namespace Search;

int kal = 0;

int Search::NegaScout(Position& pos, int alpha, int beta, int depth)
{
	int score = -2000, tmp, i = 0;
	if (depth <= 0) return pos.material;
	TMove* allMoves = MoveGen::generateAndSortAllMoves(pos);
	while (allMoves[i].transformPiece != PAWN && alpha < beta)
	{		
		MoveGen::makeMove(pos, allMoves[i]);
		tmp = -NegaScout(pos,-(alpha + 1), -alpha, depth - 1);
		if (tmp > alpha && tmp < beta)
		{
			tmp = -NegaScout(pos, -beta, -tmp, depth - 1);
		}
		MoveGen::unMakeMove(pos, allMoves[i]); 
		if (tmp > score) score = tmp;
		if (score > alpha) alpha = score;
		++i;
	}
	delete[] allMoves;
	return score;
}
