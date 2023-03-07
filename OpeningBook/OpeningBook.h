#ifndef OPENING_BOOK
#define OPENING_BOOK

#include "../Types/Constants.h"
#include "../Types/Types.h"
#include "IOpeningBook.h"

using namespace OpeningBookConstants;
using namespace OpeningBookTypes;

class OpeningBook : IOpeningBook
{
	// Унаследовано через IOpeningBook
	std::vector<TMove> getBookMoves(Position& pos) override;
};

#endif 

