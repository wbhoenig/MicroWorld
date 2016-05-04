#ifndef KNAPSACK_TABLE_H
#define KNAPSACK_TABLE_H

#include "MarginalGoodCell.h"
#include "UtilityFunction.h"
#include "HelperStructs.h"

class KnapsackTable {

public:
	KnapsackTable(const GoodPriceQuantityMap& gpqm, const UtilityFunction* utilityFunction);
	~KnapsackTable();

	void addDollar();
	void addDollars(unsigned short dollars);
	const unsigned short& getMoney() const;
	double maxUtility() const;
	const MarginalGoodCell& maxUtilityCell() const;

	// Returns the amount of money needed to do this.
	const unsigned short& addMoneyUntilUtility(double util);

private:
	// [goods available][dollars]
	std::vector<std::vector<MarginalGoodCell> > table;
	std::vector<GoodMarginalCost> marginalGoods;
	const UtilityFunction* utilityFunction;
	unsigned short money;

	// Useless constructor. Crashes intentionally.
	KnapsackTable();

};

#endif
