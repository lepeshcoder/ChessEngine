#ifndef ENDGAME_TABLE
#define ENDGAME_TABLE

#include"../Types/Constants.h"
#include"../Types/Types.h"
#include"IEndgameTable.h"

using namespace EndGameTableConstants;
using namespace EndGameTableTypes;

class EndGameTable : IEndGameTable {
public:
	// Get best move in position accourding Endgame Tables
	TMove getEndGameTableMove(Position& pos) override;

};

#endif 
