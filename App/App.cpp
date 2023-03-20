#include<iostream>

#include"MoveGen.h"
#include"Search.h"
#include"Evaluation.h"
#include"Game.h"

using namespace std;




/*
*  TODO LIST:
* 
*	1) fix getPositionFen
*   2) test MakeMove and UnMakeMove and MoveGenerate
*   3) perft
* 
* 
*/
int main()
{
	std::string fen = "r3k2r/1P2qpp1/2n1bn1p/1B2p3/pb6/B1N2N1P/P1PPQPP1/R3K2R w KQkq - 4 12";
	Position pos;

	MoveGen::InitMoveGen();
	Game::initPosition(pos,fen);

	
	
	TMove* moves = MoveGen::generateAndSortAllMoves(pos);
	MoveGen::showMoveList(moves);
	int moveIndex = 0;
	std::string info1 = Game::getFullPositionInfo(pos);

	while (moves[moveIndex].transformPiece != PAWN)
	{
		MoveGen::makeMove(pos, moves[moveIndex]);
		MoveGen::unMakeMove(pos, moves[moveIndex]);
		std::string info2 = Game::getFullPositionInfo(pos);
		if (strcmp(info1.c_str(), info2.c_str()) != 0)
		{
			std::cout << "wrong";
		}
		moveIndex++;
	}

	/*double begin_time = clock();

	std::cout << Search::NegaScout(pos, -2000, 2000, 7);

	double time = clock() - begin_time;
	std::cout << "Elapsed Time: " << time << " ms.\n";*/


	

	

	
}
