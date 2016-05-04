#include "HeuristicPerson.h"
#include "Driver.h"
#include "KnapsackSolver.h"
#include "ToString.h"

#include <algorithm>
#include <random>
#include <iostream>

HeuristicPerson::HeuristicPerson() : PersonBase{}, oldSellOffers{} {

}

HeuristicPerson::HeuristicPerson(const PersonBase& other) : PersonBase{ other },
oldSellOffers{} {

}

HeuristicPerson::HeuristicPerson(const HeuristicPerson& other) : PersonBase{ other },
oldSellOffers{ other.oldSellOffers } { }

void HeuristicPerson::receiveNewOffers() {

	std::vector<GoodPriceQuantityMap> unmerged{};
	this->neighborsCurrentlySelling.clear();

	for (const auto& currentPersonConnection : this->personTrades) {

		const GoodPriceQuantityMap& currentPriceQuantities{
			currentPersonConnection.first->getGoodPriceQuantitiesSellingTo(Pointer<PersonBase>{this}) };
		unmerged.push_back(currentPriceQuantities);

		// Also need to fill in to what neighbors are selling, for future processing
		for (auto currentPriceQuantityMap : currentPriceQuantities) {

			std::vector<PriceQuantityPerson>& currentPQPs{ this->neighborsCurrentlySelling[currentPriceQuantityMap.first] };

			for (auto currentPriceQuantity : currentPriceQuantityMap.second) {

				currentPQPs.emplace_back(currentPriceQuantity.first, currentPriceQuantity.second, currentPersonConnection.first);

			}

		}

	}

	// Hold onto the goods that our neighbors are selling
	// For figuring out our own prices.
	this->neighborSellOffers = merge(unmerged);

	if (DEBUG_OUT == true) {

		std::string debugOut{ "neighborSellOffers: " };
		debugOut += toString(this->neighborSellOffers);
		Driver::getDriver()->logMessage(this, "HeuristicPerson", "receiveNewOffers", debugOut);

		debugOut = "neighborsCurrentlySelling: [" + toString(this->neighborsCurrentlySelling) + ']';
		Driver::getDriver()->logMessage(this, "HeuristicPerson", "receiveNewOffers", debugOut);

	}

	// Also add in our endowment
	unmerged.emplace_back();
	GoodPriceQuantityMap& lastUnplaced = unmerged.back();
	for (auto currentEndowedGood : this->goodEndowment) {
		lastUnplaced[currentEndowedGood.first][0] = currentEndowedGood.second;
	}

	this->accessibleGoods = merge(unmerged);

	if (DEBUG_OUT == true) {

		std::string debugOut{ "accessibleGoods: " };
		debugOut += toString(this->accessibleGoods);

		Driver::getDriver()->logMessage(this, "HeuristicPerson", "receiveNewOffers", debugOut);

	}

}

void HeuristicPerson::determineGoodPurchaseSources() {

	// Now we need to figure out where we can most cheaply purchase these goods.

	// But first, clear all present purchases.
	for (auto currentConnectedPerson : this->personTrades) {

		currentConnectedPerson.second.clear();

	}

	this->currentActualPurchases.clear();

	// Make each necessary purchase
	for (auto currentNecessaryPurchase : this->sketchNecessaryPurchases) {

		const GOOD_ID_TYPE& currentGood = currentNecessaryPurchase.first;
		const unsigned short& currentQuantity = currentNecessaryPurchase.second;
		unsigned short remainingQuantity = currentQuantity;

		std::vector<PriceQuantityPerson>& currentPQPs = neighborsCurrentlySelling[currentGood];

		// Sort by price!
		std::sort(currentPQPs.begin(), currentPQPs.end(), PriceQuantityPersonCompare{});

		for (PriceQuantityPerson& currentPQP : currentPQPs) {

			// Nothing left for us to need to fill.
			if (remainingQuantity < 1) {
				break;
			}

			// Add the purchase in
			unsigned short purchaseQuantity = std::min(remainingQuantity, currentPQP.quantity);

			// Schedule the purchase event
			Pointer<PersonBase> person = currentPQP.person;

			// Log the buy
			this->currentActualPurchases[person][currentGood] += purchaseQuantity;

			// Do the buy
			person->buy(Pointer<PersonBase>{this}, currentGood, purchaseQuantity);
			if (DEBUG_OUT == true) {

				std::string debugOut{ "Buying " };
				debugOut += toString(purchaseQuantity) + " of " + toString(currentGood);
				debugOut += " from person " + toString(Driver::getDriver()->getPersonName(person));

				Driver::getDriver()->logMessage(this, "HeuristicPerson", "determineGoodPurchaseSources", debugOut);

			}

			// Reduce the number we need to buy!
			remainingQuantity -= purchaseQuantity;

		}

		if (remainingQuantity > 0) {
			throw std::exception{ "Not enough purchasable quantity." };
		}

	}

}

