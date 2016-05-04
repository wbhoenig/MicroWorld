#ifndef MARGINAL_GOOD_CELL_H
#define MARGINAL_GOOD_CELL_H

#include "HelperStructs.h"

class MarginalGoodCell {

public:
	MarginalGoodCell();
	MarginalGoodCell(const MarginalGoodCell* notInclude, const MarginalGoodCell* include,
		const GoodQuantityMap& deltaHoldings, double deltaUtility);

	const double& getUtility() const;
	const GoodQuantityMap& getTotalHoldings() const;
	GoodQuantityMap& getTotalHoldings();

private:
	double utility;
	GoodQuantityMap deltaHoldings;
	const MarginalGoodCell* previousCell;
	GoodQuantityMap totalHoldings;

	static void updateDeltaHoldingsOut(GoodQuantityMap& out, const GoodQuantityMap& in);
	void initTotalHoldings();

};

#endif
