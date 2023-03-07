#ifndef I_OPENING_BOOK
#define I_OPENING_BOOK
 
#include"../Types/Constants.h"
#include"../Types/Types.h"

using namespace OpeningBookConstants;
using namespace OpeningBookTypes;

class IOpeningBook
{
public:
	// Gets vector of Book Moves sorted by popularity
	virtual std::vector<TMove> getBookMoves(Position& pos) = 0;

};



#endif // !I_OPENING_BOOK

