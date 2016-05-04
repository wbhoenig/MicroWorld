#include "DumbWTP.h"
#include "PersonBase.h"
#include "KnapsackSolver.h"
#include "KnapsackTable.h"

DumbWTP::DumbWTP() : HeuristicPerson{} { }

DumbWTP::DumbWTP(const PersonBase& other) : HeuristicPerson{ other } { }

void DumbWTP::willingnessToPay(const GOOD_ID_TYPE& good, unsigned short quantity,
	Pointer<PersonBase> other, bool& willing, unsigned short& willingness) {

	// Check if there is any demand at all
	if ((quantity == 0) || (this->demands(good) == false)) {
		willing = false;
		willingness = 0;
		return;
	}

	// Okay, now is when things get complicated.
	// Let's first get utility with *not* being offered this thing.
	std::vector<GoodPriceQuantityMap> unmerged{};

	for (const auto& currentPersonConnection : this->personTrades) {

		const GoodPriceQuantityMap& gpqm{ currentPersonConnection.second };
		Pointer<PersonBase> currentOther{ currentPersonConnection.first };
		unmerged.emplace_back(gpqm);
		GoodPriceQuantityMap& insertedGpqm = unmerged.back();

		// Modify if necessary
		if (other != currentOther) {
			continue;
		}

		auto goodIt{ insertedGpqm.find(good) };
		if (goodIt == insertedGpqm.end()) {
			continue;
		}

		goodIt->second.clear();

	}

	GoodPriceQuantityMap accessibleGoodsUninclusive{ merge(unmerged) };
	unmerged.clear();

	// And now the map for if we give it for free.
	GoodPriceQuantityMap accessibleGoodsInclusive{ accessibleGoodsUninclusive };

	accessibleGoodsInclusive[good][0] += quantity;

	// The utility that we have with this offer
	unsigned short moneyWithSales;
	GoodPriceQuantityMap realAccessibleGoods{
		this->getRealAccessibleGoods(moneyWithSales, accessibleGoodsUninclusive) };

	// Figure out what sort of utility we would get without the extra goods.
	GoodQuantityMap holdings{ this->utilityFunction.optimalQuantities(
		KnapsackSolver{}, moneyWithSales, realAccessibleGoods) };
	double oldUtility{ this->utilityFunction.getUtility(holdings) };

	realAccessibleGoods = this->getRealAccessibleGoods(moneyWithSales, accessibleGoodsInclusive);

	holdings = this->utilityFunction.optimalQuantities(KnapsackSolver{},
		moneyWithSales, realAccessibleGoods);

	double newUtility{ this->utilityFunction.getUtility(holdings) };
	double deltaUtility{ newUtility - oldUtility };

	willingness = static_cast<unsigned short>(deltaUtility) / quantity;
	
	willing = (willingness > 0);

}
