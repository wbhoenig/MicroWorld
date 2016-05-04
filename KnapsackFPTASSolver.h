#ifndef KNAPSACK_FPTAS_SOLVER_H
#define KNAPSACK_FPTAS_SOLVER_H

#include "HelperStructs.h"
#include "UtilityFunction.h"

class KnapsackFPTASSolver {

public:
	GoodQuantityMap operator()(const UtilityFunction* function,
		unsigned short money, const GoodPriceQuantityMap& priceQuantities);

};

#endif
