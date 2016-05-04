#include "Driver.h"
#include "PersonBase.h"
#include "TestHeuristicPerson.h"
#include "Parameters.h"
#include "ToString.h"

// XML
#include "tinyxml2.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <set>
#include <unordered_map>

void Driver::run() {

	// Clear the debug log
	if ((DEBUG_OUT == true) && (CLEAR_LOG == true)) {

		remove(LOG_PATH);

		std::ofstream file{};
		file.open(LOG_PATH, std::ios_base::app);
		file << "\"Person\",\"Class\",\"Function\",\"Message\"" << std::endl;

	}

	if (RESULTS_OUT == true) {

		remove(RESULTS_PATH);

		std::ofstream file{};
		file.open(RESULTS_PATH, std::ios_base::app);
		file << "\"Iteration\",\"Person\",\"Holding\",\"Buying\",\"Sell offers\"" << std::endl;

	}

	this->initialize();
	this->runQueue();
	this->end();

}

Driver* Driver::getDriver() {

	static Driver driver;
	return &driver;

}

Driver::Driver() : time{ 1 }, initialRuns{}, events{}, allPeople{}, allPeopleReverse{},
defaultPrice{ 1 }, nextGoodId{ 0 }, allGoods{}, iteration{ 0 }, round{ 0 } { }

unsigned long Driver::getIteration() const {
	return this->iteration;
}

void Driver::wait() {
	if (DRIVER_WAIT == true) {
		std::cin.ignore();
	}
}

void Driver::wait(bool ovr) {
	if (ovr == true) {
		std::cin.ignore();
	}
	else {
		this->wait();
	}
}

void Driver::initialize() {

	std::cout << PROMPT_TEXT;

	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile(DEFAULT_PATH);

	tinyxml2::XMLElement* world = xmlDoc.FirstChildElement("world");

	// Iterate through all of the nodes for our first pass.
	// Need better verification here.
	for (tinyxml2::XMLElement* currentNode = world->FirstChildElement("node");
	currentNode != nullptr; currentNode = currentNode->NextSiblingElement("node")) {

		const char* nodeName = currentNode->FirstChildElement("name")->GetText();
		Pointer<PersonBase> novusHomo{ new PersonImpl{} };
		this->allPeople[nodeName] = novusHomo;
		this->allPeopleReverse[novusHomo] = nodeName;

		// Money
		std::string moneyStr{ currentNode->FirstChildElement("money")->GetText() };
		unsigned short newMoney{ static_cast<unsigned short>(std::stoul(moneyStr)) };
		unsigned short adjustedNewMoney{ static_cast<unsigned short>(newMoney * 2) };
		novusHomo->setMoney(newMoney);
		this->defaultPrice = std::max(this->defaultPrice, adjustedNewMoney);

		tinyxml2::XMLElement* holdingsNode = currentNode->FirstChildElement("holdings");

		// Iterate through all of the holdings.
		for (tinyxml2::XMLElement* currentHolding = holdingsNode->FirstChildElement("holding");
		currentHolding != nullptr; currentHolding = currentHolding->NextSiblingElement("holding")) {

			std::string holdingName{ currentHolding->FirstChildElement("good")->GetText() };
			std::string holdingQuantityStr{ currentHolding->FirstChildElement("quantity")->GetText() };
			unsigned short holdingQuantity = static_cast<unsigned short>(
				std::stoul(holdingQuantityStr));

			novusHomo->endow(this->getGoodId(holdingName), holdingQuantity);
		}

		// Now set the multiple.
		std::string multipleStr{ currentNode->FirstChildElement("multiple")->GetText() };
		novusHomo->setUtilityMultiple(std::stod(multipleStr));

		// And now all of the powers.
		tinyxml2::XMLElement* powersNode = currentNode->FirstChildElement("powers");
		for (tinyxml2::XMLElement* currentPower = powersNode->FirstChildElement("power");
		currentPower != nullptr; currentPower = currentPower->NextSiblingElement("power")) {

			std::string powerName{ currentPower->FirstChildElement("good")->GetText() };
			std::string powerStr{ currentPower->FirstChildElement("power")->GetText() };
			double power{ std::stod(powerStr) };
			novusHomo->setUtilityPower(this->getGoodId(powerName), power);

		}

	}

	// Now we actually have to go through and connect all of the nodes to each other.
	for (tinyxml2::XMLElement* currentConnection = world->FirstChildElement("connection");
	currentConnection != nullptr; currentConnection = currentConnection->NextSiblingElement("connection")) {

		std::string nodeAStr{ currentConnection->Attribute("a") };
		std::string nodeBStr{ currentConnection->Attribute("b") };

		Pointer<PersonBase> nodeA = this->allPeople.find(nodeAStr)->second;
		Pointer<PersonBase> nodeB = this->allPeople.find(nodeBStr)->second;

		nodeA->connect(nodeB);
		nodeB->connect(nodeA);

	}

	// And read the nodes that we are supposed to run
	//std::set<Pointer<Person> > initialRuns;
	for (tinyxml2::XMLElement* currentRun = world->FirstChildElement("run");
	currentRun != nullptr; currentRun = currentRun->NextSiblingElement("run")) {

		const char* currentRunNode = currentRun->Attribute("node");
		this->initialRuns.insert(this->allPeople.find(currentRunNode)->second);

	}

}

