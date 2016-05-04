#ifndef SMART_RATION_H
#define SMART_RATION_H

#include "HeuristicPerson.h"

#include <map>

class SmartRation : public virtual HeuristicPerson {

public:
	SmartRation();
	SmartRation(const PersonBase& other);

protected:
	virtual void ration(unsigned short& desiredSellQuantity, const GOOD_ID_TYPE& goodName,
		const std::map<Pointer<PersonBase>, GoodPriceQuantityMap >&);

};

#endif
