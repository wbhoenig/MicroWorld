#ifndef DUMB_RATION_DUMB_ADJUST_HEURISTIC_PERSON_H
#define DUMB_RATION_DUMB_ADJUST_HEURISTIC_PERSON_H

#include "HeuristicPerson.h"
#include "Parameters.h"
#include "DumbAdjust.h"
#include "DumbRation.h"
#include "SmartWTP.h"

class DumbRationDumbAdjustHeuristicPerson :
	public DumbAdjust, public DumbRation, public SmartWTP {

public:
	DumbRationDumbAdjustHeuristicPerson();
	DumbRationDumbAdjustHeuristicPerson(const PersonBase& other);

};

#endif
