#ifndef TEST_HEURISTIC_PERSON_H
#define TEST_HEURISTIC_PERSON_H

#include "DumbAdjust.h"
#include "SmartAdjust.h"
#include "DumbRation.h"
#include "SmartRation.h"
#include "DumbWTP.h"
#include "SmartWTP.h"
#include "WeirdWTP.h"
#include "OysterWTP.h"
#include "DumbDetermineOptimalQuantities.h"
#include "PersonBase.h"

using Adjust = DumbAdjust;
using Ration = DumbRation;
using WTP = WeirdWTP;
using Determine = DumbDetermineOptimalQuantities;

class TestHeuristicPerson :
	public Adjust, public Ration, public WTP, public DumbDetermineOptimalQuantities {

public:
	TestHeuristicPerson();
	TestHeuristicPerson(const PersonBase& other);

};

#endif
