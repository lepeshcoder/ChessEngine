#ifndef SEARCH
#define SEARCH

#include"../Types/Constants.h"
#include"../Types/Types.h"
#include"ISearch.h"

using namespace SearchConstants;
using namespace SearchTypes;

class Search : ISearch
{
public:
	TMove getBestMove(Position& pos) override;


};

#endif // !I_SEARCH
