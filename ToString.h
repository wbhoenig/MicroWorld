#ifndef TO_STRING_H
#define TO_STRING_H

#include "HelperStructs.h"
#include "PersonBase.h"
#include "Driver.h"

#include <string>
#include <map>

template<typename T> struct ToString {
	std::string operator()(const T& t) {
		return std::to_string(t);
	}
};

template<> struct ToString<bool> {
	const std::string operator()(const bool& in) {

		if (in == true) {
			return "true";
		}
		return "false";

	}
};

template<> struct ToString<std::string> {
	const std::string& operator()(const std::string& t) {
		return t;
	}
};

template<> struct ToString<GoodQuantityMap> {
	const std::string operator()(const GoodQuantityMap& in) {

		std::string out;

		for (const auto& current : in) {

			const GOOD_ID_TYPE& currentGood{ current.first };
			const unsigned short& currentQuantity{ current.second };

			out += "(G: " + ToString<GOOD_ID_TYPE>{}(currentGood) + ", Q: ";
			out += ToString<unsigned short>{}(currentQuantity) + "); ";

		}

		return out;

	}
};

template<> struct ToString<GoodPriceQuantityMap> {
	std::string operator()(const GoodPriceQuantityMap& in) {

		std::string out;

		for (const auto& currentSellOffers : in) {

			const GOOD_ID_TYPE& currentGood{ currentSellOffers.first };
			const PriceQuantityMap& currentPqm{ currentSellOffers.second };

			out += ToString<GOOD_ID_TYPE>{}(currentGood) + ": ";

			for (const auto& pq : currentPqm) {

				const unsigned short& price{ pq.first };
				const unsigned short& quantity{ pq.second };
				out += "(Pr: " + ToString<unsigned short>{}(price)+", Q: " +
					ToString<unsigned short>{}(quantity)+"); ";

			}

		}

		return out;

	}
};

template<> struct ToString<std::map<Pointer<PersonBase>, GoodPriceQuantityMap > > {
	
	std::string operator()(const std::map<Pointer<PersonBase>, GoodPriceQuantityMap >& in) {

		std::string out;

		for (const auto& current : in) {

			Pointer<PersonBase> person{ current.first };
			const GoodPriceQuantityMap& gpqm{ current.second };

			out += "Selling [" + ToString<GoodPriceQuantityMap>{}(gpqm) + "] to person ";
			out += ToString<std::string>{}(Driver::getDriver()->getPersonName(person)) + "; ";

		}

		return out;

	}

};

template<> struct ToString<GoodPriceQuanityPersonMap> {

	std::string operator()(const GoodPriceQuanityPersonMap& in) {

		std::string out{};

		for (const auto& current : in) {

			const GOOD_ID_TYPE good{ current.first };
			const std::vector<PriceQuantityPerson> pqpVect{ current.second };

			for (const PriceQuantityPerson& pqp : pqpVect) {

				const unsigned short& price{ pqp.price };
				const unsigned short& quantity{ pqp.quantity };
				Pointer<PersonBase> person{ pqp.person };

				out += "(G: " + ToString<GOOD_ID_TYPE>{}(good) + ", ";
				out += "Pr: " + ToString<unsigned short>{}(price) + ", ";
				out += "Q: " + ToString<unsigned short>{}(quantity) + ", ";
				out += "Per: " + Driver::getDriver()->getPersonName(person) + "); ";

			}

		}

		return out;

	}

};

template<> struct ToString<std::map<Pointer<PersonBase>, unsigned short> > {
	std::string operator()(const std::map<Pointer<PersonBase>, unsigned short>& in) {
		
		std::string out{};

		for (const auto& current : in) {

			Pointer<PersonBase> currentPerson{ current.first };
			const unsigned short& currentAllocation{ current.second };

			out += "(Per: " + Driver::getDriver()->getPersonName(currentPerson);
			out += ", Q: " + ToString<unsigned short>{}(currentAllocation)+"); ";

		}

		return out;

	}
};

template<> struct ToString<PersonGoodQuantityMap> {
	std::string operator()(const PersonGoodQuantityMap& in) {
		
		std::string out{};

		for (const auto& currentPersonSet : in) {

			Pointer<const PersonBase> person{ currentPersonSet.first };
			const GoodQuantityMap gqm{ currentPersonSet.second };

			out += "[P: " + Driver::getDriver()->getPersonName(person);

			for (const auto& currentGq : gqm) {

				const GOOD_ID_TYPE& good{ currentGq.first };
				const unsigned short& quantity{ currentGq.second };

				out += " (G: " + ToString<GOOD_ID_TYPE>{}(good);
				out += ", Q: " + ToString<unsigned short>{}(quantity) + "); ";

			}

			out += "]; ";

		}

		return out;

	}
};

template<typename T> auto toString(const T& in) -> decltype(ToString<T>{}(in)) {

	return ToString<T>{}(in);

}

#endif