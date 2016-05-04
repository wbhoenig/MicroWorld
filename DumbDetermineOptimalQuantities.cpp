#include "DumbDetermineOptimalQuantities.h"
#include "HeuristicPerson.h"
#include "ToString.h"
#include "Driver.h"
#include "KnapsackSolver.h"
#include "Parameters.h"

DumbDetermineOptimalQuantities::DumbDetermineOptimalQuantities() : HeuristicPerson{} { }

DumbDetermineOptimalQuantities::DumbDetermineOptimalQuantities(const PersonBase& other) :
	HeuristicPerson{ other } { }

DumbDetermineOptimalQuantities::DumbDetermineOptimalQuantities(const HeuristicPerson& other) :
	HeuristicPerson{ other } { }

void DumbDetermineOptimalQuantities::determineOptimalQuantities() {

	unsigned short moneyWithSales;
	GoodPriceQuantityMap realAccessibleGoods{ this->getRealAccessibleGoods(moneyWithSales, this->neighborSellOffers) };

	if (DEBUG_OUT == true) {

		std::string debugOut{ "About to call optimalQuantities with money: " +
			toString(moneyWithSales) };
		Driver::getDriver()->logMessage(this, "HeuristicPerson", "determineOptimalQuantities",
			debugOut);

		debugOut = "And realAccessibleGoods: " + toString(realAccessibleGoods);
		Driver::getDriver()->logMessage(this, "HeuristicPerson", "determineOptimalQuantities",
			debugOut);


	}

	this->currentHoldings = utilityFunction.optimalQuantities(KnapsackImpl{}, moneyWithSales, realAccessibleGoods);

	// Given this information, we can determine necessary purchases and desired sales as well.
	this->sketchNecessaryPurchases.clear();
	this->sketchDesiredSales.clear();
	for (auto currentHoldingGood : this->currentHoldings) {

		const GOOD_ID_TYPE& goodName{ currentHoldingGood.first };
		const unsigned short& holdingQuantity{ currentHoldingGood.second };

		// See how much is in our endowment and compare.
		auto endowmentIt{ this->goodEndowment.find(goodName) };

		// We need to make sure we have something in the endowment at all to fill
		if (endowmentIt == this->goodEndowment.end()) {
			this->goodEndowment[goodName] = 0;
		}
		endowmentIt = this->goodEndowment.find(goodName);

		if (endowmentIt != this->goodEndowment.end()) {

			const unsigned short& endowmentQuantity{ endowmentIt->second };

			// Zero out both purchases and sales to start with
			this->sketchNecessaryPurchases[goodName] = 0;
			this->sketchDesiredSales[goodName] = 0;

			// Purchases
			if (endowmentQuantity < holdingQuantity) {

				unsigned short purchaseQuantity = holdingQuantity - endowmentQuantity;

				// And add this purchase quantity in.
				this->sketchNecessaryPurchases[goodName] = purchaseQuantity;

				if (DEBUG_OUT == true) {

					std::string debugOut{ "purchaseQuantity set to: " + toString(purchaseQuantity) };
					debugOut += " for good: " + toString(goodName);
					debugOut += " from holdingQuantity: " + toString(holdingQuantity);

					Driver::getDriver()->logMessage(this, "HeuristicPerson",
						"determineOptimalQuantities", debugOut);

				}

			}
			// Sales
			else if (endowmentQuantity > holdingQuantity) {

				unsigned short saleQuantity = endowmentQuantity - holdingQuantity;
				this->sketchDesiredSales[goodName] = saleQuantity;

			}

		}

	}

	if (DEBUG_OUT == true) {

		std::string debugOut{ "currentHoldings after determineOptimalQuantities: " };
		debugOut += toString(this->currentHoldings);

		Driver::getDriver()->logMessage(this, "HeuristicPerson", "determineOptimalQuantities", debugOut);

	}

}

