#include "SmartRationDumbAdjustHeuristicPerson.h"

SmartRationDumbAdjustHeuristicPerson::SmartRationDumbAdjustHeuristicPerson() : 
	DumbAdjust{}, SmartRation{}, SmartWTP{} { }

SmartRationDumbAdjustHeuristicPerson::SmartRationDumbAdjustHeuristicPerson(const PersonBase& other) :
	DumbAdjust{ other }, SmartRation{ other }, SmartWTP{ other } { }
