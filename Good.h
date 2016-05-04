/*#ifndef GOOD_H
#define GOOD_H

#include <stdint.h>
#include <string>

class Good {

// Public functions
public:
	// Legal constructor
	Good(const GOOD_ID_TYPE& type, uint64_t quantity);

	// Setter. We can adjust quantity but not type.
	void setQuantity(uint64_t quantity);

	// Getters
	const GOOD_ID_TYPE& getType();
	uint64_t getQuanity();

// Private variables
private:
	GOOD_ID_TYPE type;
	uint64_t quantity;

// Private functions
private:
	// No need for a default constructor
	Good();


};

#endif*/
