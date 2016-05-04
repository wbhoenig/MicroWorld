#include "HelperStructs.h"
#include <string>
#include <utility>

class PersonBase;

GoodMarginalCost::GoodMarginalCost(const GOOD_ID_TYPE& good, unsigned short marginalCost) :
	good{ good }, marginalCost{ marginalCost } {

}

GoodPriceQuantityMap merge(const std::vector<GoodPriceQuantityMap>& maps) {

	GoodPriceQuantityMap output;

	// All of the maps
	for (auto currentMap : maps) {

		// And all of the goods in the current map
		for (auto currentGood : currentMap) {

			auto currentGoodIt = output.find(currentGood.first);
			// Just add this to the current map
			if (currentGoodIt == output.end()) {

				output[currentGood.first] = currentGood.second;

			}
			// Merge
			else {

				// Go through all of the prices and add quantities
				for (auto currentQuantityPrice : currentGood.second) {

					// See if that price exists
					auto currentOutputQuantityPrice = currentGoodIt->second.find(currentQuantityPrice.second);
					if (currentOutputQuantityPrice == currentGoodIt->second.end()) {
						currentGoodIt->second[currentQuantityPrice.first] = currentQuantityPrice.second;
					}
					else {
						currentGoodIt->second[currentQuantityPrice.first] += currentQuantityPrice.second;
					}

				}

			}

		}

	}

	return output;

}

bool PriceQuantityMapCompare::operator()(std::pair<unsigned short, unsigned short> a, std::pair<unsigned short, unsigned short> b) {
	return a.first < b.first;
}

bool PriceQuantityMapReverseCompare::operator()(std::pair<unsigned short, unsigned short> a, std::pair<unsigned short, unsigned short> b) {
	return a.first > b.first;
}

PriceQuantityPerson::PriceQuantityPerson(unsigned short price, unsigned short quantity, Pointer<PersonBase> person) :
	price{ price }, quantity{ quantity }, person{ person } { }

bool PriceQuantityPersonCompare::operator()(const PriceQuantityPerson& a, const PriceQuantityPerson& b) {
	return (a.price < b.price);
}

bool PriceQuantityPerson::operator==(const PriceQuantityPerson& other) const {
	bool out = true;
	
	out &= (this->price == other.price);
	out &= (this->quantity == other.quantity);
	out &= (this->person == other.person);

	return out;
}
