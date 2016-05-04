#ifndef HEURISTIC_PERSON_H
#define HEURISTIC_PERSON_H

#include "PersonBase.h"

class HeuristicPerson : public virtual PersonBase {

	// Constructors
public:
	// Constructors
	HeuristicPerson();
	HeuristicPerson(const PersonBase& other);
	HeuristicPerson(const HeuristicPerson& other);

	virtual void willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
		Pointer<PersonBase> other, bool& willing, unsigned short& willingness) = 0;

protected:

	std::map<Pointer<PersonBase>, GoodPriceQuantityMap > oldSellOffers;

	void receiveNewOffers();
	virtual void determineOptimalQuantities() = 0;
	void determineGoodPurchaseSources();
	void calculateNewSellOffers();

	GoodPriceQuantityMap getRealAccessibleGoods(unsigned short& moneyWithSales,
		const GoodPriceQuantityMap& sellOffers) const;

	virtual void ration(unsigned short& desiredSellQuantity, const GOOD_ID_TYPE& goodName,
		const std::map<Pointer<PersonBase>, GoodPriceQuantityMap >&) = 0;

	virtual void adjustPrices() = 0;

};

#endif
