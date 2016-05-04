#ifndef DUMB_ADJUST_H
#define DUMB_ADJUST_H

#include "HeuristicPerson.h"

class DumbAdjust : public virtual HeuristicPerson {

public:
	DumbAdjust();
	DumbAdjust(const PersonBase& other);

private:
	virtual void adjustPrices();

};

#endif
