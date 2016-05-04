#ifndef SMART_RATION_DUMB_ADJUST_HEURISTIC_PERSON_H
#define SMART_RATION_DUMB_ADJUST_HEURISTIC_PERSON_H

#include "HeuristicPerson.h"
#include "Parameters.h"
#include "DumbAdjust.h"
#include "SmartRation.h"
#include "SmartWTP.h"

class SmartRationDumbAdjustHeuristicPerson :
	public DumbAdjust, public SmartRation, public SmartWTP {

public:
	SmartRationDumbAdjustHeuristicPerson();
	SmartRationDumbAdjustHeuristicPerson(const PersonBase& other);

};

#endif