void HeuristicPerson::calculateNewSellOffers() {

	// Clear what we have now, as we're throwing it out.
	//std::map<Pointer<PersonBase>, GoodPriceQuantityMap > oldSellOffers{ this->currentSellOffers };
	this->oldSellOffers.clear();
	this->oldSellOffers = this->currentSellOffers;
	this->currentSellOffers.clear();

	// Temporarily hold desired sales for future comparison
	GoodQuantityMap tempDesiredSales{ this->sketchDesiredSales };

	if (DEBUG_OUT == true) {

		std::string debugOut{ "sketchDesiredSales at the start of calculateNewSellOffers: " };
		debugOut += toString(this->sketchDesiredSales);
		Driver::getDriver()->logMessage(this, "HeuristicPerson", "calculateNewSellOffers", debugOut);

	}

	// Update what we want to sell.
	for (auto& currentDesiredSellGood : this->sketchDesiredSales) {

		const GOOD_ID_TYPE& goodName = currentDesiredSellGood.first;
		unsigned short& totalDesiredSellQuantity = currentDesiredSellGood.second;

		// If we don't have any, then don't sell it.
		auto endowmentIt = this->goodEndowment.find(goodName);
		if ((endowmentIt == this->goodEndowment.end()) || (endowmentIt->second == 0)) {
			continue;
		}

		unsigned short minSell = 0;
		for (Pointer<PersonBase> person : slavePropogatedConnectedPersons) {

			if (person->demands(goodName) == true) {
				++minSell;
			}

		}
		minSell = std::min(minSell, endowmentIt->second);

		// If we're not even selling the minimum, then do that.
		if (minSell > totalDesiredSellQuantity) {
			totalDesiredSellQuantity = minSell;
			continue;
		}

		// Total up what we're currently broadcasting.
		unsigned short totalGoodBroadcastSellQuantity = PersonBase::getSellQuantity(goodName, oldSellOffers);

		// If we're shrinking, let it shrink.
		if (totalGoodBroadcastSellQuantity > totalDesiredSellQuantity) {
			continue;
		}

		// Expand a bit if all is going well
		if (totalGoodBroadcastSellQuantity == totalDesiredSellQuantity) {
			++totalDesiredSellQuantity;
			continue;
		}

		// And what we're currently actually buying.
		unsigned short totalActualSellQuantity = PersonBase::getSellQuantity(goodName, this->personTrades);

		// Also shrink if we can't even sell what we're trying to.
		if (totalActualSellQuantity < (totalDesiredSellQuantity - 1)) {
			--totalDesiredSellQuantity;
			continue;
		}

	}

	if (DEBUG_OUT == true) {

		std::string debugOut{ "sketchDesiredSales in calculateNewSellOffers after first for loop: " };
		debugOut += toString(this->sketchDesiredSales);
		Driver::getDriver()->logMessage(this, "HeuristicPerson", "calculateNewSellOffers", debugOut);

	}

	// And now update to whom we want to sell it.
	for (auto& currentDesiredSellGood : this->sketchDesiredSales) {

		const GOOD_ID_TYPE& goodName{ currentDesiredSellGood.first };
		unsigned short& totalDesiredSellQuantity{ currentDesiredSellGood.second };

		if (DEBUG_OUT == true) {

			std::string debugOut{ "totalDesiredSellQuantity: " + toString(totalDesiredSellQuantity) };
			debugOut += " for good: " + toString(goodName);

			Driver::getDriver()->logMessage(this, "HeuristicPerson", "calculateNewSellOffers", debugOut);

		}

		// Go through each potential buyer and ration out one good
		// until there is nothing left to be rationed.
		this->ration(totalDesiredSellQuantity, goodName, oldSellOffers);

		// Now update our holdings.
		const unsigned short& previousSketchDesiredSales{ tempDesiredSales[goodName] };
		const unsigned short& newSketchDesiredSales{ this->sketchDesiredSales[goodName] };
		signed long desiredSalesDelta{ newSketchDesiredSales - previousSketchDesiredSales };
		signed long holdingsDelta{ -desiredSalesDelta };
		this->currentHoldings[goodName] = static_cast<unsigned short>(this->currentHoldings[goodName] + holdingsDelta);

	}

	this->adjustPrices();

	// We are in equilibrium if this didn't change.
	if (this->currentSellOffers == oldSellOffers) {
		this->equilibrium = true;
	}

	if (DEBUG_OUT == true) {

		std::string debugOut{ "currentHoldings after calculateNewSellOffers: " };
		debugOut += toString(this->currentHoldings);
		Driver::getDriver()->logMessage(this, "HeuristicPerson", "calculateNewSellOffers", debugOut);

		debugOut = "currentSellOffers after calculateNewSellOffers: ";
		debugOut += toString(this->currentSellOffers);
		Driver::getDriver()->logMessage(this, "HeuristicPerson", "calculateNewSellOffers", debugOut);

	}

}

