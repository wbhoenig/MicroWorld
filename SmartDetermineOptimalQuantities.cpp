#include "SmartDetermineOptimalQuantities.h"
#include "ToString.h"
#include "Driver.h"
#include "KnapsackSolver.h"

SmartDetermineOptimalQuantities::SmartDetermineOptimalQuantities() : HeuristicPerson{} { }

SmartDetermineOptimalQuantities::SmartDetermineOptimalQuantities(const PersonBase& other) :
	HeuristicPerson{ other } { }

SmartDetermineOptimalQuantities::SmartDetermineOptimalQuantities(const HeuristicPerson& other) : 
	HeuristicPerson{ other } { }

void SmartDetermineOptimalQuantities::determineOptimalQuantities() {

	unsigned short moneyWithSales;
	GoodPriceQuantityMap realAccessibleGoods{ this->getRealAccessibleGoods(moneyWithSales, this->neighborSellOffers) };

	if (DEBUG_OUT == true) {

		std::string debugOut{ "About to call optimalQuantities with money: " +
			toString(moneyWithSales) };
		Driver::getDriver()->logMessage(this, "SmartDetermineOptimalQuantities", "determineOptimalQuantities",
			debugOut);

		debugOut = "And realAccessibleGoods: " + toString(realAccessibleGoods);
		Driver::getDriver()->logMessage(this, "SmartDetermineOptimalQuantities", "determineOptimalQuantities",
			debugOut);

	}

	this->currentHoldings = utilityFunction.optimalQuantities(KnapsackImpl{}, moneyWithSales, realAccessibleGoods);

}
