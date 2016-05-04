#ifndef OYSTER_WTP_H
#define OYSTER_WTP_H

#include "HeuristicPerson.h"
#include "PersonBase.h"

class OysterWTP : public virtual HeuristicPerson {

public:
	OysterWTP();
	OysterWTP(const PersonBase& other);

protected:
	virtual void willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
		Pointer<PersonBase> other, bool& willing, unsigned short& willingness);

};

#endif