#include"Game.h"

using namespace Game;


PosInfo positionInfo[400] = {};

void Game::initPosition(Position& pos, std::string fenstring)
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
	pos.fiftyMovesRule = pos.fiftyMovesRule = fenstring[i] - '0';
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
				int sq = MoveGen::bitScanForward(pieces);
				pos.pieceHash[sq].color = color;
				pos.pieceHash[sq].type = pieceType;
				pieces &= (pieces - 1);
			}
		}
	}

}

std::string Game::getPositionFen(Position& pos)
{
	return std::string();
}
