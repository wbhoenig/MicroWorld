#include <memory>
#include "Agent.h"
#include "PersonBase.h"
#include "Driver.h"

void Agent::setMoney(unsigned short moneyEndowment) {
	this->moneyEndowment = moneyEndowment;
}

void Agent::updateNeighbors() {

	// Persons
	for (auto currentConnectedPerson : this->personTrades) {
		Pointer<PersonBase> ptr = currentConnectedPerson.first;
		Driver::getDriver()->scheduleEvent([ptr] {
			ptr->update();
		});
	}

}

void Agent::propogateConnections() {

	// Deal with the empty situation first
	if (this->slavePropogatedConnectedPersons.empty() == true) {

		for (auto current : this->directlyConnectedPersons) {

			this->masterPropogatedConnectedPersons.insert(current);

		}
		return;

	}
	else {
		this->masterPropogatedConnectedPersons = this->slavePropogatedConnectedPersons;
	}

	// Now selectively add.
	for (auto currentNeighbor : this->directlyConnectedPersons) {

		const std::set<Pointer<PersonBase> >& currentNeighborConnectedPersons{ currentNeighbor->getPropogatedConnectedPersons() };
		for (auto currentNeighborsNeighbor : currentNeighborConnectedPersons) {
			
			try {
				if (currentNeighborsNeighbor == Pointer<PersonBase>{ dynamic_cast<PersonBase*>(this) }) {
					continue;
				}
			}
			catch (std::bad_cast e) {
				continue;
			}

			this->masterPropogatedConnectedPersons.insert(currentNeighborsNeighbor);

		}

	}

	// Clear out the current trades, as they are null and void.
	this->personTrades.clear();
	for (auto currentPerson : this->masterPropogatedConnectedPersons) {
		auto it = this->personTrades.emplace(currentPerson, GoodPriceQuantityMap{});
		it.first->second.clear();
	}

}

void Agent::flopPropogatedConnections(bool& done) {

	if (this->slavePropogatedConnectedPersons == this->masterPropogatedConnectedPersons) {
		done = true;
		return;
	}

	this->slavePropogatedConnectedPersons = this->masterPropogatedConnectedPersons;
	this->masterPropogatedConnectedPersons.clear();
	done = false;

}

const std::set<Pointer<PersonBase> >& Agent::getPropogatedConnectedPersons() const {
	return this->slavePropogatedConnectedPersons;
}

// Constructors
Agent::Agent() : moneyEndowment{ 0 }, directlyConnectedPersons{}, slavePropogatedConnectedPersons{},
	personTrades{}, lastUpdateTime{ 0 } { }

Agent::Agent(const Agent& other) : moneyEndowment{ other.moneyEndowment }, directlyConnectedPersons{ other.directlyConnectedPersons },
masterPropogatedConnectedPersons{ other.masterPropogatedConnectedPersons },
slavePropogatedConnectedPersons{ other.slavePropogatedConnectedPersons },
personTrades{ other.personTrades }, lastUpdateTime{ other.lastUpdateTime } { }

bool Agent::operator==(const Agent& other) const {
	
	bool out = true;

	out &= (this->moneyEndowment == other.moneyEndowment);
	out &= (this->directlyConnectedPersons == other.directlyConnectedPersons);
	out &= (this->masterPropogatedConnectedPersons == other.masterPropogatedConnectedPersons);
	out &= (this->slavePropogatedConnectedPersons == other.slavePropogatedConnectedPersons);
	out &= (this->personTrades == other.personTrades);

	return out;

}

void Agent::clear() {
	this->__clear();
}

void Agent::__clear() {

	for (auto& currentPersonTrade : this->personTrades) {

		currentPersonTrade.second.clear();

	}

}
