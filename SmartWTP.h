#ifndef SMART_WTP_H
#define SMART_WTP_H

#include "HeuristicPerson.h"
#include "PersonBase.h"

class SmartWTP : public virtual HeuristicPerson {

public:
	SmartWTP();
	SmartWTP(const PersonBase& other);

protected:
	virtual void willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
		Pointer<PersonBase> other, bool& willing, unsigned short& willingness);

};

#endif
