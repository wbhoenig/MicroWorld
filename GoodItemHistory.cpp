#include "GoodInfoHistory.h"

GoodInfoHistory::GoodInfoHistory() : history{} { }

size_t GoodInfoHistory::size() const {
	return this->history.size();
}

const QuantityPrice& GoodInfoHistory::operator[](uint32_t num) const {
	return this->history[num];
}

void GoodInfoHistory::add(QuantityPrice info) {

	while (this->history.size() >= GOOD_HISTORY_LIMIT) {
		this->history.pop_back();
	}

	this->history.push_back(info);

}
