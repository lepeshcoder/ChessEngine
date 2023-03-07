#ifndef I_ENDGAME_TABLE
#define I_ENDGAME_TABLE

#include"../Types/Constants.h"
#include"../Types/Types.h"

using namespace EndGameTableConstants;
using namespace EndGameTableTypes;

class IEndGameTable {
public:
	// Get best move in position accourding Endgame Tables
	virtual TMove getEndGameTableMove(Position& pos) = 0;

};

#endif 
