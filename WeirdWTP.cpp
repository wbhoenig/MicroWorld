#include "WeirdWTP.h"
#include "PersonBase.h"
#include "Driver.h"
#include "KnapsackSolver.h"
#include "KnapsackTable.h"
#include "Parameters.h"
#include "ToString.h"

#include <iostream>
#include <cassert>

WeirdWTP::WeirdWTP() : HeuristicPerson{} { }

WeirdWTP::WeirdWTP(const PersonBase& other) : HeuristicPerson{ other } { }

void WeirdWTP::willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
	Pointer<PersonBase> other, bool& willing, unsigned short& willingness) {

	if (DEBUG_OUT == true) {

		std::string debugOut{ "Finding WTP for good: " + toString(good) };
		debugOut += " quantity: " + toString(quantity);
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

	}

	// Check if there is any demand at all
	bool demands{ this->demands(good) };
	if ((quantity == 0) || (demands == false)) {
		willing = false;
		willingness = 0;
		if (DEBUG_OUT == true) {
			std::string message{ "Exiting willingess to pay REASON: " };
			message += "quantity: " + toString(quantity) + " demands: " + toString(demands);
			Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", message);
		}
		return;
	}

	this->receiveNewOffers();

	GoodPriceQuantityMap accessibleGoodsUninclusive{ this->neighborSellOffers };
	auto accessibleGoodsIt{ accessibleGoodsUninclusive.find(good) };

	// Take out whatever quantity is being sold by *this* neighbor.
	if (accessibleGoodsIt != accessibleGoodsUninclusive.end()) {

		const GoodPriceQuantityMap& gpqm{
			other->getGoodPriceQuantitiesSellingTo(Pointer<PersonBase>{this}) };

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

				if (currentPriceIt == accessibleGoodsIt->second.end()) {
					continue;
				}

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
	
	unsigned short moneyWithSales{ 0 };
	GoodPriceQuantityMap realAccessibleGoodsUninclusive{
		this->getRealAccessibleGoods(moneyWithSales, accessibleGoodsUninclusive) };

	GoodQuantityMap uninclusiveHoldings{ this->utilityFunction.optimalQuantities(
		KnapsackSolver{}, moneyWithSales, realAccessibleGoodsUninclusive) };

	double desiredUtility{ this->utilityFunction.getUtility(uninclusiveHoldings) };

	if (DEBUG_OUT == true) {

		// Big logging load here.
		std::string debugOut{ "moneyWithSales: " + toString(moneyWithSales) };
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

		debugOut = "desiredUtility: " + toString(desiredUtility);
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

		debugOut = "realAccessibleGoodsUninclusive: [" +
			toString(realAccessibleGoodsUninclusive) + ']';
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

		debugOut = "accessibleGoodsUninclusive: [" + toString(accessibleGoodsUninclusive) + ']';
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

		debugOut = "uninclusiveHoldings: [" + toString(uninclusiveHoldings) + ']';
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

	}

	willingness = 0;

	while (1 < 7) {

		// And now the map for if we give it for free.
		GoodPriceQuantityMap accessibleGoodsInclusive{ accessibleGoodsUninclusive };

		accessibleGoodsInclusive[good][0] += quantity;

		GoodPriceQuantityMap realAccessibleGoodsInclusive{ this->getRealAccessibleGoods(
			moneyWithSales, accessibleGoodsInclusive) };

		if ((willingness * quantity) >= moneyWithSales) {
			willingness = moneyWithSales / quantity;
			break;
		}

		moneyWithSales -= (willingness * quantity);

		GoodQuantityMap inclusiveHoldings{ this->utilityFunction.optimalQuantities(
			KnapsackSolver{}, moneyWithSales, realAccessibleGoodsInclusive) };

		double newUtility{ this->utilityFunction.getUtility(inclusiveHoldings) };
		if (desiredUtility > newUtility) {

			if (DEBUG_OUT == true) {

				std::string debugOut{ "Found our breaking point; found WTP." };
				Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

				debugOut = "moneyWithSales: " + toString(moneyWithSales);
				Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

				debugOut = "Uncorrected newUtility: " + toString(newUtility);
				Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

				debugOut = "Uncorrected inclusiveHoldings: [" + toString(inclusiveHoldings) + ']';
				Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

				debugOut = "Uncorrected willingness: " + toString(willingness);
				Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

				debugOut = "accessibleGoodsInclusive: [" + toString(accessibleGoodsInclusive) + ']';
				Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

				debugOut = "realAccessibleGoodsInclusive: [" +
					toString(realAccessibleGoodsInclusive) + ']';
				Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

			}

			break;

		}

		willingness += WTP_ADJUST;

	}

	if (willingness > WTP_ADJUST) {
		willingness -= WTP_ADJUST;
	}

	if (DEBUG_OUT == true) {

		std::string debugOut{ "Corrected willingness (total): " + toString(willingness) };
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

		GoodPriceQuantityMap accessibleGoodsInclusive{ accessibleGoodsUninclusive };
		accessibleGoodsInclusive[good][0] += quantity;
		GoodPriceQuantityMap realAccessibleGoodsInclusive{ this->getRealAccessibleGoods(
			moneyWithSales, accessibleGoodsInclusive) };
		moneyWithSales -= willingness;

		GoodQuantityMap inclusiveHoldings{ this->utilityFunction.optimalQuantities(
			KnapsackSolver{}, moneyWithSales, realAccessibleGoodsInclusive) };

		double newUtility{ this->utilityFunction.getUtility(inclusiveHoldings) };

		debugOut = "Corrected inclusiveHoldings: [" + toString(inclusiveHoldings) + ']';
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

		debugOut = "Corrected utility: " + toString(newUtility);
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

	}
	
	//willingness /= quantity;
	willing = (willingness > 0);

	if (DEBUG_OUT == true) {

		std::string debugOut{ "Final adjusted willingness per quantity: " + toString(willingness) };
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

		debugOut = "Final willing: " + toString(willing);
		Driver::getDriver()->logMessage(this, "WeirdWTP", "willingnessToPay", debugOut);

	}

}
