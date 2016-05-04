#include "KnapsackTable.h"
#include "MarginalGoodCell.h"

#include <exception>

KnapsackTable::KnapsackTable(const GoodPriceQuantityMap& gpqm,
	const UtilityFunction* utilityFunction) :
	table{}, utilityFunction{ utilityFunction }, 
	marginalGoods{ }, money{ 0 } {

	// Build out maginal goods with the gpqm.
	for (auto currentGood : gpqm) {

		for (auto currentPriceQuantity : currentGood.second) {

			const unsigned short& currentQuantity = currentPriceQuantity.second;
			const unsigned short& currentPrice = currentPriceQuantity.first;

			for (unsigned long i = 0; i < currentQuantity; ++i) {
				this->marginalGoods.emplace_back(currentGood.first, currentPrice);
			}

		}
	}
	
	table.reserve(this->marginalGoods.size() + 1);

	// Add the zero good row
	table.emplace_back();
	table.back().emplace_back();

	// Add the zero dollar column
	for (const GoodMarginalCost& goodMarginalCost : this->marginalGoods) {

		table.emplace_back();
		table.back().emplace_back();

	}

}

KnapsackTable::~KnapsackTable() {
	this->utilityFunction = nullptr;
}

KnapsackTable::KnapsackTable() {
	throw std::exception{ "Invalid constructor called;" };
}

void KnapsackTable::addDollar() {

	++(this->money);

	// Have to solve for every row
	table[0].emplace_back();
	for (decltype(this->table)::size_type i = 1; i < this->table.size(); ++i) {

		const GoodMarginalCost& marginalGood{ this->marginalGoods[i - 1] };
		const MarginalGoodCell* notIncludeCell{ &(this->table[i - 1].back()) };
		MarginalGoodCell* includeCell{ nullptr };
		const GOOD_ID_TYPE& goodName{ marginalGood.good };
		GoodQuantityMap deltaHoldings{};
		deltaHoldings.emplace(goodName, 1);

		double deltaUtility{ 0 };

		if (this->money >= marginalGood.marginalCost) {

			unsigned short previousMoney = this->money - marginalGood.marginalCost;
			includeCell = &(this->table[i - 1][previousMoney]);
			deltaUtility = utilityFunction->marginalUtility(includeCell->getTotalHoldings(), goodName);

		}

		this->table[i].emplace_back(notIncludeCell, includeCell, deltaHoldings, deltaUtility);

	}

}

void KnapsackTable::addDollars(unsigned short dollars) {

	for (unsigned int i = 0; i < dollars; ++i) {
		this->addDollar();
	}

}

const unsigned short& KnapsackTable::getMoney() const {
	return this->money;
}

double KnapsackTable::maxUtility() const {

	return this->maxUtilityCell().getUtility();

}

const MarginalGoodCell& KnapsackTable::maxUtilityCell() const {

	return this->table.back().back();

}

const unsigned short& KnapsackTable::addMoneyUntilUtility(double util) {

	const MarginalGoodCell* current{ &(this->maxUtilityCell()) };
	while (current->getUtility() < util) {
		this->addDollar();
		current = &(this->maxUtilityCell());
	}

	return this->getMoney();

}
