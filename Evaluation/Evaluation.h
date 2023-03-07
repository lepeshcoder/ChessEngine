#ifndef EVALUATION
#define EVALUATION

#include "../Types/Constants.h"
#include "../Types/Types.h"
#include "IEvaluation.h"

using namespace EvaluationTypes;
using namespace EvaluationConstants;

class Evaluation : IEvaluation
{
	// Унаследовано через IEvaluation
	EvalScore getPositionScore(Position& pos) override;
};

#endif // !EVALUATION
