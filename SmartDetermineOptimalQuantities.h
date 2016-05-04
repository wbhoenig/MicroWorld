#ifndef SMART_DETERMINE_OPTIMAL_QUANTITIES_H
#define SMART_DETERMINE_OPTIMAL_QUANTITIES_H

#include "HeuristicPerson.h"

class SmartDetermineOptimalQuantities : public virtual HeuristicPerson {

public:
	SmartDetermineOptimalQuantities();
	SmartDetermineOptimalQuantities(const PersonBase& other);
	SmartDetermineOptimalQuantities(const HeuristicPerson& other);


protected:
	void determineOptimalQuantities();

};

#endif