GoodPriceQuantityMap HeuristicPerson::getRealAccessibleGoods(unsigned short& moneyWithSales,
	const GoodPriceQuantityMap& sellOffers) const {

	moneyWithSales = this->moneyEndowment;

	// What we can buy costs the sale price to buy.
	GoodPriceQuantityMap aggregateSellOffers;
	for (const auto& currentNeighborSellOffer : sellOffers) {

		const GOOD_ID_TYPE& goodName = currentNeighborSellOffer.first;
		const PriceQuantityMap& currentPQM = currentNeighborSellOffer.second;
		for (auto currentPQ : currentPQM) {
			aggregateSellOffers[goodName][currentPQ.first] += currentPQ.second;
		}

	}

	// What we can sell costs the sale price to hold.
	//GoodPriceQuantityMap goodsFromEndowment{ this->neighborsCurrentlyPurchasing };
	GoodPriceQuantityMap goodsFromEndowment;
	for (const auto& currentOffer : this->currentSellOffers) {

		const GoodPriceQuantityMap& currentGPQM = currentOffer.second;
		for (const auto& currentGoodTrade : currentGPQM) {

			const GOOD_ID_TYPE& goodName = currentGoodTrade.first;
			const PriceQuantityMap& currentPQM = currentGoodTrade.second;

			for (const auto& currentGoodPriceTrade : currentPQM) {

				const unsigned short& currentPrice = currentGoodPriceTrade.first;
				const unsigned short& currentQuantity = currentGoodPriceTrade.second;
				goodsFromEndowment[goodName][currentPrice] += currentQuantity;

			}

		}

	}

	// Add in the rest of the endowment, for free since we're not selling it presently.
	for (const auto& currentGoodInEndowment : this->goodEndowment) {

		unsigned short quantityToAdd = currentGoodInEndowment.second;

		// Subtract out what is being sold.
		const GOOD_ID_TYPE& goodName = currentGoodInEndowment.first;
		auto currentSoldGoodIt = goodsFromEndowment.find(goodName);

		// (Only subtract if something is actually sold.)
		if (currentSoldGoodIt != goodsFromEndowment.end()) {

			const PriceQuantityMap& currentSoldPQM = currentSoldGoodIt->second;
			for (auto currentPQ : currentSoldPQM) {

				const unsigned short& price = currentPQ.first;
				const unsigned short& quantity = currentPQ.second;

				// Also need to figure out our updated money figure.
				moneyWithSales += (price * quantity);

				// Die if we can't subtract this many.
				if (quantity > quantityToAdd) {
					throw std::exception{ "Trying to subtract more from sales than is available to sell." };
				}
				quantityToAdd -= quantity;

			}

		}

		// Now actually add this quantity in.
		goodsFromEndowment[goodName][0] += quantityToAdd;

	}

	return merge(std::vector<GoodPriceQuantityMap>{goodsFromEndowment, aggregateSellOffers });

}
