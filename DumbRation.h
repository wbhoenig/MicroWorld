#ifndef DUMB_RATION_H
#define DUMB_RATION_H

#include "HeuristicPerson.h"

#include <map>

class DumbRation : public virtual HeuristicPerson {

public:
	DumbRation();
	DumbRation(const PersonBase& other);

protected:
	virtual void ration(unsigned short& desiredSellQuantity, const GOOD_ID_TYPE& goodName,
		const std::map<Pointer<PersonBase>, GoodPriceQuantityMap >&);

};

#endif
