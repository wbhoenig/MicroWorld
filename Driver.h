#ifndef DRIVER_H
#define DRIVER_H

#include "Event.h"
#include "Parameters.h"
#include "HelperStructs.h"

#include <functional>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <set>

class PersonBase;

class Driver {

	using EventQueue = std::priority_queue<Event, std::vector<Event>, std::greater<Event> >;

public:
	void run();
	void scheduleEvent(std::function<void(void)> func);
	void scheduleEvent(std::function<void(void)> func, unsigned long offset);
	static Driver* getDriver();
	unsigned long getTime() const;
	const std::string& getPersonName(Pointer<const PersonBase> ptr) const;
	const unsigned short& getDefaultPrice() const;
	EventQueue::size_type numEvents() const;
	unsigned long getIteration() const;
	unsigned long getRound() const;
	void incRound();
	void logMessage(Pointer<const PersonBase> person, const std::string& className,
		const std::string& source, const std::string& message) const;
	void logResults(Pointer<const PersonBase> person, const std::string& holding,
		const std::string& buying, const std::string& sellOffers) const;

private:
	Driver();
	void wait();
	void wait(bool ovr);
	void initialize();
	void runQueue();
	void runInitial();
	void end();
	unsigned long time;
	EventQueue events;

	unsigned long round;

	// Our database of people
	std::map<std::string, Pointer<PersonBase> > allPeople;
	std::map<Pointer<const PersonBase>, std::string> allPeopleReverse;
	std::map<std::string, GOOD_ID_TYPE> allGoods;
	unsigned short defaultPrice;

	GOOD_ID_TYPE nextGoodId;
	const GOOD_ID_TYPE& getGoodId(const std::string& good);

	std::set<Pointer<PersonBase> > initialRuns;

	unsigned long iteration;

};

#endif
