#include"Game.h"

using namespace Game;


PosInfo Game::positionInfo[400] = {};

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
	pos.fiftyMovesRule = fenstring[i] - '0';
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
	std::string fen;
	std::string dictionary = "KQRBNPkqrbnp";
	std::string files = "abcdefgh";
	int i = 63;
	int space = 0;
	while (i >= 0)
	{
		if (pos.pieceHash[i].type == NO_TYPE) space++;
		else {
			if (space != 0) fen += char(space) + '0';
			if (pos.pieceHash[i].color != PieceColors::NO_COLOR)
			{
				fen += dictionary[pos.pieceHash[i].color * 6 + pos.pieceHash[i].type];
			}
			space = 0;
		}
		if (i % 8 == 0)
		{
			if (space != 0)
			{
				fen += char(space + '0');
				space = 0;
			}
			if(i != 0) fen += '/';
		}
		i--;
	}
	if (pos.activeColor == WHITE) fen += " w ";
	else fen += " b ";
	if (pos.wShortCastle) fen += 'K';
	if (pos.wLongCastle) fen += 'Q';
	if (pos.bShortCastle) fen += 'k';
	if (pos.bLongCastle) fen += 'q';
	if (!pos.wShortCastle && !pos.wLongCastle &&
		!pos.bShortCastle && !pos.bLongCastle) fen += "- ";
	else fen += " ";
	if (pos.enPassantField)
	{
		int enPassantSq = MoveGen::bitScanForward(pos.enPassantField);
		fen += files[7 - enPassantSq % 8];
		fen += char(int((enPassantSq >> 3) + 1) + '0');
		fen += ' ';
	}
	else fen += "- ";

	fen += std::to_string(pos.fiftyMovesRule);
	fen += ' ';
	fen += std::to_string(pos.moveCounter);
	return fen;
}

std::string Game::getFullPositionInfo(Position& pos)
{
	std::string colors[3]{ "WHITE","BLACK","NO_COLOR"};
	std::string pieces[7]{ "KING","QUEEN","ROOK","BISHOP","KNIGHT","PAWN","NO_TYPE" };
	char files[8]{ 'A','B','C','D','E','F','G','H' };
	std::string info;
	info += "ActiveColor: ";
	info += ((pos.activeColor == WHITE) ? "white" : "black");
	info += "\n";

	info += "MoveCounter: ";
	info += std::to_string(pos.moveCounter);
	info += "\n";

	info += "FiftyMovesRuleCounter: ";
	info += std::to_string(pos.fiftyMovesRule);
	info += "\n";

	info += "material:";
	info += std::to_string(pos.material);
	info += '\n';


	info += "BlackShortCastle: ";
	info += ((pos.bShortCastle == true) ? "true" : "false");
	info += "\n";

	info += "WhiteShortCastle: ";
	info += ((pos.wShortCastle == true) ? "true" : "false");
	info += "\n";

	info += "BlackLongCastle: ";
	info += ((pos.bLongCastle == true) ? "true" : "false");
	info += "\n";

	info += "WhiteShortCastle: ";
	info += ((pos.wLongCastle == true) ? "true" : "false");
	info += "\n";

	info += "EnPassantField: ";
	int sq = MoveGen::bitScanForward(pos.enPassantField);
	if (sq != 0)
	{
		info += files[7 - sq % 8];
		info += std::to_string(sq / 8 + 1);
		info += "\n";
	}
	else info += "NO\n";

	info += "All White Peaces:\n";
	info += MoveGen::bbToString(pos.allWhitePeaces);
	info += "\n";

	info += "All Black Peaces:\n";
	info += MoveGen::bbToString(pos.allBlackPeaces);
	info += "\n";

	info += "All Peaces:\n";
	info += MoveGen::bbToString(pos.allPeaces);
	info += "\n";

	for (int color = PieceColors::WHITE; color <= PieceColors::BLACK; color++)
	{
		for (int pieceType = PieceTypes::KING; pieceType <= PieceTypes::PAWN; pieceType++)
		{
			info += colors[color];
			info += " ";
			info += pieces[pieceType];
			info += ":\n";
			info += MoveGen::bbToString(pos.pos[color][pieceType]);
			info += "\n";
		}
	}

	for (int row = 7; row >= 0; row--)
	{
		for (int col = 7; col >= 0; col--)
		{
			if (pos.pieceHash[row * 8 + col].color != NO_COLOR)
			{
				info += colors[pos.pieceHash[row * 8 + col].color];
				info += " ";
				info += pieces[pos.pieceHash[row * 8 + col].type];
				info += "  ";
			}
			else
			{
				info += "NO  ";
			}
		}
		info += "\n";
	}
	return info;
}
