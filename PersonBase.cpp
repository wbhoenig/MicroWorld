#include "PersonBase.h"
#include "Driver.h"
#include "Parameters.h"
#include "HelperStructs.h"
#include "KnapsackSolver.h"
#include "MarginalGoodCell.h"
#include "ToString.h"

#include <vector>
#include <list>
#include <utility>
#include <exception>
#include <string>

void PersonBase::update() {

	if (DEBUG_OUT == true) {

		Driver::getDriver()->logMessage(this, "", "", "");
		Driver::getDriver()->logMessage(this, "PersonBase", "update",
			"Beginning update cycle for person " + this->myName());

	}

	// Skip if already done
	if (this->lastUpdateTime == Driver::getDriver()->getTime()) {
		if (DEBUG_OUT == true) {

			std::string debugOut{ "Ending update cycle for person " + this->myName() };

			debugOut += " REASON: already updated in this update time.";

			Driver::getDriver()->logMessage(this, "PersonBase", "update", debugOut);
			Driver::getDriver()->logMessage(this, "", "", "");

		}

		return;
	}

	this->equilibrium = false;

	// All of the steps
	this->receiveNewOffers();
	this->determineOptimalQuantities();

	// Okay, so we know what we want to hold. Now determine where we get it from.
	this->determineGoodPurchaseSources();
	
	this->calculateNewSellOffers();

	// And make sure this doesn't happen again.
	this->lastUpdateTime = Driver::getDriver()->getTime();

	if (this->equilibrium == false) {

		if (DEBUG_OUT == true) {
			Driver::getDriver()->logMessage(this, "PersonBase", "update", "Updating neighbors.");
		}
		this->updateNeighbors();
	}

	if (DEBUG_OUT == true) {
		
		std::string debugOut{ "Ending update cycle for person " + this->myName() };
		debugOut += " REASON: normal.";
		Driver::getDriver()->logMessage(this, "PersonBase", "update", debugOut);

		debugOut = "Exiting utility: " + toString(this->getUtility());
		Driver::getDriver()->logMessage(this, "PersonBase", "update", debugOut);

		Driver::getDriver()->logMessage(this, "", "", "");

	}

	if (RESULTS_OUT == true) {

		std::string holdingString{ toString(this->currentHoldings) };
		std::string buyString{ toString(this->currentActualPurchases) };
		std::string sellString{ toString(this->currentSellOffers) };

		Driver::getDriver()->logResults(this, holdingString, buyString, sellString);

	}

}

const GoodPriceQuantityMap& PersonBase::getGoodPriceQuantitiesSellingTo(Pointer<PersonBase> other) {
	return this->currentSellOffers[other];
}

double PersonBase::getUtility() const {
	return this->utilityFunction.getUtility(this->currentHoldings);
}

PersonBase::PersonBase() : utilityFunction{}, neighborsCurrentlySelling{}, goodEndowment{},
currentHoldings{}, currentSellOffers{}, neighborSellOffers{}, accessibleGoods{},
sketchNecessaryPurchases{}, sketchDesiredSales{}, equilibrium{ false },
currentActualPurchases{}, Agent {} { }

PersonBase::PersonBase(const PersonBase& other) : utilityFunction{ other.utilityFunction },
neighborsCurrentlySelling{ other.neighborsCurrentlySelling }, goodEndowment{ other.goodEndowment },
currentHoldings{ other.currentHoldings }, currentSellOffers{ other.currentSellOffers },
neighborSellOffers{ other.neighborSellOffers }, accessibleGoods{ other.accessibleGoods },
sketchNecessaryPurchases{ other.sketchNecessaryPurchases }, sketchDesiredSales{ other.sketchDesiredSales },
equilibrium{ other.equilibrium }, currentActualPurchases{ other.currentActualPurchases },
Agent{ other } { }

void PersonBase::buy(Pointer<PersonBase> buyer, const GOOD_ID_TYPE& good, unsigned short quantity) {

	// Update the purchase record with this buyer, and potentially re-run.
	// May not be buying at all right now
	GoodPriceQuantityMap& connectedPersonGoods = this->personTrades[buyer];
	PriceQuantityMap& priceQuantities = connectedPersonGoods[good];
	auto quantityIt = priceQuantities.begin();
	if (quantityIt->second != quantity) {
		quantityIt->second = quantity;
		Pointer<PersonBase> thisPtr{ this };
		Driver::getDriver()->scheduleEvent([thisPtr] {
			thisPtr->update();
		});
	}

}

void PersonBase::endow(const GOOD_ID_TYPE& good, unsigned short quantity) {

	this->goodEndowment[good] = quantity;
	//this->initializeSellOffers();

}

void PersonBase::connect(Pointer<PersonBase> other) {

	this->personTrades[other] = GoodPriceQuantityMap{};
	this->directlyConnectedPersons.insert(other);
	this->slavePropogatedConnectedPersons = this->directlyConnectedPersons;

}

void PersonBase::setUtilityMultiple(double multiple) {
	this->utilityFunction.setMultiple(multiple);
}

