#include "MarginalGoodCell.h"

// Constructors

// Empty cell for holding nothing
// Pretty damn useless
MarginalGoodCell::MarginalGoodCell() : utility{ 0 }, deltaHoldings{ }, previousCell { nullptr } {

}

MarginalGoodCell::MarginalGoodCell(const MarginalGoodCell* notInclude, const MarginalGoodCell* include,
	const GoodQuantityMap& deltaHoldings, double deltaUtility) :
	utility{ 0 }, deltaHoldings{}, previousCell{ nullptr } {

	// Need some sort of previous utility for each.
	double notIncludeUtility{ 0 };
	// Start this at what we'd be adding
	double includeUtility{ deltaUtility };

	if (notInclude != nullptr) {
		notIncludeUtility = notInclude->getUtility();
	}

	if (include != nullptr) {
		// Simply add what we'd be getting.
		includeUtility += include->getUtility();
	}

	// Only include if actively better to do so.
	if (includeUtility > notIncludeUtility) {

		this->utility = includeUtility;
		this->deltaHoldings = deltaHoldings;
		this->previousCell = include;

	}
	else {

		this->utility = notIncludeUtility;
		this->deltaHoldings.clear();
		this->previousCell = notInclude;

	}

	this->initTotalHoldings();

}

// Getters
const double& MarginalGoodCell::getUtility() const {
	return this->utility;
}

const GoodQuantityMap& MarginalGoodCell::getTotalHoldings() const {
	return this->totalHoldings;
}

GoodQuantityMap& MarginalGoodCell::getTotalHoldings() {
	return this->totalHoldings;
}

void MarginalGoodCell::initTotalHoldings() {

	const MarginalGoodCell* source{ this->previousCell };
	this->totalHoldings = deltaHoldings;
	if (source != nullptr) {
		this->updateDeltaHoldingsOut(this->totalHoldings, source->totalHoldings);
	}

}

/*const GoodQuantityMap& MarginalGoodCell::getTotalHoldings() const {

	const MarginalGoodCell* nextSource{ this->previousCell };
	while (nextSource != nullptr) {

		this->updateDeltaHoldingsOut(this->totalHoldings, nextSource->deltaHoldings);
		nextSource = nextSource->previousCell;

	}

}*/

// And the static helper
void MarginalGoodCell::updateDeltaHoldingsOut(GoodQuantityMap& out, const GoodQuantityMap& in) {

	// Go through each good we're adding
	for (const auto& current : in) {

		const GOOD_ID_TYPE& goodName{ current.first };
		const unsigned short& quantity{ current.second };

		auto outIt = out.find(goodName);

		// Insert a new one if necessary
		if (outIt == out.end()) {
			out.emplace(goodName, quantity);
			continue;
		}

		// If not, then just update.
		outIt->second += quantity;

	}

}
