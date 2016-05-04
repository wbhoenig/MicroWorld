#include "SmartAdjust.h"
#include "Driver.h"
#include "ToString.h"
#include "Driver.h"

#include <map>
#include <exception>
#include <iostream>
#include <string>

SmartAdjust::SmartAdjust() : HeuristicPerson{} { }

SmartAdjust::SmartAdjust(const PersonBase& other) :
	HeuristicPerson{ other } { }

void SmartAdjust::adjustPrices() {

	for (auto& currentNeighborConnection : this->personTrades) {

		GoodPriceQuantityMap newGPQM;
		Pointer<PersonBase> currentNeighbor = currentNeighborConnection.first;

		for (const auto& currentTrade : currentNeighborConnection.second) {

			bool useDefault = false;
			const GOOD_ID_TYPE& goodName = currentTrade.first;
			Pointer<PersonBase> otherPerson{ currentNeighborConnection.first };

			unsigned short desiredQuantity{ 0 };

			auto goodIt{ this->currentSellOffers.find(otherPerson) };
			if (goodIt != this->currentSellOffers.end()) {

				auto pqIt{ goodIt->second.find(goodName) };
				if (pqIt != goodIt->second.end()) {

					const PriceQuantityMap& pqm{ pqIt->second };
					auto it{ pqm.begin() };

					if (it != pqm.end()) {

						const unsigned short& price{ it->first };
						const unsigned short& quantity{ it->second };

						desiredQuantity = quantity;

					}

				}

			}

			auto oldTradeIt{ this->oldSellOffers.find(currentNeighbor) };

			unsigned short newPrice{ 0 };
			bool willing{ true };
			otherPerson->willingnessToPay(goodName, desiredQuantity, this, willing, newPrice);
			if (willing == false) {
				newPrice = 0;
			}

			if (DEBUG_OUT == true) {

				std::string debugOut;
				debugOut += "Determined that person " + Driver::getDriver()->getPersonName(otherPerson);
				debugOut += " has willing of " + toString(willing) + " and willingness of ";
				debugOut += toString(newPrice) + " for good " + toString(goodName);
				debugOut += " at quantity " + toString(desiredQuantity);
				Driver::getDriver()->logMessage(this, "SmartAdjust", "adjustPrices", debugOut);

			}

			newGPQM[goodName][newPrice] = desiredQuantity;

			// Also update offer price/quantity?
			auto offerIt{ this->currentSellOffers.find(currentNeighbor) };
			if (offerIt != this->currentSellOffers.end()) {

				GoodPriceQuantityMap& gpqm{ offerIt->second };
				auto goodIt = gpqm.find(goodName);
				if (goodIt != gpqm.end()) {

					PriceQuantityMap& pqm{ goodIt->second };
					auto pqmIt = pqm.begin();
					if (pqmIt != pqm.end()) {

						unsigned short oldQuantity{ pqmIt->second };
						pqm.clear();
						pqm.emplace(newPrice, oldQuantity);

					}

				}

			}

		}

		currentNeighborConnection.second = newGPQM;

	}

}
