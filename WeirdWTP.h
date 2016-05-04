#ifndef WEIRD_WTP_H
#define WEIRD_WTP_H

#include "HeuristicPerson.h"
#include "PersonBase.h"

class WeirdWTP : public virtual HeuristicPerson {

public:
	WeirdWTP();
	WeirdWTP(const PersonBase& other);

protected:
	virtual void willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
		Pointer<PersonBase> other, bool& willing, unsigned short& willingness);

};

#endif