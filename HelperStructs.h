#ifndef HELPERSTRUCTS_H
#define HELPERSTRUCTS_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "Parameters.h"

class PersonBase;

struct QuantityPrice {
	unsigned short quantity;
	unsigned short price;
};

struct GoodMarginalCost {
	GOOD_ID_TYPE good;
	unsigned short marginalCost;
	GoodMarginalCost(const GOOD_ID_TYPE& good, unsigned short marginalCost);
};

// Map a good to a quantity or a power or quantity/price tuple.
using GoodPowerMap = std::map<GOOD_ID_TYPE, double>;
using GoodQuantityMap = std::map<GOOD_ID_TYPE, unsigned short>;
using PriceQuantityMap = std::map<unsigned short, unsigned short>;
using GoodPriceQuantityMap = std::map<GOOD_ID_TYPE, PriceQuantityMap>;
using PersonGoodQuantityMap = std::map<Pointer<const PersonBase>, std::map<GOOD_ID_TYPE, unsigned short> >;

class PersonConnection;

struct PriceQuantityPerson {
	unsigned short price;
	unsigned short quantity;
	Pointer<PersonBase> person;
	PriceQuantityPerson(unsigned short price, unsigned short quantity, Pointer<PersonBase> person);
	bool operator==(const PriceQuantityPerson& other) const;
};

using GoodPriceQuanityPersonMap = std::map<GOOD_ID_TYPE, std::vector<PriceQuantityPerson> >;

GoodPriceQuantityMap merge(const std::vector<GoodPriceQuantityMap>&);

struct OldMarginalGoodCell {
	double utility;
	GoodQuantityMap goodQuantityMap;
	OldMarginalGoodCell() : utility{ 0 }, goodQuantityMap{} {}
};

struct PriceQuantityMapCompare {
	bool operator()(std::pair<unsigned short, unsigned short>, std::pair<unsigned short, unsigned short>);
};

struct PriceQuantityMapReverseCompare {
	bool operator()(std::pair<unsigned short, unsigned short>, std::pair<unsigned short, unsigned short>);
};

struct PriceQuantityPersonCompare {
	bool operator()(const PriceQuantityPerson&, const PriceQuantityPerson&);
};

// Other various helpers

template<typename T> struct NextGoodId {
	T operator()(const std::string& name, T& previous) {
		return previous++;
	}
};

template<> struct NextGoodId<std::string> {
	const std::string& operator()(const std::string& name, const std::string& previous) {
		return name;
	}
};

template<typename T>
bool sameTransitionBefore(const std::vector<T>& history) {

	// First check if this is even a doable test.
	if (history.size() < 2) {
		return false;
	}

	// Find the last time when the time before was different.
	bool success{ false };
	std::vector<T>::size_type loc{ 0 };
	for (std::vector<T>::size_type i = history.size() - 1; i > 0; --i) {

		// Not what we want if the time before was the same.
		if (history[i] == history[i - 1]) {
			continue;
		}

		// But it is what we want if find the break.
		success = true;
		loc = i - 1;
		break;

	}

	if (success == false) {
		return false;
	}

	// Now start at the beginning and try to find this again.
	for (std::vector<T>::size_type i = 0; i < loc; ++i) {

		// Bad if the firsts don't match up.
		if (history[i] != history[loc]) {
			continue;
		}

		// Also bad if the seconds don't match up.
		if (history[i + 1] != history[loc + 1]) {
			continue;
		}

		// Wow, we found the place!
		return true;

	}

	return false;

}

#endif
