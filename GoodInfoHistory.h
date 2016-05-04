#ifndef GOODINFOHISTORY_H
#define GOODINFOHISTORY_H

#include "HelperStructs.h"
#include <vector>

class GoodInfoHistory {

	const static unsigned short GOOD_HISTORY_LIMIT = 5;


private:
	std::vector<QuantityPrice> history;

public:
	GoodInfoHistory();
	size_t size() const;
	const QuantityPrice& operator[](uint32_t num) const;
	void add(QuantityPrice info);

};

#endif
