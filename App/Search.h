#ifndef SEARCH
#define SEARCH

#include"Constants.h"
#include"Types.h"
#include"MoveGen.h"
#include"Evaluation.h"

using namespace SearchConstants;
using namespace SearchTypes;

namespace Search {
	int NegaScout(Position& pos, int alpha, int beta, int depth);

};
#endif // !SEARCH
