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


/*
*  TODO LIST:
* 
*  1) �������� ����� � ��������� ��������� ������ � ��������� ��������� ����� �����
*  2) �� ���� ��������� getBishopMoves � ��������� getRookMoves � getQueenMoves
*
*/
int main()
{
	Bitboard test = 0x38000000380000;
	MoveGen a;
	/*std::cout << a.bitScanForward(test)<<"\n\n";
	std::cout << a.bitScanReverse(test)<<"\n\n";*/

}

#endif