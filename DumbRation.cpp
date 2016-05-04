#include "DumbRation.h"
#include "Driver.h"

DumbRation::DumbRation() : HeuristicPerson{} { }

DumbRation::DumbRation(const PersonBase& other) : HeuristicPerson{ other } { }

void DumbRation::ration(unsigned short& desiredSellQuantity, const GOOD_ID_TYPE& goodName,
	const std::map<Pointer<PersonBase>, GoodPriceQuantityMap >&) {

	unsigned short remainingDesiredSellQuantity{ desiredSellQuantity };

	while (remainingDesiredSellQuantity > 0) {

		for (auto currentNeighborConnection : this->personTrades) {

			Pointer<PersonBase> currentNeighbor = currentNeighborConnection.first;
			GoodPriceQuantityMap& currentConnection = currentNeighborConnection.second;

			// Skip if this person does not demand this good at all.
			if (currentNeighbor->demands(goodName) == false) {
				continue;
			}

			// Ration one to this neighbor.
			this->currentSellOffers[currentNeighbor][goodName][Driver::getDriver()->getDefaultPrice()] += 1;
			this->personTrades[currentNeighbor][goodName][Driver::getDriver()->getDefaultPrice()] = 0;

			remainingDesiredSellQuantity -= 1;

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

}
