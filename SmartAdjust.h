#ifndef SMART_ADJUST_H
#define SMART_ADJUST_H

#include "HeuristicPerson.h"
#include "PersonBase.h"

class SmartAdjust : public virtual HeuristicPerson {

public:
	SmartAdjust();
	SmartAdjust(const PersonBase& other);

protected:
	virtual void adjustPrices();

};

#endif
