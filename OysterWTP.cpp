#include "OysterWTP.h"
#include "KnapsackSolver.h"
#include "KnapsackTable.h"
#include "Driver.h"

#include <iostream>

OysterWTP::OysterWTP() : HeuristicPerson{} { }

OysterWTP::OysterWTP(const PersonBase& other) : HeuristicPerson{ other } { }

// Algorithm Episode IV: A New Hope
void OysterWTP::willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
	Pointer<PersonBase> other, bool& willing, unsigned short& willingness) {

	// Check if there is any demand at all
	if ((quantity == 0) || (this->demands(good) == false)) {
		willing = false;
		willingness = 0;
		return;
	}

	GoodPriceQuantityMap accessibleGoodsUninclusive{ this->neighborSellOffers };
	auto accessibleGoodsIt{ accessibleGoodsUninclusive.find(good) };

	// Take out whatever quantity is being sold by *this* neighbor.
	if (accessibleGoodsIt != accessibleGoodsUninclusive.end()) {

		for (const auto& currentPersonConnection : this->personTrades) {

			if (currentPersonConnection.first != other) {
				continue;
			}

			// Okay, so we have the right person.
			const GoodPriceQuantityMap& gpqm{ currentPersonConnection.second };
			for (const auto& currentGpqm : gpqm) {

				const GOOD_ID_TYPE& currentGood{ currentGpqm.first };
				const PriceQuantityMap& pqm{ currentGpqm.second };

				if (currentGood != good) {
					continue;
				}
				// Okay, so now we have the right good.
				for (const auto& currentPq : pqm) {

					const unsigned short& currentPrice{ currentPq.first };
					const unsigned short& currentQuantity{ currentPq.first };

					// And now do the actual subtracting.
					auto currentPriceIt{ accessibleGoodsIt->second.find(currentPrice) };
					unsigned short& updatedQuantity{ currentPriceIt->second };

					if (updatedQuantity > currentQuantity) {
						updatedQuantity -= currentQuantity;
					}
					else {
						updatedQuantity = 0;
					}

				}

			}

		}

	}

	unsigned short moneyWithSales{ 0 };
	GoodPriceQuantityMap realAccessibleGoodsUninclusive{
		this->getRealAccessibleGoods(moneyWithSales, accessibleGoodsUninclusive) };

	GoodQuantityMap uninclusiveHoldings{ this->utilityFunction.optimalQuantities(
		KnapsackSolver{}, moneyWithSales, realAccessibleGoodsUninclusive) };

	while (1 < 7) {

		// And now the map for if we give it for free.
		GoodPriceQuantityMap accessibleGoodsInclusive{ accessibleGoodsUninclusive };

		accessibleGoodsInclusive[good][willingness] += quantity;

		GoodPriceQuantityMap realAccessibleGoodsInclusive{
			this->getRealAccessibleGoods(moneyWithSales, accessibleGoodsInclusive) };

		if (willingness >= moneyWithSales) {
			willingness = moneyWithSales;
			break;
		}

		moneyWithSales -= willingness;

		GoodQuantityMap inclusiveHoldings{ this->utilityFunction.optimalQuantities(
			KnapsackSolver{}, moneyWithSales, realAccessibleGoodsInclusive) };

		const unsigned short& inclusiveHoldingQuantity{ inclusiveHoldings[good] };

		// Now we need to see if there are others of this good with priority over us.
		unsigned short priorityHoldings{ 0 };
		const PriceQuantityMap& pqm{ realAccessibleGoodsInclusive[good] };
		for (const auto& currentPQ : pqm) {

			const unsigned short& currentPrice{ currentPQ.first };
			const unsigned short& currentQuantity{ currentPQ.second };

			if (currentPrice >= willingness) {
				continue;
			}

			priorityHoldings += currentQuantity;

		}

		unsigned short holdingsFromUs{ 0 };
		if (inclusiveHoldingQuantity > priorityHoldings) {
			holdingsFromUs = inclusiveHoldingQuantity - priorityHoldings;
		}

		// Now we want to die if we aren't buying all of this.
		if (holdingsFromUs < quantity) {
			std::cout << "Willing: " << willing << " Willingness: " << willingness << std::endl;
			break;
		}
		willingness += WTP_ADJUST;

	}

	if (willingness >= 1) {
		willingness -= 1;
	}

	willing = (willingness > 0);

}
