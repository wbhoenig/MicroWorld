#include "DumbRationDumbAdjustHeuristicPerson.h"
#include "Driver.h"

DumbRationDumbAdjustHeuristicPerson::DumbRationDumbAdjustHeuristicPerson() : 
	DumbAdjust{}, SmartWTP{} { }

DumbRationDumbAdjustHeuristicPerson::DumbRationDumbAdjustHeuristicPerson(const PersonBase& other) :
	DumbAdjust{ other }, SmartWTP{ other } { }

