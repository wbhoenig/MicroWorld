#include "DumbRationSmartAdjustHeuristicPerson.h"

DumbRationSmartAdjustHeuristicPerson::DumbRationSmartAdjustHeuristicPerson() :
	DumbRation{}, SmartAdjust{}, SmartWTP{} { }

DumbRationSmartAdjustHeuristicPerson::DumbRationSmartAdjustHeuristicPerson(const PersonBase& other) :
	DumbRation{ other }, SmartAdjust{ other }, SmartWTP{ other } { }
