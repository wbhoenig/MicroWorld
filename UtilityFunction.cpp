#include "UtilityFunction.h"
#include "HelperStructs.h"
#include "KnapsackSolver.h"

#include <vector>

UtilityFunction::UtilityFunction() : multiple{ 0 }, powers{} {

}

UtilityFunction::UtilityFunction(double multiple) : multiple{ multiple }, powers{}  {

}

void UtilityFunction::setMultiple(double multiple) {
	this->multiple = multiple;
}

void UtilityFunction::setPower(const GOOD_ID_TYPE& good, double power) {
	this->powers[good] = power;
}

double UtilityFunction::getUtility(const GoodQuantityMap& goodToQuantity) const {

	double out = 0.0;

	for (auto current : goodToQuantity) {

		auto currentPowerIt = this->powers.find(current.first);
		// Check to see if we have a power on this good.
		if (currentPowerIt == this->powers.end()) {
			continue;
		}

		// Since we do, apply it.
		out += pow(current.second, currentPowerIt->second);

	}

	// Finally, apply our multiple.
	out *= this->multiple;

	return out;

}

GoodQuantityMap UtilityFunction::maxUtility(double budget, const GoodPowerMap& goodToPrice) {

	GoodQuantityMap map;

	for (auto current : goodToPrice) {

		double quantity = 0;
		auto currentPowerIt = this->powers.find(current.first);

		// We actually have a power for this. So apply it.
		if (currentPowerIt != this->powers.end()) {

			quantity = (currentPowerIt->second / current.second) * budget;

		}
		// And now save that power.
		//current.second = quantity;
		map[current.first] = static_cast<unsigned short>(quantity);

	}

	return map;

}

double UtilityFunction::marginalUtility(GoodQuantityMap& quantities, const GOOD_ID_TYPE& good) const {
	
	// Just utility with one more of "good"
	// minus current utility. (Approximately. No calculus here.)
	double currentUtility{ this->getUtility(quantities) };
	//GoodQuantityMap adjustedQuantities{ quantities };

	// Update the quantities
	auto quantityIt = quantities.find(good);
	if (quantityIt == quantities.end()) {
		quantityIt = quantities.emplace(good, 0).first;
	}
	quantityIt->second += 1;

	// Get the new utility and calculate the delta.
	double adjustedUtility = this->getUtility(quantities);
	quantityIt->second -= 1;
	return (adjustedUtility - currentUtility);

}

double UtilityFunction::getMultiple() const {
	return this->multiple;
}

double UtilityFunction::getPower(const GOOD_ID_TYPE& good) const {

	auto it = this->powers.find(good);
	if (it == this->powers.end()) {
		return 0;
	}

	return it->second;

}

bool UtilityFunction::hasPower(const GOOD_ID_TYPE& good) const {

	auto it = this->powers.find(good);
	return (it != this->powers.end());

}

bool UtilityFunction::operator==(const UtilityFunction& other) const {
	
	bool out = true;

	out &= (this->multiple == other.multiple);
	out &= (this->powers == other.powers);

	return out;

}
