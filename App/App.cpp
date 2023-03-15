#include<iostream>

#include"MoveGen.h"
#include"Search.h"
#include"Evaluation.h"
#include"Game.h"

using namespace std;




/*
*  TODO LIST:
*  
*  1)  Write implementation of 4 main Functions in MoveGen that inherited from IMoveGen
*  2)  Write Simple Evaluation
*  3)  Write Negascout
*  4)  Add OpeningBook and EndGame Tables
*  5)  Add UCI_Communication
*  6)  Make Performance Test of Main Functions and save it
* 
* 
* 
*/
int main()
{
	Position pos;

	MoveGen::InitMoveGen();
	Game::initPosition(pos,"r1b2rk1/1pp1pp1p/2n4b/pB1pPNp1/P1qP3n/2N1BQ1P/1PP2PP1/3R1RK1 b q - 0 1");
	
	TMove* Captures = NULL;
	TMove* Moves = NULL;

	double begin_time = clock();

	for (int i = 0; i < 1e6; i++)
	{
		MoveGen::generateAndSortAllMoves(pos);
	}

	double time = clock() - begin_time;

	std::cout << "Elapsed Time: " << time << " ms.\n";
}
