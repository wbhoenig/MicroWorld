#ifndef DUMB_WTP_H
#define DUMB_WTP_H

#include "HeuristicPerson.h"
#include "PersonBase.h"

class DumbWTP : public virtual HeuristicPerson {

public:
	DumbWTP();
	DumbWTP(const PersonBase& other);

protected:
	virtual void willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
		Pointer<PersonBase> other, bool& willing, unsigned short& willingness);

};

#endif
