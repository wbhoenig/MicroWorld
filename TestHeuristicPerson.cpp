#include "TestHeuristicPerson.h"

TestHeuristicPerson::TestHeuristicPerson() : Adjust{}, Ration{}, WTP{} { }

TestHeuristicPerson::TestHeuristicPerson(const PersonBase& other) :
	Adjust{ other }, Ration{ other }, WTP{ other } { }
