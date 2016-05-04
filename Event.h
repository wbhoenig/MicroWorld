#ifndef EVENT_H
#define EVENT_H

#include <functional>

class Event {

public:
	void run();
	unsigned long getTime();

	Event(std::function<void(void)> func, unsigned long time);
	bool operator<(const Event& other) const;
	bool operator>(const Event& other) const;

private:
	std::function<void(void)> func;
	unsigned long time;
	Event();

};

#endif
