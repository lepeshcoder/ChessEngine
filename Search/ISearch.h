#ifndef I_SEARCH
#define I_SEARCH

#include"../Types/Constants.h"
#include"../Types/Types.h"

using namespace SearchConstants;
using namespace SearchTypes;

class ISearch {
public:
	// Get best move in position
	virtual TMove getBestMove(Position& pos) = 0;

};

#endif // !I_SEARCH
