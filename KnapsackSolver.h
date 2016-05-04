#ifndef KNAPSACK_SOLVER_H
#define KNAPSACK_SOLVER_H

#include "HelperStructs.h"
#include "UtilityFunction.h"

class KnapsackSolver {

public:
	GoodQuantityMap operator()(const UtilityFunction* function, 
		unsigned short money, const GoodPriceQuantityMap& priceQuantities);

	GoodQuantityMap solve(double scalingFactor, const UtilityFunction* function,
		unsigned short money, const GoodPriceQuantityMap& priceQuantities);

};

#endif
