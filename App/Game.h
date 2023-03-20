#ifndef  GAME
#define GAME

#include"Constants.h"
#include"Types.h"
#include"MoveGen.h"

using namespace GameConstants;
using namespace GameTypes;


namespace Game {


	extern PosInfo positionInfo[400];
	
	void initPosition(Position& pos, std::string fen);

	std::string getPositionFen(Position& pos);

	std::string getFullPositionInfo(Position& pos);





}

#endif // ! GAME
