#ifndef SMART_RATION_SMART_ADJUST_HEURISTIC_PERSON_H
#define SMART_RATION_SMART_ADJUST_HEURISTIC_PERSON_H

#include "SmartAdjust.h"
#include "SmartRation.h"
#include "SmartWTP.h"

class SmartRationSmartAdjustHeuristicPerson :
	public SmartRation, public SmartAdjust, public SmartWTP {

public:
	SmartRationSmartAdjustHeuristicPerson();
	SmartRationSmartAdjustHeuristicPerson(const PersonBase& other);

};

#endif
