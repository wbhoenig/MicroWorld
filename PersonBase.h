#ifndef PERSON_BASE_H
#define PERSON_BASE_H

#include "Agent.h"
#include "UtilityFunction.h"
#include "GoodInfoHistory.h"

#include <map>
#include <string>
#include <vector>
#include <unordered_set>

class PersonBase : public virtual Agent {

public:
	const GoodPriceQuantityMap& getGoodPriceQuantitiesSellingTo(Pointer<PersonBase> other);
	void buy(Pointer<PersonBase> buyer, const GOOD_ID_TYPE& good, unsigned short quantity);

	double getUtility() const;
	//unsigned short wouldPay(const std::string& good, unsigned short quantity) const;
	bool demands(const GOOD_ID_TYPE& good) const;

	// Constructors
	PersonBase();
	PersonBase(const PersonBase& other);

	void endow(const GOOD_ID_TYPE& good, unsigned short quantity);
	void connect(Pointer<PersonBase> other);

	void setUtilityMultiple(double multiple);
	void setUtilityPower(const GOOD_ID_TYPE& good, double power);

	std::string holdingsString() const;

	bool operator==(const PersonBase& other) const;

	const GoodQuantityMap& getCurrentHoldings() const;

	virtual void willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
		Pointer<PersonBase> other, bool& willing, unsigned short& willingness) = 0;

	// Recalculate everything
	void update();

	// Clear everything out
	void clear();

	std::string myName() const;

protected:
	// Distinguishing factor: Persons have utility functions.
	UtilityFunction utilityFunction;

	// What neighbors are currently offering to sell to us
	GoodPriceQuanityPersonMap neighborsCurrentlySelling;

	// Let's endow People with Goods
	// Has both our quantities of goods and the prices at which we are willing to sell them
	GoodQuantityMap goodEndowment;

	// Everything that we currently have, for utility.
	GoodQuantityMap currentHoldings;

	PersonGoodQuantityMap currentActualPurchases;

	// And what we're selling
	std::map<Pointer<PersonBase>, GoodPriceQuantityMap > currentSellOffers;

	// Structs needed in optimization calculation
	GoodPriceQuantityMap neighborSellOffers;
	GoodPriceQuantityMap accessibleGoods;
	//GoodQuantityMap optimalHoldingQuantities;
	//GoodPriceQuantityMap potentialEconomicalSellGoods;
	GoodQuantityMap sketchNecessaryPurchases;
	GoodQuantityMap sketchDesiredSales;

	// Marker for it we are in an equilibrium.
	bool equilibrium;

	// All of the helper functions for update. The blank deck, if you will.
	// To be filled in by various instantiations of Person.
	virtual void receiveNewOffers() = 0;
	virtual void determineOptimalQuantities() = 0;
	virtual void determineGoodPurchaseSources() = 0;
	virtual void calculateNewSellOffers() = 0;

	// Initializers
	void initializeSellOffers();

	// Helper functions
	static unsigned short getSellQuantity(const GOOD_ID_TYPE& good, std::map<Pointer<PersonBase>, GoodPriceQuantityMap>& map);
	
};

#endif
