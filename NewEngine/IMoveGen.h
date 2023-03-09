#ifndef I_MOVE_GEN
#define I_MOVE_GEN

#include"../Types/Types.h"
#include"../Types/Constants.h"

class IMoveGen
{
public:

	// Change position accourding move
	virtual void makeMove(Position& position, TMove move) = 0;

	// Cancels move in position
	virtual void unMakeMove(Position& position, TMove move) = 0;
	
	// Give an array of all captures sorted by capture cost
	virtual std::vector<TMove> generateAndSortAllCaptures(Position& position) = 0;
	
	// Give an array of all Moves sorted by ...
	virtual std::vector<TMove> generateAndSortAllMoves(Position& position) = 0;
   
};
#endif 

