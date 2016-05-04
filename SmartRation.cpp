#include "SmartRation.h"
#include "Driver.h"
#include "Parameters.h"
#include "ToString.h"

#include <algorithm>
#include <string>

SmartRation::SmartRation() : HeuristicPerson{} { }

SmartRation::SmartRation(const PersonBase& other) : HeuristicPerson{ other } { }

void SmartRation::ration(unsigned short& desiredSellQuantity, const GOOD_ID_TYPE& goodName,
	const std::map<Pointer<PersonBase>, GoodPriceQuantityMap >&) {

	// This is a fairly silly algorithm that hands out the goods
	// based on the current price that the good is selling at to a customer.
	std::map<Pointer<PersonBase>, unsigned short> allocations{};
	unsigned long totalPrices{ 0 };

	if (DEBUG_OUT == true) {

		std::string debugOut{ "Rationing out " + toString(desiredSellQuantity) };
		debugOut += " of good: " + toString(goodName);

		Driver::getDriver()->logMessage(this, "SmartRation", "ration", debugOut);

	}

	// Search for different prices
	// First, unscaled versions of the factor...
	for (const auto& currentTrade : this->oldSellOffers) {

		Pointer<PersonBase> person = currentTrade.first;
		const GoodPriceQuantityMap& gpqm{ currentTrade.second };
		auto it = gpqm.find(goodName);
		if (it == gpqm.end()) {
			continue;
		}

		const PriceQuantityMap& pqm = it->second;
		auto pqIt = pqm.begin();
		if (pqIt == pqm.end()) {
			continue;
		}

		const unsigned short& price = pqIt->first;
		const unsigned short& quantity = pqIt->second;

		totalPrices += price;
		allocations.emplace(person, price);

	}

	if (DEBUG_OUT == true) {

		std::string debugOut{ "allocations after first for loop: " + toString(allocations) };
		Driver::getDriver()->logMessage(this, "SmartRation", "ration", debugOut);

	}

	// Now, scale down the amount to allocate.
	for (auto& currentAllocation : allocations) {

		unsigned short& allocatedAmount{ currentAllocation.second };
		unsigned short oldAllocatedAmount_DEBUG{ allocatedAmount };
		double factor{ static_cast<double>(allocatedAmount) / static_cast<double>(totalPrices) };
		double newAllocation{ factor * desiredSellQuantity };

		allocatedAmount = std::max(unsigned short{ 1 },
			static_cast<unsigned short>(newAllocation));

		if (DEBUG_OUT == true) {

			std::string debugOut{"oldAllocatedAmount: " + toString(oldAllocatedAmount_DEBUG) };
			debugOut += " factor: " + toString(factor) + " newAllocation: " + toString(newAllocation);
			debugOut += " allocatedAmount: " + toString(allocatedAmount);
			debugOut += " for good: " + toString(goodName);

			Driver::getDriver()->logMessage(this, "SmartRation", "ration", debugOut);

		}

	}

	unsigned short remainingDesiredSellQuantity{ desiredSellQuantity };

	if (DEBUG_OUT == true) {

		std::string debugOut{ "remainingDesiredSellQuantity: " + toString(remainingDesiredSellQuantity) };
		debugOut += " desiredSellQuantity: " + toString(desiredSellQuantity);
		debugOut += " for good: " + toString(goodName);

		Driver::getDriver()->logMessage(this, "SmartRation", "ration", debugOut);

	}

	while (remainingDesiredSellQuantity > 0) {

		for (auto currentNeighborConnection : this->personTrades) {

			Pointer<PersonBase> currentNeighbor = currentNeighborConnection.first;
			GoodPriceQuantityMap& currentConnection = currentNeighborConnection.second;

			// Skip if this person does not demand this good at all.
			if (currentNeighbor->demands(goodName) == false) {
				continue;
			}

			unsigned short allocationAmount{ 1 };
			auto allocationIt = allocations.find(currentNeighbor);
			if (allocationIt != allocations.end()) {

				// And make sure we actually have this amount to give away
				allocationAmount = std::min(allocationIt->second, remainingDesiredSellQuantity);

			}


			// Ration the correct amount to this neighbor.
			this->currentSellOffers[currentNeighbor][goodName]
				[Driver::getDriver()->getDefaultPrice()] += allocationAmount;
			this->personTrades[currentNeighbor][goodName][Driver::getDriver()->getDefaultPrice()] = 0;

			remainingDesiredSellQuantity -= allocationAmount;

			if (remainingDesiredSellQuantity < 1) {
				break;
			}

		}

		// We also need to check if we're making no progress whatsoever
		// And, if that is true, drop the sell quantity down to 0.
		if (remainingDesiredSellQuantity == desiredSellQuantity) {

			desiredSellQuantity = 0;
			break;

		}

	}

	if (DEBUG_OUT == true) {

		std::string debugOut{ "currentSellOffers after ration: " + toString(this->currentSellOffers) };
		Driver::getDriver()->logMessage(this, "SmartRation", "ration", debugOut);

	}

}
