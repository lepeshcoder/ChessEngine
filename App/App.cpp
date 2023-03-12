
#include<iostream>
#include"../Evaluation/Evaluation.h"
#include"../Types/Constants.h"
#include"../Types/Types.h"
#include"../NewEngine/MoveGen.h"
#include"../EndgameTables/EndGameTable.h"
#include"../OpeningBook/OpeningBook.h"
#include"../Search/Search.h"

using namespace std;


void initPos(Position& pos, string fenstring)
{
	std::string dictionary = "KQRBNPkqrbnp";
	uint_8 arr[64];
	int i = 0, index = 0;
	while (fenstring[i] != ' ')
	{
		if (fenstring[i] == '/') i++;
		else
		{
			if (((fenstring[i] - '0') <= 9) && ((fenstring[i] - '0') >= 0))
			{
				for (int j = 0; j < fenstring[i] - '0'; j++) arr[index++] = '0';
			}
			else
			{
				arr[index++] = fenstring[i];
			}
			i++;
		}
	}
	for (int j = 0; j < dictionary.length(); j++)
	{
		Bitboard temp = 0;
		for (int k = 0; k < 64; k++)
		{
			if (arr[k] == dictionary[j])
			{
				temp |= (Bitboard(1) << (63 - k));
			}
		}
		pos.pos[j / 6][j % 6] = temp;
	}
	i++;
	if (fenstring[i] == 'w') pos.activeColor = WHITE;
	else pos.activeColor = BLACK;
	i += 2;
	pos.bLongCastle = pos.bShortCastle = pos.wLongCastle = pos.wShortCastle = false;
	while (fenstring[i] != ' ')
	{
		if (fenstring[i] == 'K') pos.wShortCastle = true;
		if (fenstring[i] == 'k') pos.bShortCastle = true;
		if (fenstring[i] == 'Q') pos.wLongCastle = true;
		if (fenstring[i] == 'q') pos.bLongCastle = true;
		i++;
	}
	i++;
	int sq = 0;
	if (fenstring[i] != '-')
	{
		sq = 'h' - fenstring[i] + ((fenstring[i + 1] - '0' - 1) << 3);
		pos.enPassantField = Bitboard(1) << sq;
		i++;
	}
	else pos.enPassantField= 0;
	i += 2;
	pos.prevFiftyMovesRule = pos.fiftyMovesRule= fenstring[i] - '0';
	i += 2;
	pos.moveCounter= fenstring[i] - '0';
	pos.allBlackPeaces = pos.allWhitePeaces = 0;
	for (int i = 0; i < 6; i++)
	{
		pos.allBlackPeaces |= pos.pos[BLACK][i];
		pos.allWhitePeaces |= pos.pos[WHITE][i];
	}
	pos.material = 0;
}

/*
*  TODO LIST:
*  
*  1)  Определиться с структурой Tmove 
*  2)  Write implementation of 4 main Functions in MoveGen that inherited from IMoveGen
*  3)  Сделать детектор шахов
*  4)  Написать функцию преобразования позиции FEN
*  5)  
* 
*
*/
int main()
{
	MoveGen a;
	Position pos;
	initPos(pos, "8/8/8/8/3p4/2K5/8/8 w - - 0 1");
	Bitboard blockers = (pos.allBlackPeaces | pos.allWhitePeaces);
	
	a.viewBitboard(blockers);
	std::cout << "\n\n";

	std::cout<< a.checkDetector(21, WHITE, PAWN, 28, blockers)<<"\n\n";
	
}
