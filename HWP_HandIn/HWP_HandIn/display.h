#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <avr/io.h>

// void init_7_segment(); No need for this
void display_7_segment(float value, uint8_t no_of_decimals);

#endif /* DISPLAY_H_ */