void PersonBase::setUtilityPower(const GOOD_ID_TYPE& good, double power) {
	this->utilityFunction.setPower(good, power);
}

void PersonBase::initializeSellOffers() {
	this->currentSellOffers.clear();
	// Just sell our goods at 100.
	/*for (auto currentEndowedGood : this->goodEndowment) {

		this->currentSellOffers[currentEndowedGood.first][DEFAULT_PRICE] = currentEndowedGood.second;

	}*/

	for (auto currentEndowedGood : this->goodEndowment) {

		unsigned short sellQuantity = currentEndowedGood.second / 2;
		unsigned short demandingNeighbors = 0;

		// Count number of neighbors who demand it.
		for (auto person : this->slavePropogatedConnectedPersons) {

			if (person->demands(currentEndowedGood.first) == true) {
				++demandingNeighbors;
			}

		}

		unsigned short perPerson = sellQuantity / demandingNeighbors;
		unsigned short extra = sellQuantity - (perPerson * demandingNeighbors);

	}
}

bool PersonBase::demands(const GOOD_ID_TYPE& good) const {

	return (this->utilityFunction.getPower(good) != 0.0);

}

unsigned short PersonBase::getSellQuantity(const GOOD_ID_TYPE& good, std::map<Pointer<PersonBase>, GoodPriceQuantityMap>& map) {
	
	unsigned short out = 0;
	for (auto currentPerson : map) {

		Pointer<PersonBase> other = currentPerson.first;
		const GoodPriceQuantityMap& personGPQM = currentPerson.second;

		auto currentPQMIt = personGPQM.find(good);
		if (currentPQMIt == personGPQM.end()) { continue; }
		const PriceQuantityMap& currentPQM = currentPQMIt->second;

		for (auto currentPQ : currentPQM) {

			const unsigned short& price = currentPQ.first;
			const unsigned short& quantity = currentPQ.second;

			out += quantity;

		}

	}

	return out;

}

std::string PersonBase::holdingsString() const {

	std::string str{""};
	const std::string& myName = Driver::getDriver()->getPersonName(Pointer<const PersonBase>{this});
	str += myName + HOLDINGS_DELIMITER;

	for (const auto& holding : this->currentHoldings) {

		// Holding
		const GOOD_ID_TYPE& goodName{ holding.first };
		str += ToString<GOOD_ID_TYPE>{}(goodName)+HOLDINGS_DELIMITER + 
			ToString<unsigned short>{}(holding.second) + HOLDINGS_DELIMITER;

		// Selling
		for (const auto& currentSellOffer : this->currentSellOffers) {

			const std::string& otherName = Driver::getDriver()->getPersonName(currentSellOffer.first);
			auto currentPQMIt = currentSellOffer.second.find(goodName);
			if (currentPQMIt == currentSellOffer.second.end()) {
				continue;
			}
			const PriceQuantityMap& currentPQM = currentPQMIt->second;
			auto currentPQIt = currentPQM.begin();
			if (currentPQIt == currentPQM.end()) {
				continue;
			}
			const unsigned short& currentPrice = currentPQIt->first;
			const unsigned short& currentQuantity = currentPQIt->second;

			str += otherName + HOLDINGS_DELIMITER + std::to_string(currentPrice);
			str += HOLDINGS_DELIMITER + std::to_string(currentQuantity) + HOLDINGS_DELIMITER;

			str += HOLDINGS_DELIMITER;

		}

		str += HOLDINGS_DELIMITER;

	}

	str += HOLDINGS_DELIMITER;

	return str;

}

const GoodQuantityMap& PersonBase::getCurrentHoldings() const {
	return this->currentHoldings;
}

bool PersonBase::operator==(const PersonBase& other) const {

	bool out = true;

	const Agent& demotedThis{ static_cast<const Agent&>(*this) };
	out &= (demotedThis == other);

	out &= (this->utilityFunction == other.utilityFunction);
	out &= (this->neighborsCurrentlySelling == other.neighborsCurrentlySelling);
	out &= (this->goodEndowment == other.goodEndowment);
	out &= (this->currentHoldings == other.currentHoldings);
	out &= (this->currentSellOffers == other.currentSellOffers);
	out &= (this->neighborSellOffers == other.neighborSellOffers);
	out &= (this->accessibleGoods == other.accessibleGoods);
	out &= (this->sketchNecessaryPurchases == other.sketchNecessaryPurchases);
	out &= (this->sketchDesiredSales == other.sketchDesiredSales);
	out &= (this->equilibrium == other.equilibrium);

	return out;

}

void PersonBase::clear() {

	this->__clear();

	this->neighborsCurrentlySelling.clear();
	this->currentHoldings.clear();
	this->currentSellOffers.clear();
	this->neighborSellOffers.clear();
	this->accessibleGoods.clear();
	this->sketchNecessaryPurchases.clear();
	this->sketchDesiredSales.clear();
	this->equilibrium = false;


}

std::string PersonBase::myName() const {

	return Driver::getDriver()->getPersonName(this);

}

