#ifndef MOVE_GEN
#define MOVE_GEN

#include <immintrin.h>
#include "../Types/Types.h"
#include "../Types/Constants.h"
#include "IMoveGen.h"

using namespace MoveGenTypes;
using namespace MoveGenConstants;

class MoveGen : IMoveGen
{
private:
	

public:

	// Унаследовано через IMoveGen
	virtual void MakeMove(Position& position, TMove move) override;

	virtual void UnMakeMove(Position& position, TMove move) override;

	virtual std::vector<TMove> GenerateAndSortAllCaptures(Position& position, PieceColors activeColor) override;

	virtual std::vector<TMove> GenerateAndSortAllMoves(Position& position, PieceColors activeColor) override;

};

#endif 