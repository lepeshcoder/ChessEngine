#include"Search.h"

using namespace Search;

std::string kal;

TMove Search::moveLine[7] = {};

int bShortCastle = 0;
int bLongCastle = 0;
int wShortCastle = 0;
int wLongCastle = 0;

int Search::NegaScout(Position& pos, int alpha, int beta, int depth)
{
	if (depth == 6)
	{
		kal = Game::getFullPositionInfo(pos);
	}
	int score = -2000, tmp, i = 0,size = 0;
	if (depth <= 0) return pos.material;
	TMove* allMoves = MoveGen::generateAndSortAllMoves(pos,size);
	while (i < size && alpha < beta)
	{		
		/*if (pos.activeColor == WHITE && allMoves[i].opFigure == KING && allMoves[i].moveType == CAPTURE) return 1000;
		else if(pos.activeColor == BLACK && allMoves[i].opFigure == KING && allMoves[i].moveType == CAPTURE) return -1000;*/
		
		MoveGen::makeMove(pos, allMoves[i]);
		
		//moveLine[6 - depth] = allMoves[i];
		tmp = -NegaScout(pos, -(alpha + 1), -alpha, depth - 1);
		if (tmp > alpha && tmp < beta)
		{
			tmp = -NegaScout(pos, -beta, -tmp, depth - 1);
		} 
		MoveGen::unMakeMove(pos, allMoves[i]);	
		
		if (strcmp(kal.c_str(), Game::getFullPositionInfo(pos).c_str()) != 0 && depth == 6)
		{
			std::cout << "wrong!!!\n";
			std::cout << "old pos: \n\n" << kal << "\n\nnew pos: \n\n" << Game::getFullPositionInfo(pos);
			return 0;
		}
		if (tmp > score) score = tmp;
		if (score > alpha) alpha = score;
		++i;
	}
	delete[] allMoves;
	return score;
}
