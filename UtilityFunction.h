#ifndef UTILITY_FUNCTION_H
#define UTILITY_FUNCTION_H

#include <map>
#include <vector>
#include "HelperStructs.h"

// Has to be Cobb-Douglas for now.
class UtilityFunction {

public:
	// Both constructors legal
	UtilityFunction();
	UtilityFunction(double multiple);

	// Setters
	void setMultiple(double multiple);
	void setPower(const GOOD_ID_TYPE& good, double power);

	double getMultiple() const;
	double getPower(const GOOD_ID_TYPE& good) const;
	bool hasPower(const GOOD_ID_TYPE& good) const;

	// Processors
	double getUtility(const GoodQuantityMap& goodToQuantity) const;
	
	// Gets us a quantity of each good that maximizes utility, given unlimited availability.
	GoodQuantityMap maxUtility(double budget, const GoodPowerMap& goodToPrice);

	// Utility of one more of a good
	double marginalUtility(GoodQuantityMap& quantities, const GOOD_ID_TYPE& good) const;

	bool operator==(const UtilityFunction& other) const;

private:
	double multiple;
	GoodPowerMap powers;

	// TEMPLATE STUFF
public:
	// Set the optimal quantities given prices and quantities available.
	template <typename Solver>
	GoodQuantityMap optimalQuantities(Solver s, unsigned short money, const GoodPriceQuantityMap& priceQuantities) const {
		return s(this, money, priceQuantities);
	}

};

#endif
