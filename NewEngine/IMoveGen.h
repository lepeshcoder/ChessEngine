#ifndef I_MOVE_GEN
#define I_MOVE_GEN

#include"../Types/Types.h"
#include"../Types/Constants.h"

class IMoveGen
{
public:

	virtual void MakeMove(Position& position, TMove move) = 0;

	virtual void UnMakeMove(Position& position, TMove move) = 0;

	virtual std::vector<TMove> GenerateAndSortAllCaptures(Position& position, PieceColors activeColor) = 0;

	virtual std::vector<TMove> GenerateAndSortAllMoves(Position& position, PieceColors activeColor) = 0;

private:


   
};
#endif 

