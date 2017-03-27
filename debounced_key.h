#ifndef DEBOUNCED_KEY_H_
#define DEBOUNCED_KEY_H_

#include "bit_manipulation.h"

void init_debounced_keys()
{
	CLEAR_BIT(DDRE, 4);
	CLEAR_BIT(DDRE, 5);
	// DDRE &= ~((1 << 4) | (1 << 5));
}

// key_no [1..2], return true if key pressed
uint8_t get_key(uint8_t key_no)
{
	if (key_no != 1 && key_no != 2)
		return 0;

	if ((1 << key_no + 3) & PINE)
		return 1;

	return 0;
}

#endif /* DEBOUNCED_KEY_H_ */
