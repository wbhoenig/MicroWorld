#include "SmartRationSmartAdjustHeuristicPerson.h"

SmartRationSmartAdjustHeuristicPerson::SmartRationSmartAdjustHeuristicPerson() :
	SmartRation{}, SmartAdjust{}, SmartWTP{} { }

SmartRationSmartAdjustHeuristicPerson::SmartRationSmartAdjustHeuristicPerson(const PersonBase& other) :
	SmartRation{ other }, SmartAdjust{ other }, SmartWTP{ other } { }
