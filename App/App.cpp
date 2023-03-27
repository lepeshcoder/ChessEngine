#include<iostream>

#include"MoveGen.h"
#include"Search.h"
#include"Evaluation.h"
#include"Game.h"

using namespace std;

bool kal(Position& pos, int depth)
{
	TMove* mainLine = new TMove[depth];
	std::string expected = Game::getFullPositionInfo(pos);
	for (int i = 0; i < depth; i++)
	{
		int size = 0;
		TMove* moves = MoveGen::generateAndSortAllMoves(pos, size);
		int index = rand() % size;
		mainLine[i] = moves[index];
		delete[] moves;
		MoveGen::makeMove(pos, mainLine[i]);		
	}
	
	for (int i = depth - 1; i >= 0; i--)
	{
		MoveGen::unMakeMove(pos, mainLine[i]);
	}

	std::string equal = Game::getFullPositionInfo(pos);
	if (strcmp(expected.c_str(), equal.c_str()) != 0)
	{
		std::cout << "wrong!!!\n";
		std::cout << "expected: " << expected << "\n";
		std::cout << "equal: " << equal << "\n";
		MoveGen::showMoveList(mainLine, depth);
		std::ofstream file("C:\\Users\\LepeshCoder\\Desktop\\wrongMoves.bin", ios::binary);
		if (!file)
		{
			std::cout << "cant open file to write!\n";
		}
		for (int i = 0; i < depth; i++)
		{
			file.write(reinterpret_cast<char*>(&mainLine[i]), sizeof(TMove));
		}	
		file.close();
		return true;	
	}
	delete[] mainLine;
	return false;
}


/*
*  TODO LIST:
* 
*   1) attack and defend maps
*	2) Write legak move gen based on a/d maps
*   3) parallelism in movegen (mb threadpool)
* 
* 
*/
int main()
{

	
	srand(time(NULL));
	std::string fen = "6R1/8/8/5K2/8/8/8/7k w - - 0 1";
	MoveGen::InitMoveGen();

	Position  pos;
	Game::initPosition(pos, fen);

	//std::cout << Game::getFullPositionInfo(pos);

	/*int a = 0;
	while (!kal(pos, 50))
	{
		a++;
	}
	std::cout << "a: " << a;*/

	

	/*TMove*  mainLine = new TMove[8];
	std::ifstream file("C:\\Users\\LepeshCoder\\Desktop\\wrongMoves.bin", ios::binary);
	if (!file) {
		cerr << "Failed to open file for reading!" << endl;
		return 1;
	}
	for (int i = 0; i < 8; i++)
	{
		file.read(reinterpret_cast<char*>(&mainLine[i]), sizeof(TMove));
	}
	MoveGen::showMoveList(mainLine,8);

	std::string info = Game::getFullPositionInfo(pos);
	std::cout <<"\n\n\nSTART POS: \n\n\n" << info << "\n\n\n";

	for (int i = 0; i < 8; i++)
	{
		MoveGen::makeMove(pos, mainLine[i]);
		info = Game::getFullPositionInfo(pos);
		std::cout << "\n\n\nMOVE: "<<i<<"\n\n\n" << info << "\n\n\n";
	}

	for (int i = 7; i >= 0; i--)
	{
		MoveGen::unMakeMove(pos, mainLine[i]);
		info = Game::getFullPositionInfo(pos);
		std::cout << "\n\n\nUN_MOVE: " << i << "\n\n\n" << info << "\n\n\n";
	}*/

	







	//file.close();

	//int size = 0;
	//TMove* moves = MoveGen::generateAndSortAllQuietMoves(pos, size);

	//MoveGen::makeMove(pos, moves[1]);

	//std::cout << Game::getFullPositionInfo(pos);

	/*int size = 0;
	TMove* moves = MoveGen::generateAndSortAllMoves(pos, size);
	string info1 = Game::getFullPositionInfo(pos);
	for (int i = 0; i < size; i++)
	{
		MoveGen::makeMove(pos, moves[i]);
		MoveGen::unMakeMove(pos, moves[i]);
		string info2 = Game::getFullPositionInfo(pos);
		if (strcmp(info1.c_str(), info2.c_str()) != 0)
		{
			std::cout << "wrong!!! i:" <<i<<"\n";
		}
	}*/


	

	/*int size = 0;



	int captureSize = 0, quietMoveSize = 0,allMovesSize = 0;


	captures = MoveGen::generateAndSortAllCaptures(pos, captureSize);
	quietMoves = MoveGen::generateAndSortAllQuietMoves(pos, quietMoveSize);
	allMoves = MoveGen::generateAndSortAllMoves(pos, allMovesSize);


	std::cout << "Captures: \nsize:"<< captureSize<<"\n";
	MoveGen::showMoveList(captures, captureSize);

	std::cout << "Quiet Moves: \nsize:" << quietMoveSize << "\n";
	MoveGen::showMoveList(quietMoves, quietMoveSize);

	std::cout << "All Moves: \nsize:" << allMovesSize << "\n";
	MoveGen::showMoveList(allMoves, allMovesSize);*/


	//int size = 0;
	//TMove* quietMoves = NULL;
	//TMove* captures = NULL;
	//TMove* allMoves = NULL;
	//
	

	//for (int i = 0; i < 1e3; i++)
	//{
	//	/*captures = MoveGen::generateAndSortAllCaptures(pos, size);
	//	delete[] captures;
	//	quietMoves = MoveGen::generateAndSortAllQuietMoves(pos, size);
	//	delete[] quietMoves;
	//	*/
	//	allMoves = MoveGen::generateAndSortAllMoves(pos, size);
	//	delete[] allMoves;
	//}


	
	//int l;
	//cin >> l;

	//MoveGen::showMoveList(quietMoves,size);

	double beginTime = clock();	

	std::cout<<"score: "<<Search::NegaScout(pos, -2000, 2000, 6)<<"\n";

	double time = clock() - beginTime;

	std::cout << "Elapsed Time: " << time << " ms.\n"; 


}
