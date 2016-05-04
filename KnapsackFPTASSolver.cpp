#include "KnapsackFPTASSolver.h"
#include "KnapsackSolver.h"
#include "Parameters.h"

GoodQuantityMap KnapsackFPTASSolver::operator()(const UtilityFunction* function,
	unsigned short money, const GoodPriceQuantityMap& priceQuantities) {

	// Scale down according to the factor
	GoodPriceQuantityMap adjusted{ priceQuantities };

	// Run through normal Knapsack after solving.
	return KnapsackSolver{}(function, money, adjusted);

}