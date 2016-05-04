#include "KnapsackSolver.h"
#include "MarginalGoodCell.h"
#include "HelperStructs.h"
#include "KnapsackTable.h"

#include <iostream>

GoodQuantityMap KnapsackSolver::operator()(const UtilityFunction* function,
	unsigned short money, const GoodPriceQuantityMap& priceQuantities) {

	return this->solve(1.0, function, money, priceQuantities);

}

GoodQuantityMap KnapsackSolver::solve(double scalingFactor, const UtilityFunction* utilityFunction,
	unsigned short money, const GoodPriceQuantityMap& priceQuantities) {

	// Hand off to KnapsackTable. It does everything that this function used to do,
	// although I expect this to get more responsibility later.

	KnapsackTable table{ priceQuantities, utilityFunction };
	table.addDollars(money);
	return table.maxUtilityCell().getTotalHoldings();

}
