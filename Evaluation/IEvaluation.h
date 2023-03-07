#ifndef I_EVALUATION
#define I_EVALUATION

#include"../Types/Constants.h"
#include"../Types/Types.h"

using namespace EvaluationConstants;
using namespace EvaluationTypes;

class IEvaluation
{

public:
	// Evaluate position score including all positional factors
	virtual EvalScore getPositionScore(Position& pos) = 0;

};

#endif // !I_EVALUATION

