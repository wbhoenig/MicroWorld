#include "Good.h"

// Legal constructor
Good::Good(const std::string& type, uint64_t quantity) : type{ type }, quantity{ quantity } {

}

// Quantity setter
void Good::setQuantity(uint64_t quantity) {
	this->quantity = quantity;
}

std::string Good::getType() {
	return this->type;
}

uint64_t Good::getQuanity() {
	return this->quantity;
}
