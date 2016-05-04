#ifndef AGENT_H
#define AGENT_H

#include <memory>
#include <set>
#include <map>

#include "Good.h"
#include "HelperStructs.h"
#include "Parameters.h"

class PersonBase;

class Agent {


public:
	// The main function.
	virtual void update() = 0;
	virtual void clear();
	void setMoney(unsigned short moneyEndowment);

	const std::set<Pointer<PersonBase> >& getPropogatedConnectedPersons() const;

	void propogateConnections();
	void flopPropogatedConnections(bool& done);

	bool operator==(const Agent& other) const;

protected:
	void updateNeighbors();

	// Assets.
	unsigned short moneyEndowment;

	// All of the other Agents to which this Agent is connected.
	std::set<Pointer<PersonBase> > directlyConnectedPersons;
	std::set<Pointer<PersonBase> > masterPropogatedConnectedPersons;
	std::set<Pointer<PersonBase> > slavePropogatedConnectedPersons;
	std::map<Pointer<PersonBase>, GoodPriceQuantityMap> personTrades;

	unsigned long lastUpdateTime;

	Agent();
	Agent(const Agent& other);

	void __clear();

};

#endif
