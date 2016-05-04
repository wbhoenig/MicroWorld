#ifndef DUMB_DETERMINE_OPTIMAL_QUANTITIES_H
#define DUMB_DETERMINE_OPTIMAL_QUANTITIES_H

#include "HeuristicPerson.h"
#include "PersonBase.h"

class DumbDetermineOptimalQuantities : public virtual HeuristicPerson {

public:
	DumbDetermineOptimalQuantities();
	DumbDetermineOptimalQuantities(const PersonBase& other);
	DumbDetermineOptimalQuantities(const HeuristicPerson& other);

protected:
	void determineOptimalQuantities();

};

#endif
