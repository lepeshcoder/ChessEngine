#include<iostream>

#include"MoveGen.h"
#include"Search.h"
#include"Evaluation.h"
#include"Game.h"

using namespace std;




/*
*  TODO LIST:
* 
*	1) Think about attack-defend maps
*   2) zobrist hash
*   3)
* 
* 
*/
int main()
{
	std::string fen = "1Q6/5r2/k7/3KB1n1/8/8/8/8 w - - 6 4";
	MoveGen::InitMoveGen();

	Position  pos;
	Game::initPosition(pos, fen);

	std::cout << Search::NegaScout(pos, -2000, 2000, 6);


}