void Driver::runInitial() {

	for (Pointer<PersonBase> currentRunPerson : this->initialRuns) {

		currentRunPerson->clear();

	}

	for (Pointer<PersonBase> currentRunPerson : this->initialRuns) {

		currentRunPerson->update();
		this->time += 1;

	}

}

void Driver::runQueue() {

	unsigned short propIteration{ 0 };

	//std::unordered_map<std::string, unsigned long> holdingsHistory;
	//std::vector<std::string> otherHoldingsHistory;
	std::vector<std::map<std::string, GoodQuantityMap> > holdingsHistory;
	//unsigned long iteration{ 0 };
	this->iteration = 0;
	while (1 < 7) {

		this->runInitial();

		if (iteration > 0) {
			std::cout << "Run (again)? (Y/n) ";
			char val;
			std::cin >> val;
			if ((val == 'n') || (val == 'N')) {
				break;
			}
		}

		while (this->events.empty() == false) {

			std::cout << "Iteration: " << iteration << std::endl;

			//std::cout << "Time: " << this->getTime() << "\n";

			// Run the top event.
			Event currentEvent = events.top();

			// Update the time
			this->time = currentEvent.getTime();

			currentEvent.run();
			this->events.pop();
			this->wait();

			// We can also declare dead if we've seen this before.
			// Means that we are in some sort of circular equilibrium.

			// Insert our current holdings.
			holdingsHistory.emplace_back();
			std::map<std::string, GoodQuantityMap>& holdingsBack{ holdingsHistory.back() };
			for (const auto& currentPerson : this->allPeople) {
				holdingsBack.emplace(currentPerson.first, currentPerson.second->getCurrentHoldings());
			}

			if ((STOP_ON_SAME_TRANSITION_BEFORE == true) &&
				(sameTransitionBefore(holdingsHistory) == true)) {
				std::cout << "Stopping because of same transition logic." << std::endl;
				break;
			}

			if ((STOP_ON_MAX_ITERATION == true) &&
				(iteration > MAX_ITERATION)) {
				std::cout << "Stopping because of max iteration." << std::endl;
				break;
			}

			++iteration;

		}

		holdingsHistory.clear();
		++propIteration;
		if (propIteration == MAX_PROP_ITERATION) {
			//this->debugOut();
			return;
		}

		// Clear the queue, because we don't want the old runs impacting these.
		while (this->events.empty() == false) {
			this->events.pop();
		}

		// Now handle propogation
		for (auto& currentPerson : this->allPeople) {
			currentPerson.second->propogateConnections();
		}

		bool done = true;
		for (auto& currentPerson : this->allPeople) {
			bool current = false;
			currentPerson.second->flopPropogatedConnections(current);
			done &= current;
		}

		if (done == true) {
			break;
		}

	}

	std::cout << "No events remaining.\n";
	this->wait(true);

}

void Driver::end() {

	// Delete all of the people
	for (auto currentPerson : this->allPeople) {
		std::cout << "About to delete: " << currentPerson.first << std::endl;
		delete currentPerson.second;
	}

}

void Driver::scheduleEvent(std::function<void(void)> func) {
	this->scheduleEvent(func, 1);
}

void Driver::scheduleEvent(std::function<void(void)> func, unsigned long offset) {
	this->events.emplace(func, this->time + offset);
}

unsigned long Driver::getTime() const {
	return this->time;
}

const unsigned short& Driver::getDefaultPrice() const {
	return this->defaultPrice;
}

Driver::EventQueue::size_type Driver::numEvents() const {

	return this->events.size();

}

const std::string& Driver::getPersonName(Pointer<const PersonBase> ptr) const {

	const auto& find = this->allPeopleReverse.find(ptr);
	if (find == this->allPeopleReverse.end()) {
		throw std::exception{ "Person does not exist." };
	}

	return find->second;

}

int main(void) {

	Driver* driver = Driver::getDriver();
	driver->run();
	return 0;

}

const GOOD_ID_TYPE& Driver::getGoodId(const std::string& good) {

	// Check if already there
	auto it{ this->allGoods.find(good) };
	if (it == this->allGoods.end()) {

		// Not already there, so insert.
		it = this->allGoods.emplace(good, NextGoodId<GOOD_ID_TYPE>{}(good, this->nextGoodId)).first;

	}

	return it->second;

}

unsigned long Driver::getRound() const {
	return this->round;
}

void Driver::incRound() {
	this->round += 1;
}

void Driver::logMessage(Pointer<const PersonBase> person, const std::string& className,
	const std::string& source, const std::string& message) const {

	std::string line;
	line += '"';
	line += this->getPersonName(person) + "\",";
	line += '"' + className + "\",";
	line += '"' + source + "\",";
	line += '"' + message + '"';

	std::ofstream file{};
	file.open(LOG_PATH, std::ios_base::app);
	file << line << std::endl;

}

void Driver::logResults(Pointer<const PersonBase> person, const std::string& holding,
	const std::string& buying, const std::string& sellOffers) const {

	std::string line;
	line += '"' + toString(this->iteration) + "\",\"";
	line += this->getPersonName(person) + "\",\"";
	line += holding + "\",\"" + buying + "\",\"" + sellOffers + '"';

	std::ofstream file{};
	file.open(RESULTS_PATH, std::ios_base::app);
	file << line << std::endl;

}
