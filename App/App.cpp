
#include<iostream>
#include"../Evaluation/Evaluation.h"
#include"../Types/Constants.h"
#include"../Types/Types.h"
#include"../NewEngine/MoveGen.h"
#include"../EndgameTables/EndGameTable.h"
#include"../OpeningBook/OpeningBook.h"
#include"../Search/Search.h"

using namespace std;


void initPos(Position& pos, string fenstring, MoveGen& movegen)
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
	else pos.enPassantField = 0;
	i += 2;
	pos.prevFiftyMovesRule = pos.fiftyMovesRule = fenstring[i] - '0';
	i += 2;
	pos.moveCounter = fenstring[i] - '0';
	pos.allBlackPeaces = pos.allWhitePeaces = 0;
	for (int i = 0; i < 6; i++)
	{
		pos.allBlackPeaces |= pos.pos[BLACK][i];
		pos.allWhitePeaces |= pos.pos[WHITE][i];
	}
	pos.allPeaces = pos.allBlackPeaces | pos.allWhitePeaces;
	pos.material = 0;


	for (int color = PieceColors::WHITE; color <= PieceColors::BLACK; color++)
	{
		for (int pieceType = PieceTypes::KING; pieceType <= PieceTypes::PAWN; pieceType++)
		{
			int index = 0;
			Bitboard pieces = pos.pos[color][pieceType];
			while (pieces)
			{
				int sq = movegen.bitScanForward(pieces);
				pos.pieceHash[sq].color = color;
				pos.pieceHash[sq].type = pieceType;
				pieces &= (pieces - 1);
			}
		}
	}
}

/*
*  TODO LIST:
*  
*  1)  Write implementation of 4 main Functions in MoveGen that inherited from IMoveGen
*  2)  
* 
* 
* 
*/
int main()
{
	MoveGen a;
	Position pos;
	initPos(pos,"r4rk1/2p2ppp/3q1n2/pp1pp1N1/1bNnP1b1/1BP1BQ2/PP3PPP/R4RK1 b - -1 13", a);
	Bitboard blockers = (pos.allPeaces);

	double begin_time = clock();

	TMove* Captures = new TMove;
	for (int i = 0; i < 1e6; i++)
	{
		Captures = a.generateAndSortAllCaptures(pos);
	}

	double time = clock() - begin_time;

	std::cout << "elapsed Time: " << time << " ms.\n";
	
	int index = 0;
	while (Captures[index].opFigure != NO_TYPE)
	{
		std::cout <<"from: " << int(Captures[index].from)<< " to: " << int(Captures[index].to) <<" sort: " << int(Captures[index++].sortField) << "\n";
	}
	


	/*for (auto& piece : pos.pieceHash)
	{
		std::cout << "color: " << int(piece.second.color) << " type: " << int(piece.second.type) << " sq: " << int(piece.second.sq) << "\n";
	}*/

	

	

	
}
