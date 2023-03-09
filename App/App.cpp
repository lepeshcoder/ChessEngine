#ifndef APP
#define APP

#include<iostream>
#include"../Evaluation/Evaluation.h"
#include"../Types/Constants.h"
#include"../Types/Types.h"
#include"../NewEngine/MoveGen.h"
#include"../EndgameTables/EndGameTable.h"
#include"../OpeningBook/OpeningBook.h"
#include"../Search/Search.h"

using namespace std;

int main()
{
	MoveGen a;
	a.ViewBitboard(Bitboard(7));
	std::cout<<"\n\n";
	std::cout << a.bitScanForward(Bitboard(7))<<"\n\n";
	std::cout << a.bitScanReverse(Bitboard(7));
}

#endif