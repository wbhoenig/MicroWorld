#ifndef DUMB_RATION_SMART_ADJUST_HEURISTIC_PERSON_H
#define DUMB_RATION_SMART_ADJUST_HEURISTIC_PERSON_H

#include "SmartAdjust.h"
#include "DumbRation.h"
#include "SmartWTP.h"

class DumbRationSmartAdjustHeuristicPerson :
	public DumbRation, public SmartAdjust, public SmartWTP {

public:
	DumbRationSmartAdjustHeuristicPerson();
	DumbRationSmartAdjustHeuristicPerson(const PersonBase& other);

};

#endif
