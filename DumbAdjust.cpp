#include "DumbAdjust.h"
#include "PersonBase.h"
#include "Driver.h"

DumbAdjust::DumbAdjust() : HeuristicPerson{} { }

DumbAdjust::DumbAdjust(const PersonBase& other) :
	HeuristicPerson{ other } { }  

void DumbAdjust::adjustPrices() {

	for (auto& currentNeighborConnection : this->personTrades) {

		GoodPriceQuantityMap newGPQM;
		Pointer<PersonBase> currentNeighbor = currentNeighborConnection.first;

		for (const auto& currentTrade : currentNeighborConnection.second) {

			bool useDefault = false;
			const GOOD_ID_TYPE& goodName = currentTrade.first;

			const unsigned short& desiredQuantity = currentTrade.second.begin()->second;
			auto oldTradeIt{ this->oldSellOffers.find(currentNeighbor) };
			
			unsigned short newPrice{ Driver::getDriver()->getDefaultPrice() };

			// Determine if we have a legal old price to go off of or not
			if (oldTradeIt == this->oldSellOffers.end()) {
				useDefault = true;
			}
			else {

				auto oldGoodIt = oldTradeIt->second.find(goodName);
				if (oldGoodIt == oldTradeIt->second.end()) {
					useDefault = true;
				}
				else {

					auto oldPriceIt = oldGoodIt->second.begin();
					if (oldPriceIt == oldGoodIt->second.end()) {
						useDefault = true;
					}
					else {
						newPrice = oldPriceIt->first;
					}

				}

			}

			// Look at changing the price if we have a legal old price to go off of.
			if (useDefault == false) {

				unsigned short oldActualSellQuantity = PersonBase::getSellQuantity(goodName, this->personTrades);
				if (oldActualSellQuantity >= desiredQuantity) {
					++newPrice;
				}
				else if (oldActualSellQuantity < desiredQuantity) {
					--newPrice;
				}

			}

			// Let's chart our actual prices here.
			if (Driver::getDriver()->getPersonName(this) == "1") {

				Pointer<const PersonBase> target{ currentNeighbor };
				const std::string& targetName{ Driver::getDriver()->getPersonName(target) };
				//Driver::getDriver()->deposit(targetName, newPrice);

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
