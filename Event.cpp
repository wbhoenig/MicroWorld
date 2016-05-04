#include "Event.h"

Event::Event(std::function<void(void)> func, unsigned long time) :
	func{ func }, time{ time } {}

void Event::run() {
	this->func();
}

unsigned long Event::getTime() {
	return this->time;
}

bool Event::operator<(const Event& other) const {
	return (this->time < other.time);
}

bool Event::operator>(const Event& other) const {
	return (this->time > other.time);
}
